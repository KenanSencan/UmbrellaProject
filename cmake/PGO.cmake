message("PROFDATA FOUND: ${PROFDATA_FOUND}")
message("INSTR_PROFILE: ${INSTR_PROFILE}")
message(FLAGS ARE: ${CMAKE_CXX_FLAGS})

file(GLOB PROFRAW_FILES ${CMAKE_BINARY_DIR}/bin/*.profraw)
if (NOT ${PROFDATA_FOUND} AND ${INSTR_PROFILE} AND PROFRAW_FILES)

    set(PROFDATA_FOUND ON)

    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE) # Get only the filename, this will be target's and executable's name

        # Step 4: Create a custom command to merge profiling data after profiling has been run
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                COMMAND llvm-profdata merge -output=${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}.profdata ${PROFRAW_FILES}
                COMMENT "Merging all valid profiling data for ${EXECUTABLE_NAME}"
                DEPENDS ${PROFRAW_FILES}
                VERBATIM
        )
    endforeach ()
endif ()


#instead of manually invoking to recompilation after executable executed, this will be automatically execute to generate profraw files
if (${INSTR_PROFILE} AND NOT ${PROFDATA_FOUND})


    #profraw file generation is corrupted if command strings are given automatic. I don't know why but I disabled. For now number of times execution must be manually given like before  
    #    set(SET_PROFRAW_COUNT 5)

    #    math(EXPR SET_PROFRAW_COUNT "${SET_PROFRAW_COUNT} -1") ##This is required because foreach executing 0-5 5th inclusive
    #    set(COMMAND_LIST "")
    #    foreach (i RANGE ${SET_PROFRAW_COUNT})
    #        LIST(APPEND COMMAND_LIST
    #                COMMAND ${CMAKE_COMMAND} -E echo " ${i}. Execution of: ${EXECUTABLE_NAME}"
    #                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
    #                COMMAND ${CMAKE_COMMAND} -E sleep 1)
    #    endforeach ()
    #
    #    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD ${COMMAND_LIST}


    foreach (MAIN_FILE ${MAIN_FILES})
        get_filename_component(EXECUTABLE_NAME ${MAIN_FILE} NAME_WE) # Get only the filename, this will be target's and executable's name
        #        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD ${COMMAND_LIST})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo " 2. Execution of: ${EXECUTABLE_NAME}"
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND ${CMAKE_COMMAND} -E sleep 1

                COMMAND ${CMAKE_COMMAND} -E echo " 2. Execution of: ${EXECUTABLE_NAME}"
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND ${CMAKE_COMMAND} -E sleep 1

                COMMAND ${CMAKE_COMMAND} -E echo " 3. Execution of: ${EXECUTABLE_NAME}"
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND ${CMAKE_COMMAND} -E sleep 1

                COMMAND ${CMAKE_COMMAND} -E echo " 4. Execution of: ${EXECUTABLE_NAME}"
                COMMAND $<TARGET_FILE:${EXECUTABLE_NAME}>
                COMMAND ${CMAKE_COMMAND} -E sleep 1
        )
    endforeach ()
endif ()