#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>
#include <unordered_set>
#include "Helper.h"

// Brute-force solution
void Solution1(const int A[], const int size, const int x, benchmark::State &state) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (A[i] + A[j] == x) {
                state.counters["ReturnVal"] = 0;
                return;
            }
        }
    }
    state.counters["ReturnVal"] = 1;
}

//Two pointer technique. Time Complexity: O(NlogN), Time complexity for sorting the array
//NOTE: Faster than the binary search !!!!
void TwoPointer(int *A, int arrSize, int sum, benchmark::State &state) //, benchmark::State& state
{
    std::sort(A, A + arrSize);
    int left = 0, right = arrSize - 1;

    while (right > left || left < right) {
        if (A[left] + A[right] == sum) {
            state.counters["ReturnVal"] = 0;
            return;
        };
        if (A[left] + A[right] > sum) right--;
        else if (A[left] + A[right] < sum) left++;
    }
    state.counters["ReturnVal"] = 1;
}

void BinarySearch(int *A, int arrSize, int sum, benchmark::State &state) {
    std::sort(A, A + arrSize);
    int high = arrSize - 1, low = 0;

    while (low < high) {
        int currentSum = A[low] + A[high];

        if (currentSum == sum) {
            state.counters["returnVal"] = 0;
            return;
        } else if (currentSum < sum)
            low++;
        else
            high--;
    }

    state.counters["returnVal"] = 1;
}


void Hashing(int arr[], int arr_size, int sum, benchmark::State &state) {
    std::unordered_set<int> s;
    for (int i = 0; i < arr_size; i++) {
        int temp = sum - arr[i];

        if (s.find(temp) != s.end()) {
            int first = temp;
            int sec = arr[i];
            int result = first + sec;
            state.counters["returnVal"] = 0;

            return;
        }
        s.insert(arr[i]);
    }
    state.counters["returnVal"] = 1;
}

int ReturnRandomElementAddUp(int *A, int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, size - 1);

    int first = 0;
    int sec = 0;

    while (first == sec) {
        first = A[dis(gen)];
        sec = A[dis(gen)];

        if (first != sec) return first + sec;
    }
    return -1;
}

int main(int argc, char **argv) {
    constexpr int size = 50000;
    Helper::WriteRandomValToPath(size);
    int A4[size];
    Helper::ReadFromPathAssignToArr(A4, size);
    int FindAddUp = ReturnRandomElementAddUp(A4, size);

    benchmark::Initialize(&argc, argv);
    benchmark::RegisterBenchmark("Solution1", [size, &A4, FindAddUp](benchmark::State &state) {
        int *ptr = new int[size];
        std::copy_n(A4, size, ptr);
        for (auto _: state)
            Solution1(ptr, size, FindAddUp, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);
    benchmark::RegisterBenchmark("TwoPointer", [size, &A4, FindAddUp](benchmark::State &state) {
        int *ptr = new int[size];
        std::copy_n(A4, size, ptr);
        for (auto _: state)
            TwoPointer(ptr, size, FindAddUp, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("BinarySearch", [size, &A4, FindAddUp](benchmark::State &state) {
        int *ptr = new int[size];
        std::copy_n(A4, size, ptr);
        for (auto _: state)
            BinarySearch(ptr, size, FindAddUp, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RegisterBenchmark("Hashing", [size, &A4, FindAddUp](benchmark::State &state) {
        int *ptr = new int[size];
        std::copy_n(A4, size, ptr);
        for (auto _: state)
            Hashing(ptr, size, FindAddUp, state);
        delete ptr;
    })->Unit(benchmark::kMillisecond);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}

// -----------------------------------------------------------------------
// Benchmark             Time             CPU   Iterations UserCounters...
// -----------------------------------------------------------------------
// Solution1          8.56 ms         6.84 ms          112 ReturnVal=0
// TwoPointer        0.037 ms        0.027 ms        29867 ReturnVal=0
// BinarySearch      0.040 ms        0.032 ms        23579 returnVal=0
// Hashing           0.386 ms        0.298 ms         2987 returnVal=0
