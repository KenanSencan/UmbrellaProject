# Library dependency has 3 options  FETCHCONTENT, VCPKG, or SOURCEBUILD. Only these 3 option can be given. 
set(BuildType "SourceBuild" CACHE STRING "Specify how to build the project: FetchContent, Vcpkg, or SourceBuild, by default SourceBuild is given")
set(BuildTypeEnum "FetchContent" "Vcpkg" "SourceBuild")

# Convert elements to upper to be not case sensitive 
string(TOUPPER ${BuildType} BuildType)
list(TRANSFORM BuildTypeEnum TOUPPER OUTPUT_VARIABLE UpperBuildTypeEnum)

# Set the toolchain vcpkg file if build option is given for vcpkg. VCPKG_CMAKE environment variable has to be given to the vcpkg.cmake path. Otherwise fatal error will be given
if (BuildType STREQUAL "VCPKG")
    if (NOT DEFINED ENV{VCPKG_CMAKE})
        message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set. VCPKG_CMAKE should be equal to ..vcpkg/scripts/buildsystems/vcpkg.cmake")
    endif ()
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}")
endif ()

# Initial configurations
cmake_minimum_required(VERSION 3.15)
project(CMakeSFMLProject)
set(CMAKE_CXX_STANDARD 23)

# Set the build type
set(VCPKG_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug or Release")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

# Print the variables
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Library dependency resolution method: ${BuildType}")
message(STATUS "CMake version is: ${CMAKE_VERSION}")

# Option to USE CLANG, MSVC, or G++
set(SPECIFY_CUSTOM_COMPILER_PATHS OFF CACHE STRING "When changing the compilers often, instead of giving user's own compiler paths every time, if necessary environment variables are set it will be
easier to switch the compilers with changing the given cache variables ON or OFF. USE_LINUX_CLANG, USE_G++, USE_MSVC")

SET(USE_LINUX_CLANG FALSE CACHE BOOL "Use Linux's clang")
SET(USE_G++ FALSE CACHE BOOL "Use G++")
SET(USE_MSVC FALSE CACHE BOOL "Use MSVC")

# Increment counter
file(READ "${CMAKE_SOURCE_DIR}/Counter.txt" COUNTER)
MATH(EXPR COUNTER "${COUNTER}+1")
file(WRITE "${CMAKE_SOURCE_DIR}/Counter.txt" "${COUNTER}")

if (SPECIFY_CUSTOM_COMPILER_PATHS)
    if (USE_LINUX_CLANG)
        #If given env variables are defined use them 
        if (DEFINED ENV{CLANG++_PATH} AND DEFINED ENV{CLANG_PATH})
            SET(CMAKE_C_COMPILER $ENV{CLANG_PATH})
            SET(CMAKE_CXX_COMPILER $ENV{CLANG++_PATH})
        else ()
            #If not then use the default path in linux
            message(WARNING "Attempted to use Linux's clang, but the CLANG++_PATH and CLANG_PATH environment variables are not set. 
            The default installation path from the usr/bin/clang++ directory will be assumed, but there is no guarantee it will work.")
            SET(CMAKE_C_COMPILER "/usr/bin/clang")
            SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
        endif ()
    endif ()
    if (USE_G++)
        #If given env variables are defined use them 
        if (DEFINED ENV{G++} AND DEFINED ENV{GCC})
            SET(CMAKE_C_COMPILER $ENV{G++})
            SET(CMAKE_CXX_COMPILER $ENV{GCC})
        else ()
            #If not then use the default path in linux
            message(WARNING "Attempted to use g++, but the G++ and GCC environment variables are not set. 
            The default installation path from the /usr/bin/g++ directory will be assumed, but there is no guarantee it will work.")
            SET(CMAKE_C_COMPILER "/usr/bin/gcc")
            SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
        endif ()
    endif ()
    # Option to set USE MSVC
    if (USE_MSVC)
        #If given env variables are defined use them 
        if (DEFINED ENV{CL_PATH})
            SET(CMAKE_C_COMPILER $ENV{CL_PATH})
            SET(CMAKE_CXX_COMPILER $ENV{CL_PATH})
        else ()
            #If not set as env variable then try to get from the default path 
            message(WARNING "Attempted to use MSVC, but the CL_PATH environment variable is not set. MSVC will be searched for automatically, but there is no guarantee it will work.")
            FIND_MSVC_COMPILER()
            SET(CMAKE_C_COMPILER "${MSVC_PATH}")
            SET(CMAKE_CXX_COMPILER "${MSVC_PATH}")
        endif ()
    endif ()

    #Function to find the visual studio compiler path regardless of different version names. The found output path should look like 
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
endif ()

#Compiler flags 
set(OFF CACHE STRING "Turn on or off instrumented profiling ONLY FOR CLANG. Off by default")
set(INSTR_PROFILE ON)
set(PROFDATA_FOUND OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

#Delete here soon
#if (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fprofile-instr-use=${CMAKE_CURRENT_BINARY_DIR}/bin/code.profdata")
#endif ()

#Step 1: Start instr-generate if desired. After the building configuration is done, below extra profraws will be generated, they will be merged and then the target target will be recompiled with new profile data 
#TODO: This one works instead of the above one. This is extremely important to learn when space involved in path. STUDY EXTREMELY CLOSELY THIS WITH ABOVE COMMEND
set(Hello ON)
if (${CMAKE_CXX_COMPILER_ID} MATCHES Clang AND INSTR_PROFILE)

    if (EXISTS ${CMAKE_BINARY_DIR}/TargetName)
        # Check if profiling data exists and set flags accordingly
        file(READ ${CMAKE_BINARY_DIR}/TargetName TARGETNAME)
        set(PROFDATA_FILE "${CMAKE_BINARY_DIR}/${TARGETNAME}.profdata")
        set(PROFDATA_FOUND ON)
        message("Profdata file is found. PCO will be attempted")
    endif ()

    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fprofile-instr-use=\"${CMAKE_CURRENT_BINARY_DIR}/bin/code.profdata\"")
    if (EXISTS ${PROFDATA_FILE})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=profile-instr-unprofiled -O2 -fprofile-instr-use=\"${PROFDATA_FILE}\"")
        message("Profdata is applied. From now on it will build using profdata")
    else () #If PROFDATA not found, generate 
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=profile-instr-unprofiled -O2 -fprofile-instr-generate=" code-%p.profraw"")
        message("Profile instruction is generated")
    endif ()
endif ()

# Option to enable thread sanitizer inspection
option(INSPECT_THREAD "Enable ThreadSanitizer and disable AddressSanitizer" OFF)

# Enable different sanitizers based on the compiler
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
set(BENCHMARK_ENABLE_TESTING OFF)


# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# MSVC doesn't care about case sensitiveness in here, but linux cares. Before attempting to find files, lowercase and uppercase files with "main" "Main" suffixes will be appended 
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE LOWERCASE_MAIN_FILES "${CMAKE_SOURCE_DIR}/src/*main.cpp") #Lowercase ending main
file(GLOB_RECURSE UPPERCASE_MAIN_FILES "${CMAKE_SOURCE_DIR}/src/*Main.cpp") #Uppercase ending Main
list(APPEND MAIN_FILES ${LOWERCASE_MAIN_FILES} ${UPPERCASE_MAIN_FILES})

# Remove duplicates, if any
list(REMOVE_DUPLICATES MAIN_FILES)

# Print out the found main files
message(STATUS "Found standalone executable files are:")
foreach (MAIN_FILE ${MAIN_FILES})
    message(STATUS " - ${MAIN_FILE}")
endforeach ()

# Check if MAIN_FILES is empty
if (NOT MAIN_FILES)
    message(FATAL_ERROR "No file with main suffix files found.")
endif ()

#Prepare to get include directories
file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/**/*") #Get the path of all the cpp files in src dir
set(ALL_INCLUDE_DIRS "")
foreach (MAIN_FILE ${ALL_CPP_FILES})
    get_filename_component(DIR ${MAIN_FILE} DIRECTORY) #Get the path of the current CPP file 
    list(APPEND ALL_INCLUDE_DIRS ${DIR})
endforeach ()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

# Handle creating executables for each cpp file ending with "main" 
foreach (MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE) #Get only the filename, this will be target's and executable's name 
    message(STATUS "Executable: ${EXECUTABLE_NAME} created")
    add_executable(${EXECUTABLE_NAME} ${MAIN_FILE})

    #I am not sure this is right way to handle inclusion. There's custom mimic for STL. Standalone executables often using these but I am not really sure how the performance would be affected when the includes get bigger.
    #TODO:  I need to later on find better resolution for here 
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ALL_INCLUDE_DIRS})
endforeach ()

