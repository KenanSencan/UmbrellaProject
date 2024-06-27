#TODO: Later on I also want to execute all the benchmarks with given BM suffix end of the file name. Also a way to execute arbitrary BM would not be bad
# Present option to the user how to build the project: FETCHCONTENT, VCPKG, or SOURCEBUILD
set(BuildType "VCPKG" CACHE STRING "Specify how to build the project: FetchContent, Vcpkg, or SourceBuild")
set(BuildTypeEnum "FetchContent" "Vcpkg" "SourceBuild")

# Convert elements to upper to be not case sensitive 
string(TOUPPER ${BuildType} BuildType)
list(TRANSFORM BuildTypeEnum TOUPPER OUTPUT_VARIABLE UpperBuildTypeEnum)

# Set the toolchain vcpkg file only if not building from source
if (BuildType STREQUAL "VCPKG")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()

# Initial configurations
cmake_minimum_required(VERSION 3.15)
project(CMakeSFMLProject)
set(CMAKE_CXX_STANDARD 23)

# Set the build type
set(CMAKE_BUILD_TYPE Debug)
set(VCPKG_BUILD_TYPE Debug)

# Set benchmark configurations
set(BENCHMARK_FORMAT "console")
set(BENCHMARK_OUT_FORMAT "console")
set(BENCHMARK_OUT "${CMAKE_BINARY_DIR}/benchmark/hello.txt")

# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
# Include recursively every cpp file along with their paths ending with "ArrayMain.cpp" 
file(GLOB_RECURSE MAIN_FILES "${CMAKE_SOURCE_DIR}/src/**/*main.cpp")

# Handle creating executables for each cpp file ending with "main" 
foreach (MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${MAIN_FILE})
endforeach ()

# Define the benchmark executable
add_executable(MyBenchmark src/Benchmarks/MyBenchmarkMain.cpp)

# Temporarily adding the main executable for the project.
add_executable(main src/MainMain.cpp)

# Print the variables
message(STATUS "CMake version is: ${CMAKE_VERSION}")

# For now I am doing everything for Windows, later on I will consider doing cross-platform. Abort if it's not Windows
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR "Detected OS is not Windows, for now I am doing every stuff in Windows")
endif ()

# Print the given or default BuildType
if (NOT BuildType IN_LIST UpperBuildTypeEnum)
    message(FATAL_ERROR "Invalid value for BuildType: ${BuildType}. Allowed values are: ${BuildTypeEnum}")
else ()
    message(STATUS "BuildType is ${BuildType}")
endif ()

# Make the dependency library building with FetchContent
if (BuildType STREQUAL "FETCHCONTENT")
    include(FetchContent)

    # SFML fetch
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x)
    FetchContent_MakeAvailable(SFML)

    target_link_libraries(main PRIVATE sfml-graphics)

    # FLTK Fetch
    set(FLTK_COMMIT "branch-1.3" CACHE STRING "FLTK branch name, tag, or commit")
    FetchContent_Declare(FLTK
            GIT_REPOSITORY https://github.com/fltk/fltk
            GIT_TAG "${FLTK_COMMIT}"
    )
    FetchContent_MakeAvailable(FLTK)
    target_include_directories(main PRIVATE ${fltk_BINARY_DIR} ${fltk_SOURCE_DIR})
    target_link_libraries(main PRIVATE fltk) # + fltk_gl fltk_images fltk_png fltk_jpeg fltk_z

    # Google benchmark Fetch
    set(BENCHMARK_ENABLE_TESTING OFF)
    FetchContent_Declare(googlebenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v1.8.4
    )
    FetchContent_MakeAvailable(googlebenchmark)
    add_library(benchmark::benchmark ALIAS benchmark)
    target_link_libraries(MyBenchmark PRIVATE benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
    endforeach()

# Make the dependency library building with VCPKG either in normal mode or manifest mode
elseif (BuildType STREQUAL "VCPKG")
    find_package(FLTK CONFIG REQUIRED )
    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    find_package(benchmark REQUIRED)
    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images sfml-system sfml-window sfml-graphics sfml-network sfml-audio sfml-main benchmark::benchmark)
    target_link_libraries(MyBenchmark PRIVATE benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
    endforeach()
    
# Make the dependency library building with SourceBuild
elseif (BuildType STREQUAL "SOURCEBUILD")
    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/fltk")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/fltk/src)
    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images)

    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/SFML")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/SFML/include)
    target_link_libraries(main PRIVATE sfml-audio sfml-graphics sfml-main sfml-network sfml-system sfml-window)

    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/benchmark")
    target_include_directories(MyBenchmark PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)
    target_link_libraries(MyBenchmark PRIVATE benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
        target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)

    endforeach()
endif ()

# Output benchmark to given path 
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/benchmark")

# Add post-build command to run benchmarks automatically (only once when the rebuilding from scratch) 
add_custom_command(TARGET MyBenchmark
        COMMAND MyBenchmark --benchmark_out=${BENCHMARK_OUT} --benchmark_out_format=${BENCHMARK_OUT_FORMAT}
        COMMAND echo "Automatically running benchmarks after build"
        COMMENT "Automatically running benchmarks after build"
)
#TODO: CREATE A NEW FOLDER (PROBABLY IN SOURCE FOLDER) NAMED BenchmarkResults, every time the txt file needs to be refreshed, first copy to that folder and then refresh yourself