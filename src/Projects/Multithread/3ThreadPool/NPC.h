#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>
#include <future>
#include "ThreadPool.h"

// NPC class - rectangle with random color that runs performance tests
static inline std::atomic<int> counter = 0;
static inline std::chrono::steady_clock::time_point timerStart;
static inline std::once_flag TimerStartFlag;
static inline std::once_flag TimerStopFlag;

class NPC
{
private:
    sf::RectangleShape shape;
    sf::Color color;
    ThreadPool& threadPool;  // Reference to shared thread pool
    std::future<void> workFuture;  // Track the running task

public:
    NPC(sf::Vector2f position, sf::Vector2f size, ThreadPool& pool)
        : threadPool(pool)
    {
        shape.setSize(size);
        shape.setPosition(position);

        color = sf::Color(50,50,50);
        shape.setFillColor(color);

        // Submit initial heavy work task to the pool
        workFuture = threadPool.enqueue([this]() {
            HeavyWorkOptimizedWithPool(threadPool);
        });
    }

    // No need for custom destructor - future handles cleanup
    ~NPC() = default;

    // Delete copy constructor and copy assignment (future is non-copyable)
    NPC(const NPC&) = delete;
    NPC& operator=(const NPC&) = delete;

    // Move constructor (needed for vector::emplace_back)
    NPC(NPC&& other) noexcept
        : shape(std::move(other.shape))
        , color(other.color)
        , threadPool(other.threadPool)  // References can't be moved, just rebind to same object
        , workFuture(std::move(other.workFuture))
    {
    }

    // Delete move assignment (can't reassign references)
    NPC& operator=(NPC&& other) noexcept = delete;

    static void HeavyWork()
    {
        // Warm-up CPU caches
        std::vector<int> warmup(1000);
        std::accumulate(warmup.begin(), warmup.end(), 0);
        std::vector<size_t> test_sizes = {1000, 10'000, 100'000, 1'000'000, 10'000'000, 10'000'000};

        for (size_t size : test_sizes)
        {
            std::cout << "\n=== Testing with " << size << " elements ===" << std::endl;

            std::vector<int> numbers(size);
            std::iota(numbers.begin(), numbers.end(), 1);

            // Sequential
            auto start_seq = std::chrono::steady_clock::now();
            std::accumulate(numbers.begin(), numbers.end(), 0LL);
            auto end_seq = std::chrono::steady_clock::now();
            auto time_seq = std::chrono::duration_cast<std::chrono::microseconds>(end_seq - start_seq);
            std::cout << "Total Time ms : " << time_seq.count();
        }
    }

    static void HeavyWorkOptimizedWithPool(ThreadPool& pool)
    {
        // Warm-up CPU caches
        std::vector<int> warmup(1000);
        std::accumulate(warmup.begin(), warmup.end(), 0);
        std::vector<size_t> test_sizes = {1000, 10'000, 100'000, 1'000'000, 10'000'000, 10'000'000, 100'000'000, 100'000'000'0, 100'000'000'0};

        const size_t num_threads = pool.size();
        std::cout << "Using ThreadPool with " << num_threads << " threads for parallel computation" << std::endl;

        for (size_t size : test_sizes)
        {
            std::cout << "\n=== Testing with " << size << " elements (THREADPOOL) ===" << std::endl;

            std::vector<int> numbers(size);
            std::iota(numbers.begin(), numbers.end(), 1);

            // Parallel computation using ThreadPool
            auto start_parallel = std::chrono::steady_clock::now();

            // Calculate chunk size for each thread
            size_t chunk_size = size / num_threads;
            std::vector<std::future<long long>> futures;

            // Submit tasks to the ThreadPool (no thread creation!)
            for (size_t i = 0; i < num_threads; ++i)
            {
                size_t start_idx = i * chunk_size;
                size_t end_idx = (i == num_threads - 1) ? size : (i + 1) * chunk_size;

                // Submit task and get future
                futures.push_back(pool.enqueue([&numbers, start_idx, end_idx]() -> long long
                {
                    return std::accumulate(
                        numbers.begin() + start_idx,
                        numbers.begin() + end_idx,
                        0LL
                    );
                }));
            }

            // Collect results from futures (waits for completion)
            long long sum_parallel = 0;
            for (auto& future : futures)
            {
                sum_parallel += future.get();
            }

            auto end_parallel = std::chrono::steady_clock::now();
            auto time_parallel = std::chrono::duration_cast<std::chrono::microseconds>(end_parallel - start_parallel);

            std::call_once(TimerStartFlag, []() {
                timerStart = std::chrono::steady_clock::now();
            });

            counter++;
            std::cout << counter << std::endl;
            std::cout << "Total Time ms : " << time_parallel.count() << " (Sum: " << sum_parallel << ")" << std::endl;

            // Check if counter reached 1000 and stop timer (using atomic compare-exchange)
            if (counter >= 1000)
            {
                std::call_once(TimerStopFlag, []() {
                    auto timerEnd = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timerEnd - timerStart);
                    std::cout << "\n========================================" << std::endl;
                    std::cout << "TIMER STOPPED: Counter reached 1000!" << std::endl;
                    std::cout << "Total elapsed time: " << elapsed.count() << " ms" << std::endl;
                    std::cout << "========================================\n" << std::endl;
                });
            }
        }
    }
    
    void Update()
    {
        // Check if the current task is finished
        if (workFuture.valid() &&
            workFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            // Task is done, submit a new one
            workFuture = threadPool.enqueue([this]() {
                HeavyWorkOptimizedWithPool(threadPool);
            });
        }
        // If task is still running, do nothing - keeps game at 60fps
    }

    void Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};