# Temporarily adding the main executable for the project. Even though this MainMain.cpp is not used for anything, for now SFML only linked to use this target 
add_executable(main src/MainMain.cpp)


# For now I am doing everything for Windows, later on I will consider doing cross-platform for vcpkg. Though it supposed to work cross platform 
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" AND BuildType STREQUAL "VCPKG")
    message(WARNING "Detected OS is not Windows, for now don't use vcpkg without windows. At least I didn't try ")
endif ()

# Give error if another value given outside of BuildTypeEnum variable's values 
if (NOT BuildType IN_LIST UpperBuildTypeEnum)
    message(FATAL_ERROR "Invalid value for BuildType: ${BuildType}. Allowed values are: ${BuildTypeEnum}")
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

    # FLTK Fetch. (FLTK has been removed from the project) 
    #    set(FLTK_COMMIT "branch-1.3" CACHE STRING "FLTK branch name, tag, or commit")
    #    FetchContent_Declare(FLTK
    #            GIT_REPOSITORY https://github.com/fltk/fltk
    #            GIT_TAG "${FLTK_COMMIT}"
    #    )
    #    FetchContent_MakeAvailable(FLTK)
    #    target_include_directories(main PRIVATE ${fltk_BINARY_DIR} ${fltk_SOURCE_DIR})
    #    target_link_libraries(main PRIVATE fltk) # + fltk_gl fltk_images fltk_png fltk_jpeg fltk_z

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
    endforeach ()

    # Make the dependency library building with VCPKG either in normal mode or manifest mode
