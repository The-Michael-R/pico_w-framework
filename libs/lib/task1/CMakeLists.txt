# Give the current Library a name
set(CURR_LIB Task1)

# Add library specific compile options
add_compile_options(
        )

add_library(${CURR_LIB}
        task1.c
        )

# List all include directories here:
# This way you can include them as #include "lib1/lib1.h"
target_include_directories(${CURR_LIB} PUBLIC
        ${PROJECT_SOURCE_DIR}/libs/include
        ${PROJECT_SOURCE_DIR}/src
        )

# Point the linker to all library entries:
target_link_libraries(${CURR_LIB} PUBLIC
        pico_stdlib
        pico_aon_timer
        FreeRTOS-Kernel-Heap4
        )

# Append the currend library to the global list anr return it to the callee
list(APPEND LIBRARIES ${CURR_LIB})
return(PROPAGATE LIBRARIES)
