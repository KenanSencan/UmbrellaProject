#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <stdexcept>

    class ThreadPool
    {
    public:
        // Constructor: creates the thread pool with the specified number of worker threads
        explicit ThreadPool(size_t numThreads);

        // Destructor: stops all threads and waits for them to finish
        ~ThreadPool();

        // Enqueue a task to be executed by the thread pool
        // Returns a future that can be used to get the result
        template<class F, class... Args>
        std::future<std::invoke_result_t<F, Args...>> enqueue(F&& f, Args&&... args);

        // Get the number of worker threads
        size_t size() const { return workers.size(); }

        // Check if the thread pool has been stopped
        bool isStopped() const { return stop; }

    private:
        // Worker threads
        std::vector<std::thread> workers;

        // Task queue
        std::queue<std::function<void()>> tasks;

        // Synchronization
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
    };

    template<class F, class... Args>
    std::future<std::invoke_result_t<F, Args...>> ThreadPool::enqueue(F&& f, Args&&... args)
    {
        using return_type = std::invoke_result_t<F, Args...>;

        std::shared_ptr<std::packaged_task<return_type()>> task = std::make_shared<std::packaged_task<return_type()>>
        (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            // Don't allow enqueueing after stopping the pool
            if(stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]()
            {
                (*task)();
            });
        }
        condition.notify_one();
        return res;
    }
