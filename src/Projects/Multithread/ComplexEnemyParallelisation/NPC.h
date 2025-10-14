#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <memory>

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

        // Random color using C++11 random
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 255);

        color = sf::Color(dis(gen), dis(gen), dis(gen));
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
        std::vector<size_t> test_sizes = {1000, 10'000, 100'000, 1'000'000};

        for (size_t size : test_sizes)
        {
            std::cout << "\n=== Testing with " << size << " elements ===" << std::endl;

            std::vector<int> numbers(size);
            std::iota(numbers.begin(), numbers.end(), 1);

            // Force data into cache
            volatile int dummy = numbers[0] + numbers[size - 1];

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
        std::vector<size_t> test_sizes = {1000, 10'000, 100'000, 1'000'000, 1'000'000'0, 1'000'000'0};

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
            std::cout << "Total Time ms : " << time_parallel.count();
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