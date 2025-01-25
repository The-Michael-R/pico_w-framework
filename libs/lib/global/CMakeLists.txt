# Give the current Library a name
set(CURR_LIB GlobalLibs)

# Add library specific compile options
add_compile_options(
        )

add_library(${CURR_LIB}
        debug_print.c
        )

target_compile_definitions(${CURR_LIB} PRIVATE
        )


# List all include directories here:
# This way you can include them as #include "lib/libs.h"
target_include_directories(${CURR_LIB} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        ${PROJECT_SOURCE_DIR}/src
        # The following line in required because lwip/opt.h includes the lwipopts.h:
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include/wlan/
        )

# Point the linker to all library entries:
target_link_libraries(${CURR_LIB} PUBLIC
        pico_stdlib
        )

# Append the currend library to the global list anr return it to the callee
list(APPEND LIBRARIES ${CURR_LIB})
return(PROPAGATE LIBRARIES)
