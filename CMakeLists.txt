# Present option to the user how to build the project: FETCHCONTENT, VCPKG, or SOURCEBUILD
set(BuildType "VCPKG" CACHE STRING "Specify how to build the project: FetchContent, Vcpkg, or SourceBuild")
set(BuildTypeEnum "FetchContent" "Vcpkg" "SourceBuild")

# Convert elements to upper to be not case sensitive 
string(TOUPPER ${BuildType} BuildType)
list(TRANSFORM BuildTypeEnum TOUPPER OUTPUT_VARIABLE UpperBuildTypeEnum)

# Set the toolchain vcpkg file only if not building from source
if (BuildType STREQUAL "VCPKG")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# Initial configurations
cmake_minimum_required(VERSION 3.15)
project(CMakeSFMLProject)
set(CMAKE_CXX_STANDARD 23)

# Set the build type
set(VCPKG_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Debug)

# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
# Include recursively every cpp file along with their paths ending with "ArrayMain.cpp" 
file(GLOB_RECURSE MAIN_FILES "${CMAKE_SOURCE_DIR}/src/**/*main.cpp")

#Handle creating executables for each cpp file ending with "main" 
foreach (MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${MAIN_FILE})
endforeach ()

#IMP Temporarily adding the main executable for the project. Later on I will delete this. I am right know adding this because library linkers expecting a target
add_executable(main src/MainMain.cpp)

# Print the variables
message(STATUS "CMake version is: ${CMAKE_VERSION}")

# For now I am doing everything for windows, later on I will consider doing cross platform. Abort if's not Windows
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    MESSAGE(FATAL_ERROR "Detected OS is not Windows, for now I am doing every stuff in windows")
endif ()

# Print the given or default BuildType
if (NOT BuildType IN_LIST UpperBuildTypeEnum)
    message(FATAL_ERROR "Invalid value for BuildType: ${BuildType}. Allowed values are: ${BuildTypeEnum}")
else ()
    message(STATUS "BuildType is ${BuildType}")
endif ()

#IMP: Make library binding 3 different ways. I only binding for the MainMain.cpp 

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

    # Make the dependency library building with VCPKG either in normal mode or manifest mode
elseif (BuildType STREQUAL "VCPKG")
    find_package(FLTK REQUIRED CONFIG)
    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images sfml-system sfml-window sfml-graphics sfml-network sfml-audio sfml-main)

    # Make the dependency library building with SourceBuild
elseif (BuildType STREQUAL "SOURCEBUILD")
    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/fltk")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/fltk/src)
    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images)

    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/SFML")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/SFML/include)
    target_link_libraries(main PRIVATE sfml-audio sfml-graphics sfml-main sfml-network sfml-system sfml-window)
endif ()
