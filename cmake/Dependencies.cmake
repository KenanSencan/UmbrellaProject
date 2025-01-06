# Windows and macOS doesn't care about case sensitiveness for globbing in here, but linux cares. Before attempting to find files, lowercase and uppercase files with "main" "Main" suffixes will be appended 
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
    target_compile_options(${EXECUTABLE_NAME} PRIVATE ${ADD_ALL_WARNINGS}) #Enable all possible warnings

    #I am not sure this is right way to handle inclusion. There's custom mimic for STL. Standalone executables often using these but I am not really sure how the performance would be affected when the includes get bigger.
    #TODO:  I need to later on find better resolution for here 
    target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ALL_INCLUDE_DIRS})
    
    #IF custom STL desired, include and link.
    if (${CUSTOM_STD} STREQUAL "ON")
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE c++ c++abi unwind)
    endif ()
endforeach ()


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

    #After coming here after a long while, I don't know why specifically for fetch content, I only linking graphics but I had better leave it as is.  
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
    find_package(OpenAL CONFIG REQUIRED)
    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    find_package(benchmark CONFIG REQUIRED)
    find_package(Threads REQUIRED)
    
    target_link_libraries(main PRIVATE
            sfml-audio
            sfml-graphics
            $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
            sfml-network
            sfml-system
            sfml-window
            benchmark::benchmark 
            OpenAL::OpenAL
    )
    #FLTK has been removed from the project  
    #    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images sfml-system sfml-window sfml-graphics sfml-network sfml-audio sfml-main benchmark::benchmark)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main Threads::Threads) 
    endforeach ()

    # Make the dependency library building with SourceBuild
elseif (BuildType STREQUAL "SOURCEBUILD")
    # FLTK has been removed from the project 
    #    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/fltk")
    #    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/fltk/src)
    #    target_link_libraries(main PRIVATE fltk fltk_gl fltk_forms fltk_images)

    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/SFML")
    target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/dep/SFML/include)
    target_link_libraries(main PRIVATE
            sfml-audio
            sfml-graphics
            $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
            sfml-network
            sfml-system
            sfml-window
    )
#    "$<IF:
    #$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,18.1.7>, # Condition: Check if compiler version is >= 18.1.7
    #VERSION_GREATER,                                         # True value: Define VERSION_GREATER
    #VERSION_NOT_GREATER                                       # False value: Define VERSION_NOT_GREATER
    #>")
    #"$<IF:$<CXX_COMPILER_ID:MSVC>,MSVC_DEFINED,MSVC_NOT_DEFINED>"
    
    #$<PLATFORM_ID:Windows> #1

    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory("${CMAKE_SOURCE_DIR}/dep/benchmark")

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE benchmark::benchmark)
        target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/dep/benchmark/include)

    endforeach ()
endif ()