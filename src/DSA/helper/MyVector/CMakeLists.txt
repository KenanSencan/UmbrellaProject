cmake_minimum_required(VERSION 3.28)
project(MyVectorImplementation)
set(CMAKE_CXX_STANDARD 17)

add_executable(VectorMain VectorExecutable.cpp MyVector.h MyVector.cpp)
