include(cmake/UserConfigs.cmake)

# Initial configurations
cmake_minimum_required(VERSION 3.15)
project(CMakeSFMLProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set the build type

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug or Release")

include(cmake/CompilerOptions.cmake)

include(cmake/Sanitizers.cmake)

# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Temporarily adding the main executable for the project. Even though this MainMain.cpp is not used for anything, for now SFML only linked to use this target 
# this gave an error in my laptop for no reason. 
#add_executable(main src/MainMain.cpp)

include(cmake/Dependencies.cmake)

include(cmake/PGO.cmake)

#Here is for other subdirectory targets. 
#TODO: Automatically detect all the subdirectories of src/DSA/helper and add them as add_subdirectory 
add_subdirectory(src/DSA/CustomDS/MyVector)
add_subdirectory(src/DSA/CustomDS/MyString)

#VCPKG or cmake I don't know broken again for no reason. It always getting broken for no reason. It always annoying me for no reason. I hate vcpkg and cmake more than everything. 
# If you so much care about these projects (probably not, you just wasted tons of your fucking time last year with all this bullshit cmake), just build their respective projects from source. I don't care about anything. The fucking error will stay as is and I will do nothing about it as if if I waste my time again to fix, somethin will change
#add_subdirectory(src/Projects/ReflectionExample)


#This is for ImportantConcepts 
add_subdirectory(src/ImportantConcepts)
add_project_subdirectories(${CMAKE_SOURCE_DIR}/src/Projects)

message(FLAGS ARE: ${CMAKE_CXX_FLAGS})


