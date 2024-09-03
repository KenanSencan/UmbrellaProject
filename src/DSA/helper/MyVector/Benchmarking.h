#pragma once

#include <benchmark/benchmark.h>
#include "MyVector.h"
#include "../Helper.h"

class Benchmarking
{
public:
    static MyVector* Setup()
    {
        constexpr int size = 5;
        Helper::WriteRandomValToPath(size);

        MyVector* TestVector = new MyVector();
        TestVector->AllocateArray(size);
        int* Arr = TestVector->GetArr();
        Helper::ReadFromPathAssignToArr(Arr, size);
        return TestVector;
    }

    inline static void AddBenchmarks()
    {
        MyVector* Vector = Setup();
        std::cout << "Vector allocated at: " << Vector << std::endl;
        
        benchmark::RegisterBenchmark("MySorting", [Vector](benchmark::State& state) 
        {  
            MyVector* TestVector = new MyVector();
            TestVector->copyFrom(*Vector);
     
            for (auto _ : state)
             {
                Sorting::MySorting(*TestVector);
            }
            delete TestVector;
        })->Unit(benchmark::kMillisecond)->Iterations(882);

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
