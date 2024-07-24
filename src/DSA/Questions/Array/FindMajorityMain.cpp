//Find majority element in an array. It has to be minimum more than n/2 times
//A[] = {3,4,2,4,2,4,4} Output: 4. Frequency of 4, is greater than the half of the size of the array size.
//A[] = {3,3,4,2,4,4,2,4} Output: None. None of the elements are more frequent than the half of the size of the array
//Being equal to size of the array is not acceptable it has to be minimum "(n / 2) + 1"  

#include <benchmark/benchmark.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Helper.h"
#include <filesystem>

//O(N^2)
void BruteForce(const int Arr[], const int ArrSize, benchmark::State& state) //,
{
    for (int i = 0; i < ArrSize; ++i)
    {
        int counter = 0;
        for (int j = 0; j < ArrSize; ++j)
        {
            if (Arr[i] == Arr[j])
            {
                counter++;
            }
        }
        if (counter > ArrSize / 2)
        {
            state.counters["ReturnVal"] = 0;
            // std::cout << Arr[i];
        }
    }
    state.counters["ReturnVal"] = 1;
}

//Solution 1 O(N) time O(N) Auxilary 
void Hashmap(const int Arr[], const int ArrSize, benchmark::State& state)
{
    benchmark::MemoryManager* mn = nullptr;
    benchmark::RegisterMemoryManager(mn);

    std::unordered_map<int, int> EncountNumbers;
    EncountNumbers.insert({Arr[0], 1}); //O(1)   
    for (int i = 1; i < ArrSize; ++i) //O(ArrSize)
    {
        EncountNumbers[Arr[i]]++;
    }

    // Find the max incremented element O(arrSize)
    auto maxNumber = std::max_element(EncountNumbers.begin(), EncountNumbers.end(), [](auto& a, auto& b)
    {
        return a.second < b.second;
    });

    ////More cooler solution
    // const int max_element = maxNumber->second;
    // if (max_element > ArrSize / 2)
    //     std::cout << max_element;
    // else
    //     std::cout << "max element so far is " << max_element << " minimum expectation is: " << ArrSize / 2;

    //or
    for (auto pair : EncountNumbers)
    {
        if (pair.second > ArrSize / 2)
        {
            state.counters["ReturnVal"] = 0;
            return;
        }
    }
    state.counters["ReturnVal"] = 1;
}


//The algorithm BoyerMoore
//O(N) Time O(1) Auxilary 
void BoyerMooreMethod(const int Arr[], const int size, benchmark::State& state)
{
    int maj_index = 0, count = 1;
    for (int i = 1; i < size; i++)
    {
        if (Arr[maj_index] == Arr[i])
            count++;
        else
            count--;
        if (count == 0)
        {
            maj_index = i;
            count = 1;
        }
    }

    //maj_index is determined. The found element is mostly repetitive element but still it's not sure it's > n/2. So final check really if it's majority element in an array
    count = 0;
    for (int i = 0; i < size; i++)

        if (Arr[i] == Arr[maj_index])
            count++;

    if (count > size / 2)
        state.counters["ReturnVal"] = 0;
    else
        state.counters["ReturnVal"] = 1;
}

//There's Binary search tree solution for this question but since I don't have literally any knowledge on binary trees, I will skip this and come back when I learned. Here's the link for BS and problem's link
//https://www.geeksforgeeks.org/binary-search-tree-data-structure/

int main(int argc, char** argv)
{
    constexpr int size = 10000;
    const std::filesystem::path inputPath = std::filesystem::current_path().parent_path() / "src" / "input.txt";
    std::cout << inputPath.string() << std::endl;
    Helper::WriteRandomValToPath(size);
    int arr[size];
    Helper::ReadFromPathAssignToArr(arr, size);

    benchmark::Initialize(&argc, argv);

    benchmark::RegisterBenchmark("BruteForce", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            BruteForce(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("Hashmap", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            Hashmap(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);


    benchmark::RegisterBenchmark("BoyerMooreMethod", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            BoyerMooreMethod(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    //MSVC
    // ---------------------------------------------------------------------------
    // Benchmark                 Time             CPU   Iterations UserCounters...
    // ---------------------------------------------------------------------------
    // BruteForce             6.18 ms         4.10 ms          236 ReturnVal=1
    // Hashmap               0.748 ms        0.479 ms         1338 ReturnVal=1
    // BoyerMooreMethod      0.024 ms        0.015 ms        40727 ReturnVal=1

    //G++
    // Run on (16 X 3593 MHz CPU s)
    // Load Average: 0.52, 0.58, 0.59
    // ---------------------------------------------------------------------------
    // Benchmark                 Time             CPU   Iterations UserCounters...
    // ---------------------------------------------------------------------------
    // BruteForce             6.39 ms         3.70 ms          249 ReturnVal=1
    // Hashmap               0.641 ms        0.372 ms         1723 ReturnVal=1
    // BoyerMooreMethod      0.006 ms        0.004 ms       203636 ReturnVal=1
    
}
