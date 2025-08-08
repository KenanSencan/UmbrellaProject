#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

// Modified function to return a value and be more realistic
int ComplexTaskOriginal(int taskId)
{
    auto start = std::chrono::high_resolution_clock::now();

    // Simulate complex calculation
    for (long long i = 0; i < 10000000000; ++i)
    {
        
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Task " << taskId << " completed in " << duration.count() << "ms" << std::endl;
    return 0;
}

// I have 16 threads so it will do this 16 times
void AllThreadsDoingIt()
{
    std::cout << "\n=== Scenario 1: Maximum Resources ===\n";

    // Get number of hardware threads available
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Hardware threads available: " << numThreads << std::endl;

    // For R7 3700X (8 threads), we'll use all available threads
    std::vector<std::thread> threads;
    std::atomic<int> totalResult{0};

    auto start = std::chrono::high_resolution_clock::now();

    // Divide work among threads
    threads.reserve(numThreads);
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([i, &totalResult]() {
            int result = ComplexTaskOriginal(i);
            totalResult += result;
        });
    }

    // Wait for all threads to complete
    for (auto& t : threads)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "Total execution time: " << duration.count() << " seconds\n";
    std::cout << "Combined result: " << totalResult << std::endl;
}

//NOTE: Best
int ComplexTaskDivided(int taskId, int totalThreads) {
    const auto start = std::chrono::high_resolution_clock::now();

    constexpr long long totalIterations = 10000000000LL;
    const long long iterationsPerThread = totalIterations / totalThreads;
    const long long startIdx = taskId * iterationsPerThread;
    const long long endIdx = (taskId == totalThreads - 1) ? totalIterations : startIdx + iterationsPerThread;
    
    // Each thread only does its portion!
    volatile long long dummy = 0; // volatile prevents optimization
    for (long long i = startIdx; i < endIdx; ++i) {
        dummy = i; // Prevent compiler from optimizing away the loop
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Task " << taskId << " completed " 
              << (endIdx - startIdx) / 1000000 << " million iterations in " 
              << duration.count() << "ms" << std::endl;
    return 0;
}

//NOTE: BEEEEEEEEEEEEEEEEEEEST
void correctApproach() {
    std::cout << "\n=== CORRECT APPROACH (Using ComplexTaskDivided) ===\n";
    std::cout << "Creating 8 threads, each doing 1/8th of the work:\n\n";
    
    const int numThreads = 16;
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Each thread does only its portion
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([i, numThreads]() {
            ComplexTaskDivided(i, numThreads);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    std::cout << "\nTotal execution time: " << duration.count() << " seconds\n";
    std::cout << "Total work done: 10 billion iterations (divided among threads)\n";
    std::cout << "✅ Each thread did exactly its share!\n";
}

// Single-threaded baseline
void runSingleThread() {
    std::cout << "\n=== Single Thread (Baseline) ===\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    ComplexTaskOriginal(0);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Total execution time: " << duration.count() << " seconds\n";
}

//NOTE: IDK WHat this doing FIXED approach (GOOD - divides work)
void scenario1_MaxResourcesFixed() {
    std::cout << "\n=== Scenario 1: FIXED (Divides Work) ===\n";
    
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Using " << numThreads << " threads\n";
    
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Each thread does only its portion
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.emplace_back([i, numThreads]() {
            ComplexTaskDivided(i, numThreads);
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    std::cout << "Total execution time: " << duration.count() << " seconds\n";
    std::cout << "Total work done: 10 billion iterations (divided among threads)\n";
}

int main()
{
    std::cout << "C++ Multithreading Examples\n";
    std::cout << "CPU: R7 3700X (4 cores, 8 threads)\n";

    correctApproach();
    // Run scenario 2
    // scenario2_BackgroundExecution();

    // Show thread pool alternative
    // scenario2_ThreadPool();

    return 0;
}