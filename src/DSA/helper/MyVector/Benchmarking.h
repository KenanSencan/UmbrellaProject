// #pragma once
//
// #include <benchmark/benchmark.h>
// #include "MyVector.h"
// #include "../Helper.h"
//
// class Benchmarking
// {
// public:
//     static MyVector* Setup()
//     {
//         constexpr int size = 5;
//         Helper::WriteRandomValToPath(size);
//
//         MyVector* TestVector = new MyVector();
//         TestVector->AllocateArray(size);
//         int* Arr = TestVector->GetArr();
//         Helper::ReadFromPathAssignToArr(Arr, size);
//         return TestVector;
//     }
//
//     inline static void AddBenchmarks()
//     {
//         MyVector* Vector = Setup();
//         std::cout << "Vector allocated at: " << Vector << std::endl;
//
//         benchmark::RegisterBenchmark("MySorting", [Vector](benchmark::State& state)
//         {
//             for (auto _ : state)
//             {
//                 MyVector originalVector;
//                 originalVector.Add(10);
//                 originalVector.Add(20);
//                 originalVector.Add(30);
//                 originalVector.Add(40);
//                 originalVector.Add(50);
//
//                 originalVector.Insert(1, 15);
//
//                 MyVector copiedVector(originalVector);
//                 copiedVector.Add(40);
//                 copiedVector.RemoveIndex(1);
//                 MyVector assignedVector = originalVector;
//                 assignedVector.Add(50);
//                 assignedVector.RemoveIndex(0);
//
//                 assignedVector.Swap(0, 3);
//                 assignedVector.Swap(40, 15, false);
//                 originalVector.Add(-15);
//
//             }
//         })->Unit(benchmark::kMillisecond);
//     }
// };