elseif (BuildType STREQUAL "VCPKG")
    #    find_package(FLTK CONFIG REQUIRED )
    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    find_package(benchmark REQUIRED)
    target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio sfml-main benchmark::benchmark)

    #FLTK has been removed from the project  
    #    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images sfml-system sfml-window sfml-graphics sfml-network sfml-audio sfml-main benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
    endforeach ()

    # Make the dependency library building with SourceBuild
elseif (BuildType STREQUAL "SOURCEBUILD")
    # FLTK has been removed from the project 
    #    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/fltk")
    #    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/fltk/src)
    #    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images)

    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/SFML")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/SFML/include)
    target_link_libraries(main PRIVATE sfml-audio sfml-graphics sfml-main sfml-network sfml-system sfml-window)

    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/benchmark")

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
        target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)

    endforeach ()
endif ()

#TODO: IF INSTR_PROFILE is given, I should with custom command, call it 10 times (change this number with cmake variable) with this "LLVM_PROFILE_FILE="code-%p.profraw" ./code" command 
#TODO: Then make the merge with llvm-profdata merge -output=code.profdata code-*.profraw
#TODO: Build it again with clang++ -O2 -fprofile-instr-use=code.profdata code.cc -o code
if (NOT ${PROFDATA_FOUND})
    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE) # Get only the filename, this will be target's and executable's name 

        # Step 1: Create a custom command to run profiling
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Running ${EXECUTABLE_NAME} 4 times with different profile files"
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMENT "Running ${EXECUTABLE_NAME} executable 4 times for profiling"
                VERBATIM
        )

        # Step 2: Create a custom command to merge profiling data after profiling has been run
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                COMMAND llvm-profdata merge -output=${EXECUTABLE_NAME}.profdata ";code-*.profraw"
                COMMENT "Merging all the profiling data for ${EXECUTABLE_NAME}"
                VERBATIM
        )

        #Set Profdata to be on because it generated at above command 
        set(PROFDATA_FOUND ON)

        #When rebuilding with profdata again, .profdata file's name needs to be known. Create a file with the executable name for later use
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo_append ${EXECUTABLE_NAME} > "${CMAKE_BINARY_DIR}/TargetName"
                COMMENT "Creating a file with the target executable name"
                VERBATIM
        )

        # Condition in CMakeLists.txt for recompiling with profile data
        add_custom_target(run_cmake_twice ALL
                COMMAND ${CMAKE_COMMAND} -E env
                CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
                CMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}

                COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
        )

        add_dependencies(run_cmake_twice ${EXECUTABLE_NAME})
    endforeach ()
endif ()

#Here is for other subdirectory targets.
add_subdirectory(src/DSA/helper/MyVector)

#This is for ImportantConcepts 
add_subdirectory(src/ImportantConcepts)

# Create a custom target to run CMake again
#if (${PROFDATA_FOUND})
#    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
#            COMMAND cmake CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} CMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
#            CMAKE_C_COMPILER=${CMAKE_C_COMPILER} CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -G "${CMAKE_GENERATOR}"
#            -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR})
#endif ()


