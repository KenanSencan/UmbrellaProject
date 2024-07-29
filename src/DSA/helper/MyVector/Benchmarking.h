#pragma once

#include <benchmark/benchmark.h>
#include "MyVector.h"
#include "../Helper.h"

class Benchmarking
{
public:
    static MyVector Setup()
    {
        constexpr int size = 10000;
        Helper::WriteRandomValToPath(size);

        MyVector MyVector;
        MyVector.AllocateArray(size);
        int* Arr = MyVector.GetArr();
        Helper::ReadFromPathAssignToArr(Arr, size);
        return MyVector;
    }

    inline static void AddBenchmarks()
    {
        MyVector* Vector = new MyVector(Setup());

        benchmark::RegisterBenchmark("MySorting", [Vector](benchmark::State& state) 
        {  
            MyVector* TestVector = new MyVector();
            TestVector->copyFrom(*Vector);
     
            for (auto _ : state)
             {
                Sorting::MySorting(*TestVector);
            }
        })->Unit(benchmark::kMillisecond);

        // benchmark::RegisterBenchmark("SelectionSort", [&Vector](benchmark::State& state)
        // {
        //     const MyVector& TestVector = Vector;y
        //     for (auto _ : state)
        //     {
        //         Sorting::SelectionSort(TestVector);
        //     }
        // })->Unit(benchmark::kMillisecond);
        //
        // benchmark::RegisterBenchmark("BubbleSort", [&Vector](benchmark::State& state)
        // {
        //     const MyVector& TestVector = Vector;
        //     for (auto _ : state)
        //     {
        //         Sorting::BubbleSort(TestVector);
        //     }
        // })->Unit(benchmark::kMillisecond);
        std::cout << "hello world";
    }
};
