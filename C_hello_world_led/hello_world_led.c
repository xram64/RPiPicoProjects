#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/types.h"

int main() {
    const uint LED_PIN = 0;  // GP0 = Pin #1
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("Blinking LED!\n");

    for (uint i = 0; i < 1000; i++)
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }

    return 0;
}
