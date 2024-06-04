cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)  # alternative to specifying `-DPICO_BOARD=pico_w` in cmake command

# Initialize the SDK based on PICO_SDK_PATH
# Note: this must happen before project()
include(pico_sdk_import.cmake)

project(hello_world_led)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# Project
add_executable(
    hello_world_led
    hello_world_led.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(
    hello_world_led
    pico_stdlib
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(hello_world_led)