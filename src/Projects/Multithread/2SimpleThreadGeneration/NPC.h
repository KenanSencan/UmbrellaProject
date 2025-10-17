#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

static inline std::atomic<int> counter{0};
static inline std::chrono::steady_clock::time_point timerStart;
static inline std::once_flag timerStartFlag;
static inline std::once_flag timerStopFlag;

// NPC class - rectangle with random color that runs performance tests
class NPC
{
private:
    sf::RectangleShape shape;
    sf::Color color;
    std::unique_ptr<std::thread> workThread;
    std::atomic<bool> isRunning;

public:
    NPC(sf::Vector2f position, sf::Vector2f size) : isRunning(true)
    {
        shape.setSize(size);
        shape.setPosition(position);

        color = sf::Color(50,50,50);
        shape.setFillColor(color);

        // Start heavy work in background thread
        workThread = std::make_unique<std::thread>([this]() {
            while (isRunning)
            {
                HeavyWorkOptimized();
            }
        });
    }

    ~NPC()
    {
        isRunning = false;
        if (workThread && workThread->joinable())
        {
            workThread->join();
        }
    }

    // Delete copy constructor and copy assignment
    NPC(const NPC&) = delete;
    NPC& operator=(const NPC&) = delete;

    // Move constructor and move assignment
    NPC(NPC&& other) noexcept:
        shape(std::move(other.shape))
        , color(other.color)
        , workThread(std::move(other.workThread))
        , isRunning(other.isRunning.load())
    {
    }

    NPC& operator=(NPC&& other) noexcept
    {
        if (this != &other)
        {
            shape = std::move(other.shape);
            color = other.color;
            workThread = std::move(other.workThread);
            isRunning.store(other.isRunning.load());
        }
        return *this;
    }

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

    static void HeavyWorkOptimized()
    {
        // Warm-up CPU caches
        std::vector<int> warmup(1000);
        std::accumulate(warmup.begin(), warmup.end(), 0);
        std::vector<size_t> test_sizes = {1000, 10'000, 100'000, 1'000'000, 10'000'000, 10'000'000};

        const size_t hardware_threads = std::thread::hardware_concurrency();
        std::cout << "Using " << hardware_threads << " threads for parallel computation" << std::endl;

        for (size_t size : test_sizes)
        {
            std::cout << "\n=== Testing with " << size << " elements (PARALLEL) ===" << std::endl;

            std::vector<int> numbers(size);
            std::iota(numbers.begin(), numbers.end(), 1);

            // Parallel computation
            auto start_parallel = std::chrono::steady_clock::now();

            // Calculate chunk size for each thread
            size_t chunk_size = size / hardware_threads;
            std::vector<std::thread> threads;
            std::vector<long long> partial_sums(hardware_threads, 0);

            // Launch threads
            for (size_t i = 0; i < hardware_threads; ++i)
            {
                size_t start_idx = i * chunk_size;
                size_t end_idx = (i == hardware_threads - 1) ? size : (i + 1) * chunk_size;

                threads.emplace_back([&numbers, &partial_sums, i, start_idx, end_idx]() {
                    partial_sums[i] = std::accumulate(
                        numbers.begin() + start_idx,
                        numbers.begin() + end_idx,
                        0LL
                    );
                });
            }

            // Join all threads
            for (auto& thread : threads)
            {
                thread.join();
            }

            // Sum the partial results
            long long sum_parallel = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);

            auto end_parallel = std::chrono::steady_clock::now();
            auto time_parallel = std::chrono::duration_cast<std::chrono::microseconds>(end_parallel - start_parallel);

            // Start timer on first increment (using atomic compare-exchange to avoid race)
            std::call_once(timerStartFlag, []() {
               timerStart = std::chrono::steady_clock::now(); 
            });

            counter++;
            std::cout << counter << std::endl;
            std::cout << "Total Time ms : " << time_parallel.count() << " (Sum: " << sum_parallel << ")" << std::endl;

            // Check if counter reached 1000 and stop timer (using atomic compare-exchange)
            if (counter >= 1000)
            {
                std::call_once(timerStopFlag, []() {
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
        // Heavy work runs in background thread, nothing to do here
        // This keeps the game loop running smoothly at 60fps
    }

    void Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};