set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
set(VCPKG_BUILD_TYPE Release)

# Print the variables
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Library dependency resolution method: ${BuildType}")
message(STATUS "CMake version is: ${CMAKE_VERSION}")
message(STATUS "BINARY DIR: ${CMAKE_BINARY_DIR}")

set(HAVE_STD_REGEX ON)

set(RUN_HAVE_STD_REGEX 1)


# Option to USE CLANG, MSVC, or G++
set(SPECIFY_CUSTOM_COMPILER_PATHS OFF CACHE STRING "When changing the compilers often, instead of giving user's own compiler paths every time, if necessary environment variables are set it will be
easier to switch the compilers with changing the given cache variables ON or OFF. USE_LINUX_CLANG, USE_G++, USE_MSVC")

SET(USE_LINUX_CLANG FALSE CACHE BOOL "Use Linux's clang")
SET(USE_G++ FALSE CACHE BOOL "Use G++")
SET(USE_MSVC FALSE CACHE BOOL "Use MSVC")

# Disable compiler flag tests
set(CMAKE_REQUIRED_FLAGS "")

# Increment counter. This used to be test to make sure automatically cmake runs expected amount of time repeatedly to apply instrumented profile optimization
#file(READ "${CMAKE_SOURCE_DIR}/Counter.txt" COUNTER)
#MATH(EXPR COUNTER "${COUNTER}+1")
#file(WRITE "${CMAKE_SOURCE_DIR}/Counter.txt" "${COUNTER}")

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


#Remove this shit later on
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wconversion -Wfloat-equal -Wold-style-cast")
endif ()


#Compiler options:
set(INSTR_PROFILE OFF CACHE STRING "Turn on or off instrumented profiling ONLY FOR CLANG. Off by default")
set(PROFDATA_FOUND OFF)
file(GLOB PROFDATA_FILE "${CMAKE_BINARY_DIR}/bin/*.profdata")

#Step 1: Start instr-generate if desired. After the building configuration is done, below extra profraws will be generated, they will be merged and then the target will be recompiled with new profile data 
if (${CMAKE_CXX_COMPILER_ID} MATCHES Clang AND INSTR_PROFILE)

    #EXTREMELY IMPORTANT to make sure if a file added in bin folder, force cmake to recompile. It will be fast since nothing changed and cmake incremental building
    file(GLOB_RECURSE MYFILE LIST_DIRECTORIES true CONFIGURE_DEPENDS ${CMAKE_BINARY_DIR}/bin/*)

    if (EXISTS ${PROFDATA_FILE})
        # Check if profiling data exists and set flags accordingly
        set(PROFDATA_FOUND ON)
        message("Profdata file is found. PCO will be attempted")
    endif ()

    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fprofile-instr-use=\"${CMAKE_CURRENT_BINARY_DIR}/bin/code.profdata\"")
    if (EXISTS ${PROFDATA_FILE})

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=profile-instr-unprofiled -O2 -fprofile-instr-use=\"${PROFDATA_FILE}\"")

        message("Profdata is applied. From now on it will build using profdata")

    else () #If PROFDATA not found, generate 

        # I changed here here's the most important part to take a look up later on

        set(PROFILE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
        add_compile_options(-Wno-error=profile-instr-unprofiled
                -O2
                "-fprofile-instr-generate=${PROFILE_OUTPUT_PATH}/code-%p.profraw")
        message("Profile instruction is generated")

    endif ()
endif ()

# Set benchmark configurations
set(BENCHMARK_FORMAT "console")
set(BENCHMARK_OUT_FORMAT "console")
set(BENCHMARK_OUT "${CMAKE_BINARY_DIR}/benchmark/hello.txt")
set(BENCHMARK_ENABLE_TESTING OFF)

# Necessary compiler options given here 

# By default in clang-cl, exceptions disabled. Enable it. 
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif ()