cmake_minimum_required(VERSION 3.28)
project(MyVectorImplementation)
set(CMAKE_CXX_STANDARD 17)

add_library(SourceFiles OBJECT MyVector.h MyVector.cpp Sorting.h Sorting.cpp Benchmarking.h)
add_executable(VectorMain VectorExecutable.cpp)
target_link_libraries(VectorMain PRIVATE SourceFiles)

# Determine the correct parent path
set(levels_up 3)
set(CurrentPath ${CMAKE_SOURCE_DIR})

foreach (i RANGE ${levels_up})
    cmake_path(GET CurrentPath PARENT_PATH CurrentPath)
endforeach ()

message(STATUS "Benchmark path: ${CurrentPath}/dep/benchmark")
message(STATUS "Another: ${CMAKE_SOURCE_DIR}/dep/benchmark")
message(STATUS "Binary is at: ${CMAKE_BINARY_DIR}")

# Set Benchmark options
set(BENCHMARK_ENABLE_TESTING OFF)

# Link and include benchmark
target_link_libraries(VectorMain PRIVATE benchmark::benchmark)
target_include_directories(VectorMain PRIVATE ${CurrentPath}/dep/benchmark/include)
