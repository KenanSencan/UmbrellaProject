#TODO: Later on I also want to execute all the benchmarks with given BM suffix end of the file name. Also a way to execute arbitrary BM would not be bad
# Present option to the user how to build the project: FETCHCONTENT, VCPKG, or SOURCEBUILD
set(BuildType "SourceBuild" CACHE STRING "Specify how to build the project: FetchContent, Vcpkg, or SourceBuild")
set(BuildTypeEnum "FetchContent" "Vcpkg" "SourceBuild")

# Convert elements to upper to be not case sensitive 
string(TOUPPER ${BuildType} BuildType)
list(TRANSFORM BuildTypeEnum TOUPPER OUTPUT_VARIABLE UpperBuildTypeEnum)
 
# Set the toolchain vcpkg file only if not building from source
if (BuildType STREQUAL "VCPKG")
    if (NOT DEFINED ENV{VCPKG_CMAKE})
        message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set. VCPKG_CMAKE should be equal to ..vcpkg/scripts/buildsystems/vcpkg.cmake" )
    endif ()
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}")
#    set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
endif ()

# Initial configurations
cmake_minimum_required(VERSION 3.15)
project(CMakeSFMLProject)
set(CMAKE_CXX_STANDARD 23)

# Set the build type
set(VCPKG_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug or Release")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Enable different sanitizers for debug builds
# Option to set USE CLANG in Linux
SET(USE_LINUX_CLANG FALSE CACHE BOOL "Use Linux's clang")
if (USE_LINUX_CLANG)
    #If given env variables are defined use them 
    if (DEFINED ENV{CLANG++_PATH} AND DEFINED ENV{CLANG_PATH})
        SET(CMAKE_C_COMPILER $ENV{CLANG_PATH})
        SET(CMAKE_CXX_COMPILER $ENV{CLANG++_PATH})
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    else ()
        #If not then use the default path in linux
        SET(CMAKE_C_COMPILER "/usr/bin/clang")
        SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    endif ()
endif ()
SET(USE_G++ FALSE CACHE BOOL "Use G++")
if (USE_G++)
    #If given env variables are defined use them 
    if (DEFINED ENV{G++} AND DEFINED ENV{GCC})
        SET(CMAKE_C_COMPILER $ENV{G++})
        SET(CMAKE_CXX_COMPILER $ENV{GCC})
    else ()
        #If not then use the default path in linux
        SET(CMAKE_C_COMPILER "/usr/bin/gcc")
        SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
    endif ()
endif ()

#Function to find the visual studio compiler path. The output path should look like 
#"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe"
function(FIND_MSVC_COMPILER)
    #This path is universal however after from here, the version number folder will differ. So I have to find it manually 
    set(VS_ROOT "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC")
    file(GLOB MSVC_VERSIONS "${VS_ROOT}/*")
    list(SORT "${MSVC_VERSIONS}")
    foreach (VERSION_PATH ${MSVC_VERSIONS})
        message(STATUS "Found MSVC Version Directory: ${VERSION_PATH}")
        if (EXISTS "${VERSION_PATH}/bin/Hostx64/x64/cl.exe")
            set(MSVC_PATH "${VERSION_PATH}/bin/Hostx64/x64/cl.exe" PARENT_SCOPE)
            break()
        endif ()
    endforeach ()
endfunction()

# Option to set USE MSVC
SET(USE_MSVC FALSE CACHE BOOL "Use MSVC")
if (USE_MSVC)
    #If given env variables are defined use them 
    if (DEFINED ENV{CL_PATH})
        SET(CMAKE_C_COMPILER $ENV{CL_PATH})
        SET(CMAKE_CXX_COMPILER $ENV{CL_PATH})
    else ()
        #If not set as env variable then try to get from the default path 
        FIND_MSVC_COMPILER()
        SET(CMAKE_C_COMPILER "${MSVC_PATH}")
        SET(CMAKE_CXX_COMPILER "${MSVC_PATH}")
    endif ()
endif ()

# Option to enable thread sanitizer inspection
option(INSPECT_THREAD "Enable ThreadSanitizer and disable AddressSanitizer" OFF)

# Enable different sanitizers based on build type
MESSAGE(STATUS "CURRENT COMPILER IS: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(SANITIZERS "address,undefined,leak")
    if (INSPECT_THREAD)
        set(SANITIZERS "thread,undefined")
    endif ()
endif ()
# Apply sanitizers
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZERS} -fno-omit-frame-pointer -g")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=${SANITIZERS}")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZERS} -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=${SANITIZERS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # MSVC sanitizer configuration (e.g., for AddressSanitizer)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address /Zi /MDd /FS")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /fsanitize=address /DEBUG /INCREMENTAL:NO")
    endif ()
endif ()


# Set benchmark configurations
set(BENCHMARK_FORMAT "console")
set(BENCHMARK_OUT_FORMAT "console")
set(BENCHMARK_OUT "${CMAKE_BINARY_DIR}/benchmark/hello.txt")

# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

##MSVC doesn't care case sensitiveness in here, but linux cares
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE LOWERCASE_MAIN_FILES "${CMAKE_SOURCE_DIR}/src/*main.cpp") #Lowercase ending main
file(GLOB_RECURSE UPPERCASE_MAIN_FILES "${CMAKE_SOURCE_DIR}/src/*Main.cpp") #Uppercase ending Main
list(APPEND MAIN_FILES ${LOWERCASE_MAIN_FILES} ${UPPERCASE_MAIN_FILES})

