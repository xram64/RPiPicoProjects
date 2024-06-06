/*
    Pico LED Indicator Library
    xram | 6/6/24
*/

#include "pico/stdlib.h"

const uint LED_PIN_SUCCESS = 0;  // GP0 (Pin 1) : Green
const uint LED_PIN_WARNING = 1;  // GP1 (Pin 2) : Yellow
const uint LED_PIN_FAILURE = 2;  // GP2 (Pin 4) : Red
const uint PINS[] = {
    LED_PIN_SUCCESS,
    LED_PIN_WARNING,
    LED_PIN_FAILURE
};
const size_t PINS_LEN = sizeof(PINS) / sizeof(PINS[0]);

const uint LED_DUR_LONG  = 500;
const uint LED_DUR_MED   = 250;
const uint LED_DUR_SHORT = 100;

// Initialize each LED pin in the given array
void init_leds(const uint *pins, const size_t pins_len, bool blink_on_init) {
    for (uint i = 0; i < sizeof(pins); i++) {
        uint pin = pins[i];

        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);

        if (blink_on_init) {
            sleep_ms(250);
            gpio_put(pin, 1);
            sleep_ms(250);
            gpio_put(pin, 0);
        }
    }
}

// Flash the LED on the given pin for the givem on/off cycle length.
void flash_led(uint pin, uint on_dur, uint off_dur) {
    for (uint i = 0; i < 20; i++) {
        gpio_put(pin, 1);
        sleep_ms(on_dur);
        gpio_put(pin, 0);
        sleep_ms(off_dur);
    }
}

// Hold the LED on indefinitely.
void hold_led(uint pin) {
    gpio_put(pin, 1);
}