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