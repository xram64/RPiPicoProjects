cmake_minimum_required(VERSION 3.13)

# Set board type (alternative to specifying `-DPICO_BOARD=pico_w` in cmake command)
set(PICO_BOARD pico_w)

# Set WiFi login
#set(WIFI_SSID _ssid_)
#set(WIFI_PASSWORD _password_)

# Initialize the SDK based on PICO_SDK_PATH
# Note: this must happen before project()
include(pico_sdk_import.cmake)

project(motion_notifier_range_sensor)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# Project
add_executable(
    motion_notifier_range_sensor
    motion_notifier_range_sensor.c
)

# Add libraries
target_link_libraries(
    motion_notifier_range_sensor
    pico_cyw43_arch_lwip_threadsafe_background
    pico_stdlib
)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(motion_notifier_range_sensor)