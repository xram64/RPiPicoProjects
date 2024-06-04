cmake_minimum_required(VERSION 3.13)

# Set board type (alternative to specifying `-DPICO_BOARD=pico_w` in cmake command)
set(PICO_BOARD pico_w)

# Set WiFi login
#set(WIFI_SSID _ssid_)
#set(WIFI_PASSWORD _password_)

# Initialize the SDK based on PICO_SDK_PATH
# Note: this must happen before project()
include(pico_sdk_import.cmake)

project(motion_notifier_range_sensor) # C CXX ASM)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# Project
add_executable(
    motion_notifier_range_sensor
    motion_notifier_range_sensor.c
)
target_compile_definitions(
    motion_notifier_range_sensor
    PRIVATE WIFI_SSID=\"${WIFI_SSID}\" WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

# Add libraries
target_link_libraries(
    motion_notifier_range_sensor
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

# Includes (using `${CMAKE_CURRENT_LIST_DIR}/..` to find copy of `lwipopts.h` file in top-level folder)
target_include_directories(
    motion_notifier_range_sensor
    PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_LIST_DIR}/../.includes
)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(motion_notifier_range_sensor)