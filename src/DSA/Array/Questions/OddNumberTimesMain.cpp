//All numbers will occur even number of times. Only one element will occur odd times find that element [1,2,3,2,3,1,3]
//1: repeated 2 times
//2: repeated 2 times
//3: repeated 3 times. This is the odd number 

#include <bitset>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <benchmark/benchmark.h>

#include "Helper.h"

#define BENCHMARKING

void BruteForce(const int Arr[], const int Size, benchmark::State& state)
{
    for (int i = 0; i < Size; ++i)
    {
        int Counter = 0;
        for (int j = 0; j < Size; ++j)
        {
            if (Arr[i] == Arr[j])
                Counter++;
        }

        if (Counter % 2 != 0)
        {
            // std::cout << "Element " << Arr[i] << " repeated " << Counter << "times which is odd " << std::endl;
            state.counters["El"] = Arr[i];
            return;
        }
    }
}

void HashMap(int Arr[], int Size, benchmark::State& state, const bool ShouldReserve)
{
    std::unordered_map<int, int> Counter;

    if (ShouldReserve) Counter.reserve(Size);
    
    for (int i = 0; i < Size; ++i)
    {
        Counter[Arr[i]]++;
    }

    for (auto pair : Counter)
    {
        if (pair.second % 2 != 0)
        {
            // std::cout << "Element " << pair.first << " repeated " << pair.second << " times which is odd " << std::endl;
            state.counters["El"] = pair.first;
        }
    }
}

//So important. USE XOR to solve this problem
int getOddOccurrence(const int ar[], int ar_size, benchmark::State& state)
{
    int res = 0;
    for (int i = 0; i < ar_size; i++)
        res = res ^ ar[i];

    state.counters["res"] = res;
    return res;
}


int main(int argc, char** argv)
{
    //Prepare the array
     constexpr int size = 100001;
    // int [size];
    int* arr = new int[size];
    for (int i = 0; i <= size - 2; ++i) //Till last element insert 0 and 1 depending on odd or even
    {
        arr[i] = i % 2 == 0 ? 0 : 1;
    }
    arr[size-1] = 31; //This is the last odd number that will be considered odd

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
    benchmark::RegisterBenchmark("HashMapNoReserve", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            HashMap(ptr, size, state, false);
        delete ptr;
    })->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("HashMapWithReserve", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            HashMap(ptr, size, state, true);
        delete ptr;
    })->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("XOR solution", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            getOddOccurrence(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;

    // constexpr int size = std::size(Arr);
    // BruteForce(Arr, size);
    // HashMap(Arr,size);
    delete arr;
}

// -----------------------------------------------------------------------------
// Benchmark                   Time             CPU   Iterations UserCounters...
// -----------------------------------------------------------------------------
// BruteForce                603 ms          383 ms            2 El=31
// HashMapNoReserve        0.412 ms        0.267 ms         2635 El=31
// HashMapWithReserve      0.739 ms        0.561 ms         1867 El=31
// XOR solution            0.005 ms        0.003 ms       248889 res=31

//Quite interesting how Reserving memory for std::unordered_map, slower than not reserving one 