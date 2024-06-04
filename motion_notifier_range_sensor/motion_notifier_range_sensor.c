/*
    Motion Notifier using Range Sensor
    xram | 6/4/24

    Monitors for motion using a HC-SR04 ultrasonic range finder and
      sends a Gotify notification via REST API call (over WiFi).

    [References]
    - https://www.digikey.com/htmldatasheets/production/1979760/0/0/1/hc-sr04.html
    - https://docs.google.com/document/d/1Y-yZnNhMYy7rwhAgyL_pfa39RsB-x2qR4vP8saG73rE/edit
    - https://www.makerguides.com/hc-sr04-arduino-tutorial/
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// char ssid[] = "A Network";
// char pass[] = "A Password";

const uint SUCCESS_LED_PIN = 0;  // GP0 (Pin 1)
const uint FAILURE_LED_PIN = 1;  // GP1 (Pin 2)

const uint FLASH_SLOW = 500;
const uint FLASH_FAST = 250;

// Flash the LED on the indicated pin at the indicated rate.
void flash_led(uint pin, uint speed) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);

    for (uint i = 0; i < 10; i++) {
        gpio_put(pin, 1);
        sleep_ms(speed);
        gpio_put(pin, 0);
        sleep_ms(speed);
    }
    return
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        flash_led(FAILURE_LED_PIN, FLASH_SLOW);
        printf("Failed to initialize...\n");
        return 1;
    }
    printf("Initialized!\n");

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000)) {
        flash_led(FAILURE_LED_PIN, FLASH_FAST);
        printf("Failed to connect...\n");
        return 1;
    }

    flash_led(SUCCESS_LED_PIN, FLASH_FAST);
    printf("Connected!\n");

    return 0;
}