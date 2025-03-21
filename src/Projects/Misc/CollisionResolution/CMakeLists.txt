cmake_minimum_required(VERSION 3.28)
if (NOT DEFINED ENV{VCPKG_CMAKE})
    message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set.")
endif ()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}" CACHE STRING "Vcpkg toolchain file")
project(CollisionResolution LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Specify only source files
add_executable(CollisionResolution CollisionResolutionMain.cpp)

# Easiest and fastest way is VCPKG 
find_package(OpenAL REQUIRED)
find_package(SFML CONFIG REQUIRED system window graphics network audio)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif ()

target_compile_definitions(CollisionResolution PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}")


target_compile_options(CollisionResolution PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
        $<$<CXX_COMPILER_ID:Clang>:-ftemplate-backtrace-limit=0 -ferror-limit=0>
)

target_link_libraries(CollisionResolution PRIVATE
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        OpenAL::OpenAL
)
