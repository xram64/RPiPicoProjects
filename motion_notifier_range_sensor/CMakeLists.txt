cmake_minimum_required(VERSION 3.13)

# Set board type (alternative to specifying `-DPICO_BOARD=pico_w` in cmake command)
set(PICO_BOARD pico_w)

# Set WiFi login (alternative to specifying `-DWIFI_SSID` and `-DWIFI_PASSWORD` in cmake command)
#set(WIFI_SSID _ssid_)
#set(WIFI_PASSWORD _password_)

# Initialize the SDK based on PICO_SDK_PATH
# Note: this must happen before project()
include(pico_sdk_import.cmake)

project(motion_notifier_range_sensor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# Project
add_executable(motion_notifier_range_sensor
    motion_notifier_range_sensor.c
    tls_common.c
    # tls_verify.c
)

# WiFi SSID/Passphrase definitions - must be passed into `cmake` command to build.
target_compile_definitions(motion_notifier_range_sensor PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
    # By default verification is optional (MBEDTLS_SSL_VERIFY_OPTIONAL)
    ALTCP_MBEDTLS_AUTHMODE=MBEDTLS_SSL_VERIFY_REQUIRED
)

# Includes (using `${CMAKE_CURRENT_LIST_DIR}/../.includes` to find copy of `lwipopts_common.h` file in top-level `.includes` folder)
target_include_directories(motion_notifier_range_sensor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../.includes
)

# Add libraries
target_link_libraries(motion_notifier_range_sensor
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_lwip_mbedtls
    pico_mbedtls
    pico_stdlib
)
# CYW43 Libraries (https://github.com/raspberrypi/pico-sdk/blob/6a7db34ff63345a7badec79ebea3aaef1712f374/src/rp2_common/pico_cyw43_arch/include/pico/cyw43_arch.h#L54):
# pico_cyw43_arch_lwip_poll, pico_cyw43_arch_lwip_threadsafe_background, pico_cyw43_arch_lwip_sys_freertos


# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(motion_notifier_range_sensor)

# Ignore warnings from lwIP code
set_source_files_properties(
    ${PICO_LWIP_PATH}/src/apps/altcp_tls/altcp_tls_mbedtls.c
    PROPERTIES
    COMPILE_OPTIONS "-Wno-unused-result"
)