# Option to enable thread sanitizer inspection
option(INSPECT_THREAD "Enable ThreadSanitizer and disable AddressSanitizer" OFF)
option(APPLY_SANITIZERS "Enable or disable to apply sanitizers when in debug mode and using GCC/CLANG" OFF)

# Enable different sanitizers based on the compiler. "leak" removed !!!
MESSAGE(STATUS "CURRENT COMPILER IS: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(SANITIZERS "address,undefined") ##"leak" removed from here later on when I am studying sanitizer in depth I will come back here
    if (INSPECT_THREAD)
        set(SANITIZERS "thread,undefined")
    endif ()
endif ()


# Apply sanitizers
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND APPLY_SANITIZERS)
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