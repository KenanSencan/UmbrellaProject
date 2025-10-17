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
    std::atomic<bool> isRunning;

public:
    NPC(sf::Vector2f position, sf::Vector2f size) : isRunning(true)
    {
        shape.setSize(size);
        shape.setPosition(position);

        color = sf::Color(50,50,50);
        shape.setFillColor(color);
        
    }

    ~NPC()
    {
        isRunning = false;
    }

    // Delete copy constructor and copy assignment
    NPC(const NPC&) = delete;
    NPC& operator=(const NPC&) = delete;

    // Move constructor and move assignment
    NPC(NPC&& other) noexcept:
        shape(std::move(other.shape))
        , color(other.color)
        , isRunning(other.isRunning.load())
    {
    }

    NPC& operator=(NPC&& other) noexcept
    {
        if (this != &other)
        {
            shape = std::move(other.shape);
            color = other.color;
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
    
    void Update()
    {
        HeavyWork();
        // Heavy work runs in background thread, nothing to do here
        // This keeps the game loop running smoothly at 60fps
    }

    void Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};