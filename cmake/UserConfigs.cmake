# cmake/UserConfigs.cmake

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

#Apply these compiler options only if compiler is clang and OS not windows
if ("${CMAKE_CXX_COMPILER}" MATCHES "clang" AND NOT WIN32)
    message(NOTICE "extra warnings for clang compilers added")
    list(APPEND ADD_ALL_WARNINGS
            -fno-elide-constructors
            -Wshadow
            -Wextra
            -Wpedantic
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-newline-eof
    )
endif()

# Use custom compiled libc++ to be able to step into libc++ implementation if desired. 
SET(CUSTOM_STD OFF CACHE BOOL "If custom STD is desired, enable this. Note that with vcpkg, forcing libc++ will get an error. If you enabled this either build everything from source or use fetchcontent")

if (${CUSTOM_STD} STREQUAL "ON")
    SET(CUSTOM_STD_RELEASE ON CACHE BOOL "IF CUSTOM_STD Desired. Configure this option to use release or debug mode.")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        message(WARNING "Custom STD LOC: /home/selviniah/Documents/llvm-project/build/include/c++/v1")
        set(STD_INCLUDE_DIR "/home/selviniah/Documents/llvm-project/build/include/c++/v1")
        set(STD_LIB_DIR "/home/selviniah/Documents/llvm-project/build/lib")
    else ()
        message(WARNING "Custom STD LOC: /home/selviniah/Documents/llvm-project/_InstallLibcxxDebug/include/c++/v1")
        set(STD_INCLUDE_DIR "/home/selviniah/Documents/llvm-project/_InstallLibcxxDebug/include/c++/v1")
        set(STD_LIB_DIR "/home/selviniah/Documents/llvm-project/_InstallLibcxxDebug/lib")
    endif ()


    add_compile_options(-nostdinc++) # -fno-elide-constructors -Wshadow -Wextra -Wpedantic -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-newline-eof

    link_directories(${STD_LIB_DIR})
    include_directories(${STD_INCLUDE_DIR})
endif ()
