#ifndef LED_CONTROL_H
#define LED_CONTROL_H  // include guard

// GPIO related includes (assuming you have these in your actual implementation)
#include "pico/stdlib.h"

///~~~~~~~~~~~~~///
/// Definitions ///
///~~~~~~~~~~~~~///
extern const uint LED_PIN_SUCCESS;  ///< GP0 (Pin 1)
extern const uint LED_PIN_WARNING;  ///< GP1 (Pin 2)
extern const uint LED_PIN_FAILURE;  ///< GP2 (Pin 4)
extern const uint PINS[];           ///< An array containing all LED pins
extern const size_t PINS_LEN;       ///< The number of pins in the PINS array.

extern const uint LED_DUR_LONG;   ///< Long hold duration
extern const uint LED_DUR_MED;    ///< Medium hold duration
extern const uint LED_DUR_SHORT;  ///< Short hold duration

///~~~~~~~~~~~///
/// Functions ///
///~~~~~~~~~~~///

/**
 * @brief Initializes each LED pin in the given array.
 *
 * @param pins Array of LED pins to initialize.
 * @param pins_len The number of pins in the array.
 * @param blink_on_init If true, each LED will blink once during initialization.
 */
void init_leds(const uint *pins, const size_t pins_len, bool blink_on_init);

/**
 * @brief Flashes the LED on the given pin for the specified on/off cycle length.
 *
 * @param pin The LED pin to flash on.
 * @param on_dur 'On' duration (in ms) of the flash cycle.
 * @param off_dur 'Off' duration (in ms) of the flash cycle.
 */
void flash_led(uint pin, uint on_dur, uint off_dur);

/**
 * @brief Holds the LED on indefinitely.
 *
 * @param pin The LED pin to hold on.
 */
void hold_led(uint pin);


#endif // LED_CONTROL_H
