#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>
#include <unordered_set>

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

int main(int argc, char **argv) {
    // Seed the random number generator to ensure consistency across runs
    std::mt19937 rng(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(1, 10000);

    // Create a fixed input array to use in the benchmarks
    const int size = 100000;
    int A[size];
    for (int i = 0; i < size; ++i) {
        A[i] = dist(rng);
    }

    const int x = 150;  // Target sum to find in the array

    benchmark::Initialize(&argc, argv);
    benchmark::RegisterBenchmark("Solution1", [A, size, x](benchmark::State &state) {
        for (auto _: state) {
            Solution1(A, size, x, state);  // Call the solution function with fixed input
        }
    })->Unit(benchmark::kMillisecond);

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
