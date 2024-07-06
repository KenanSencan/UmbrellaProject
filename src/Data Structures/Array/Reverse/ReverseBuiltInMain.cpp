#include <iostream>
#include <stack>
#include <benchmark/benchmark.h>
#include "Helper.h"

void BuiltInMethod(int originalArray[], int length)
{
    std::reverse(&originalArray[0], &originalArray[length]);

    // Print the reversed array
    // for (int i = 0; i < length; i++) {
    //     std::cout << originalArray[i] << " ";
    // }
}

//Create a new array, from behind -> Front insert. I could also instead start the for loop from behind 
void reverseArrayExtraArray(int arr[], const int size)
{
    int* reversedArr = new int[size];
    for (int i = 0; i < size; i++)
    {
        reversedArr[i] = arr[size - i - 1];
    }

    // for (int i = 0; i < size; i++)
    //     std::cout << arr[i] << " " ;
    // std::cout << std::endl;
    delete reversedArr;
}

/* reverse arr[] from start to end inside loop*/
void ReverseWithLoop(int arr[], int start, int end)
{
    while (start < end)
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }

    // for (int i = 0; i < size; i++)
    //     std::cout << arr[i] << " ";
    //
    // std::cout << std::endl;
}

void reverseArrayUsingStack(int arr[], int size)
{
    std::stack<int> stack;

    // Push elements onto the stack
    for (int i = 0; i < size; i++)
    {
        stack.push(arr[i]);
    }

    // Pop elements from the stack to reverse the array
    for (int i = 0; i < size; i++)
    {
        arr[i] = stack.top();
        stack.pop();
    }
}

int main(int argc, char** argv)
{
    //Set up
    constexpr int size = 1000;
    Helper::GenerateRandomValToPath("C:/Users/Selviniah/Desktop/cmake-sfml-project/src/input.txt", size);
    int originalArray[size];
    Helper::ReadFromPathAssignToArr(originalArray, "C:/Users/Selviniah/Desktop/cmake-sfml-project/src/input.txt");
    int length = sizeof(originalArray) / sizeof(originalArray[0]);

    benchmark::Initialize(&argc, argv);
    benchmark::RegisterBenchmark("BuiltInMethod", [&originalArray,length](benchmark::State& state)
    {
        for (auto _ : state)
        {
            int* firstArray = originalArray;
            BuiltInMethod(firstArray, length);
        }
    });

    benchmark::RegisterBenchmark("reverseArrayExtraArray", [&originalArray,length](benchmark::State& state)
    {
        for (auto _ : state)
        {
            int* secArray = originalArray;
            reverseArrayExtraArray(secArray, length);
        }
    });

    benchmark::RegisterBenchmark("ReverseWithLoop", [&originalArray,length](benchmark::State& state)
    {
        for (auto _ : state)
        {
            int* secArray = originalArray;
            ReverseWithLoop(secArray, 0, length - 1);
        }
    });

    benchmark::RegisterBenchmark("reverseArrayUsingStack", [&originalArray,length](benchmark::State& state)
    {
        for (auto _ : state)
        {
            int* FourthArray = originalArray;
            reverseArrayUsingStack(FourthArray, length);
        }
    });

    int arr[] = {1, 2, 3, 4, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);
    ReverseWithLoop(arr, 0, n - 1);

    //4. method
    int* FourthArray = originalArray;
    reverseArrayUsingStack(FourthArray, length);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}

/*
*-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BuiltInMethod                64.1 ns         60.9 ns     10000000
reverseArrayExtraArray        467 ns          414 ns      1659259
ReverseWithLoop               242 ns          220 ns      2986667
reverseArrayUsingStack      21662 ns        19252 ns        37333
*/
