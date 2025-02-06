cmake_minimum_required(VERSION 3.13)

set(ENV{PICO_BOARD} "pico")

## pico_sdk_import.cmake is copied from
## https://github.com/raspberrypi/pico-sdk/blob/master/external/pico_sdk_import.cmake
include(${CMAKE_CURRENT_LIST_DIR}/pico_sdk_import.cmake)

## Give the baby a name. Will also be the name of the resulting binaries
project(rp2040-silentDisco)

## Init the pico-sdk. Will instruct the user what to do if deps are missing
pico_sdk_init()

## We want C++20 and will fail if not supported
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Add our own C/C++ files here
## Sorted alphabetically
add_executable(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/lib/Pico-USB-audio/SDM/src/SDM.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/Pico-USB-audio/SDM/src/pdmRP2040.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

## Needed so tusb_config.h is found by the tinyUSB-library included in the pico-sdk
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/Pico-USB-audio/SDM/src
)

## Add our rp2040-PIO programs here
#pico_generate_pio_header(${CMAKE_PROJECT_NAME}
#    ${CMAKE_CURRENT_LIST_DIR}/lib/tx16.pio
#)

## Print memory usage percentages after linking
string(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage")

## Pull in all pico-sdk + non-pico-sdk libraries
## Sorted alphabetically
target_link_libraries(${CMAKE_PROJECT_NAME}
    hardware_irq
    hardware_pio
    hardware_pwm
    pico_multicore
    pico_stdlib
    pico_time
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

## Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
