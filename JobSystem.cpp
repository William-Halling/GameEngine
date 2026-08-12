#include "JobSystem.hpp"
#include <algorithm>
#include <cassert>

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
		m_Running.store(false, std::memory_order_release);
		m_QueueCV.notify_all();

		for (auto &t : m_Workers)
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

		auto batch = std::make_shared<JobBatch>();
		batch->job = job;
		batch->count = count;

		{
			std::lock_guard<std::mutex> lk(m_QueueMutex);
        	m_Queue.push(batch);
		}
		m_QueueCV.notify_all();


		m_QueueCV.notify_all();

		// Wait for completion using condition variable to avoid busy-wait
		std::unique_lock<std::mutex> lk(batch->mtx);
		batch->cv.wait(lk, [&]() { return batch->finished.load(std::memory_order_acquire) >= batch->count; });
	}

	void JobSystem::WorkerLoop(std::uint32_t)
	{
		while (m_Running.load(std::memory_order_acquire))
		{
			std::shared_ptr<JobBatch> batch;

			{
				std::unique_lock<std::mutex> lk(m_QueueMutex);
				m_QueueCV.wait(lk, [this]() { return !m_Running.load(std::memory_order_acquire) || !m_Queue.empty(); });

				if (!m_Running.load(std::memory_order_acquire) && m_Queue.empty())
					return;

				if (!m_Queue.empty())
				{
					batch = m_Queue.front();
					m_Queue.pop();
				}
			}

			if (!batch) 
			{
				continue;
			}

			while (true)
			{
				std::uint32_t index = batch->nextIndex.fetch_add(1, std::memory_order_relaxed);
				
				if (index >= batch->count) 
				{
					break;
				}
				try
				{
					batch->job(index);
				}
				catch (...)
				{
					
				}

				auto finished = batch->finished.fetch_add(1, std::memory_order_acq_rel) + 1;
				
				if (finished >= batch->count)
				{
					std::lock_guard<std::mutex> lk(batch->mtx);
					batch->cv.notify_all();
				}
			}
		}
	}
}