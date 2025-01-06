cmake_minimum_required(VERSION 3.28)
project(MyString LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(StringMain StringExecutable.cpp MyString.h)

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
target_link_libraries(StringMain PRIVATE benchmark::benchmark)
target_include_directories(StringMain PRIVATE ${CurrentPath}/dep/benchmark/include)

if (${CUSTOM_STD} STREQUAL "ON")
    target_link_libraries(StringMain PRIVATE c++ c++abi unwind)
endif ()

#Tests
enable_testing()
add_executable(TestMyString Test.cpp MyString.h)
#Register the tests with CTest
add_test(NAME MyStringTests COMMAND TestMyString)

#Add a dependency. This way building StringMain also builds TestMyString first
add_dependencies(StringMain TestMyString)

#Run the tests every time before StringMain finishes building
add_custom_target(run_tests ALL
        DEPENDS TestMyString
        COMMAND "${CMAKE_CTEST_COMMAND}" -R MyStringTests --output-on-failure
        COMMENT "Running MyStringTests..."
)
add_dependencies(StringMain run_tests)

#This is not necessary
#file(GLOB_RECURSE MYFILE LIST_DIRECTORIES true CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/DSA/CustomDS/MyString/*)
