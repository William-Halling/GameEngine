#include "JobSystem.hpp"


namespace Core
{
	JobSystem::JobSystem(std::uint32_t threadCount)
	{
		if (threadCount == 0)
			threadCount = 1;

		m_Workers.reserve(threadCount);

		for (std::uint32_t i = 0; i < threadCount; ++i)
		{
			m_Workers.emplace_back([this, i]() { WorkerLoop(i); });
		}
	}

	JobSystem::~JobSystem()
	{
		m_Running.store(false);
		m_QueueCV.notify_all();

		for (auto& t : m_Workers)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
	}

	void JobSystem::ParallelFor(std::uint32_t count, const Job& job)
	{
		if (count == 0)
			return;

		auto* batch = new JobBatch{};
		batch->job = job;
		batch->count = count;
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_Queue.push(batch);
		}
		m_QueueCV.notify_all();


		while (batch->finished.load(std::memory_order_acquire) < count)
		{
			std::this_thread::yield();
		}

		delete batch;
	}

	void JobSystem::WorkerLoop(std::uint32_t)
	{
		while (m_Running.load(std::memory_order_relaxed))
		{
			JobBatch* batch = nullptr;

			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				
				m_QueueCV.wait(lock, [this]()
				{
					return !m_Running.load(std::memory_order_relaxed) || !m_Queue.empty();
				});


				if (!m_Running.load(std::memory_order_relaxed))
					break;

				if (!m_Queue.empty())
				{
					batch = m_Queue.front();
					m_Queue.pop();
				}
			}

			if (!batch)
				continue;


			while (true)
			{
				std::uint32_t index = batch->nextIndex.fetch_add(1, std::memory_order_relaxed);
				
				if (index >= batch->count)
					break;

				batch->job(index);
				batch->finished.fetch_add(1, std::memory_order_release);
			}
		}
	}
}