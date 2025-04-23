cmake_minimum_required(VERSION 3.28)
project(TimerComp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Specify only source files
add_executable(TimerComp mainTimer.cpp TimerComponent.cpp)