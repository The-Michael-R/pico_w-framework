# stop if cmake version is below 3.0
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

################################################################################
# Project related configuration

#Note: Changing the PICO_BOARD delecte build-cache to prevent build issues!

set(PROJECT_NAME "RP2350_Test")      # Give the global project a name
set(PICO_BOARD "pico2_w")            # Possible values "pico_w" or "pico2_w"
set(CYW43_HOST_NAME  "PicoW")        # W-LAN Hostname

set(PICO_SDK_PATH "../../pico-sdk")  # Location of pico-sdk
set(FREERTOS_KERNEL_PATH "../../FreeRTOS-Kernel")  # Location of FreeRTOS

add_compile_definitions(DEFAULT_DEBUG_LEVEL=4)  # Defines default debug-level
add_compile_definitions(HOST_LOG_PORT=54323)    # Defines the debug UDP-port


################################################################################
# No or little manual changes after this

set(PICOTOOL_FETCH_FROM_GIT_PATH)

# Needed for Pico-W, otherwise #include "pico/cyw43_arch.h" won't be found
# Needs to be before include(pico_sdk_import.cmake)
if(PICO_BOARD STREQUAL "pico_w")
        set(PICO_PLATFORM rp2040)
else()
        set(PICO_PLATFORM rp2350)
endif()

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

# ...and a version
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES C CXX ASM)
configure_file(src/project_conf.h.in project_conf.h)

project(RP2350_W_FeeRTOS C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Just to make your life a bit harer but the code might contain less bugs/erros
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-std=gnu2x>)
add_compile_options(
        -O2
        -Wall
        #-Wnested-externs
        -Wcast-qual
        -Wshadow
        -Wunreachable-code
        -Wlogical-op
        -Wfloat-equal
        -Wdouble-promotion
        #-Wredundant-decls
        -Wold-style-definition
        -Wno-format            # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function   # we have some for the docs that aren't called
        -Wunused
        -Wuninitialized
        #-Werror               # Real daredevels might un-comment this one ;-)
        )



# Add the libraries in the libs sub-directory
add_subdirectory("libs")
MESSAGE (STATUS "Collected libraries: ${LIBRARIES}")

# Now adding the main code
add_executable(${PROJECT_NAME}
        src/main.c
        )

# Enable UART over USB (needs to be set after add_executable)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Point the linker to all library entries:
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${LIBRARIES}
        FreeRTOS-Kernel-Heap4
        )

# This way you can include them as #include "lib1/lib1.h"
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/libs/include
        ${PROJECT_BINARY_DIR}
        )

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# Add documentation target
add_subdirectory("docs")
