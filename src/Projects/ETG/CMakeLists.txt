cmake_minimum_required(VERSION 3.28)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}" CACHE STRING "Vcpkg toolchain file")
project(ETG LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Specify only source files
add_executable(ETG main.cpp
        Managers/DebugTexts.h
        UI/UserInterface.cpp
        UI/UserInterface.h
        Managers/Globals.cpp
        Animation/AnimationManager.h
        Animation/Animation.cpp
        Animation/Animation.h
        Managers/GameManager.cpp)

# Easiest and fastest way is VCPKG 
find_package(OpenAL CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED system window graphics network audio)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(ETG PRIVATE
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        OpenAL::OpenAL
)
