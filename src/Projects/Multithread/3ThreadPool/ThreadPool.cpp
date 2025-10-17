#include "ThreadPool.h"

    ThreadPool::ThreadPool(size_t numThreads) : stop(false)
    {
        // Create worker threads
        for(size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back(
                [this]
                {
                    for(;;)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queueMutex);

                            // Wait until there's a task or the pool is stopped
                            this->condition.wait(lock,
                                [this]
                                {
                                    return this->stop || !this->tasks.empty();
                                });

                            // If stopped and no tasks remain, exit
                            if(this->stop && this->tasks.empty())
                                return;

                            // Get the next task
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }

                        // Execute the task
                        task();
                    }
                }
            );
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }

        // Wake up all threads
        condition.notify_all();

        // Wait for all threads to finish
        for(std::thread& worker: workers)
        {
            if(worker.joinable())
                worker.join();
        }
    }
