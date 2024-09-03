# This folder ImportantConcepts contains small projects that are vital to remember and I put all the projects in here for believing that I will need to take a look more than couple times 

# For the cpp files in this directory, they all contain main function. They can be built as single executable 
file(GLOB ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/ImportantConcepts/*.cpp")
foreach (MAIN_CPP ${ALL_CPP_FILES})
    get_filename_component(MAIN_CPP_NAME ${MAIN_CPP} NAME_WE)
    message(STATUS "The current cpp is: ${MAIN_CPP}")
    add_executable(${MAIN_CPP_NAME} ${MAIN_CPP})
endforeach ()

# Other concepts I made with extra classes instead of single cpp file 
add_subdirectory(MoveConstrMove)
