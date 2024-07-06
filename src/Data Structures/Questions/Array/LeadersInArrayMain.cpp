//Write a program to print all the LEADERS in the array. An element is a leader if it is greater than all the elements to its right side. And the rightmost element is always a leader.
//Input: arr[] = {16, 17, 4, 3, 5, 2}, 
// Output: 17,5,2

#include <stack>
#include <vector>
#include <benchmark/benchmark.h>

#include "Helper.h"
using namespace std;

//Works but extremely slow. Time cmplexity is: O(N * N) because there's 2 nested loops
void printLeaders(int arr[], const int size, benchmark::State& state)
{
    std::vector<int> Leaders;
    for (int i = 0; i < size; i++)
    {
        int j;
        for (j = i + 1; j < size; j++)
        {
            if (arr[i] <= arr[j])
                break;
        }
        if (j == size) // the loop didn't break
            Leaders.push_back(arr[i]);
    }
    state.counters["Leaders"] = static_cast<double>(Leaders.size());
    // std::cout << "\n";
}

//Time complexity O(n)
void printLeaders2(int arr[], int size, benchmark::State& state)
{
    std::vector<int> Leaders;
    //Determine the biggest number.
    //When moving from right to left, if the current index not bigger than the previous one, count that in
    //and assign that as biggest number
    int biggestNumber = arr[size - 1];
    for (int i = size - 2; i >= 0; --i) //move backwards
    {
        if (arr[i] > biggestNumber)
        {
            Leaders.push_back(arr[i]);
            biggestNumber = arr[i];
        }
    }

    state.counters["Leaders"] = static_cast<double>(Leaders.size());
}

//Roughly same as the last method. Instead stack is used
void printLeaders3(int arr[], int size, benchmark::State& state)
{
    std::vector<int> Leaders;

    /* create stack to store leaders*/
    stack<int> sk;
    sk.push(arr[size - 1]);

    for (int i = size - 2; i >= 0; i--)
    {
        if (arr[i] >= sk.top())
        {
            sk.push(arr[i]);
        }
    }

    /* print stack elements*/ /* run loop till stack is not empty*/
    while (!sk.empty())
    {
        Leaders.push_back(sk.top());
        sk.pop();
    }
    state.counters["Leaders"] = static_cast<double>(Leaders.size());
}

int main(int argc, char** argv)
{
    constexpr int size = 50000;
    Helper::GenerateRandomValToPath("C:/Users/Selviniah/Desktop/cmake-sfml-project/src/input.txt", size);
    int arr[size];
    Helper::ReadFromPathAssignToArr(arr, "C:/Users/Selviniah/Desktop/cmake-sfml-project/src/input.txt");

    benchmark::Initialize(&argc, argv);

    benchmark::RegisterBenchmark("printLeaders", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            printLeaders(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("printLeaders2", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            printLeaders2(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("printLeaders3", [&arr, &size](benchmark::State& state)
    {
        int* ptr = new int[size];
        std::copy_n(arr, size, ptr);
        for (auto _ : state)
            printLeaders3(ptr, size, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);
    
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;

//------------------------------------------------------------------------
// Benchmark              Time             CPU   Iterations UserCounters...
// ------------------------------------------------------------------------
// printLeaders       0.172 ms        0.120 ms        10000 Leaders=8
// printLeaders2      0.025 ms        0.018 ms        40727 Leaders=7
// printLeaders3      0.036 ms        0.024 ms        32000 Leaders=10
}
