#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <cstdint>
#include <memory>


namespace Core
{
	class JobSystem
	{
		public:
			using Job = std::function<void(std::uint32_t)>;

			explicit JobSystem(std::uint32_t threadCount = std::thread::hardware_concurrency());

			~JobSystem();
			JobSystem(const JobSystem&) = delete;
			JobSystem& operator=(const JobSystem&) = delete;

			void ParallelFor(std::uint32_t count, const Job& job);

		private:
			void WorkerLoop(std::uint32_t workerIndex);

			struct JobBatch
			{
				Job job;
				std::uint32_t count = 0;
				std::atomic<std::uint32_t> nextIndex{0};
				std::atomic<std::uint32_t> finished{0};

				std::mutex mtx;
				std::condition_variable cv;
			};

			std::vector<std::thread> m_Workers;
			std::queue<std::shared_ptr<JobBatch>> m_Queue;

			std::mutex m_QueueMutex;
			std::condition_variable m_QueueCV;
			std::atomic<bool> m_Running{ true };
		};
}