# Remove duplicates, if any
list(REMOVE_DUPLICATES MAIN_FILES)

# Print out the found main files
message(STATUS "Found MAIN_FILES:")
foreach (MAIN_FILE ${MAIN_FILES})
    message(STATUS " - ${MAIN_FILE}")
endforeach ()

# Check if MAIN_FILES is empty
if (NOT MAIN_FILES)
    message(FATAL_ERROR "No main.cpp files found.")
endif ()

#Prepare to get include directories
file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/**/*")
set(ALL_INCLUDE_DIRS "")
foreach (MAIN_FILE ${ALL_CPP_FILES})
    get_filename_component(DIR ${MAIN_FILE} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${DIR})
endforeach ()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

file(GLOB_RECURSE ALL_HELPER_CLASSES "${CMAKE_SOURCE_DIR}/src/Data Structures/helper/*")

# Handle creating executables for each cpp file ending with "main" 
foreach (MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
    message(STATUS "MAIN_FILE: " ${MAIN_FILE})
    add_executable(${EXECUTABLE_NAME} ${MAIN_FILE} ${ALL_HELPER_CLASSES})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ALL_INCLUDE_DIRS}) #I am not sure to whether delete or keep this line
endforeach ()

# Temporarily adding the main executable for the project.
add_executable(main src/MainMain.cpp)

# Print the variables
message(STATUS "CMake version is: ${CMAKE_VERSION}")

# For now I am doing everything for Windows, later on I will consider doing cross-platform for vcpkg. Abort if it's not Windows
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" AND BuildType STREQUAL "VCPKG" )
    message(FATAL_ERROR "Detected OS is not Windows, for now don't use vcpkg without windows. At least I didn't try ")
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
#    target_link_libraries(MyBenchmark PRIVATE benchmark::benchmark) #I don't have any idea what is this doing in here? 

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
    # I don't have any idea what is this doing in here? 
#    target_include_directories(MyBenchmark PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)
#    target_link_libraries(MyBenchmark PRIVATE benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
        target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)

    endforeach()
endif ()

# Output benchmark to given path 
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/benchmark")

# Add post-build command to run benchmarks automatically (only once when the rebuilding from scratch) 
# Since I deleted MyBenchmark target, I temporarily disabled here
#add_custom_command(TARGET MyBenchmark
#        COMMAND MyBenchmark --benchmark_out=${BENCHMARK_OUT} --benchmark_out_format=${BENCHMARK_OUT_FORMAT}
#        COMMAND echo "Automatically running benchmarks after build"
#        COMMENT "Automatically running benchmarks after build"
#)

#Here is for other subdirectory targets 
add_subdirectory(src/DSA/helper/MyVector)
