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

// #include <stdio.h>
// #include "lwip/apps/http_client.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define TLS_CLIENT_SERVER       "en.wikipedia.org"
#define TLS_CLIENT_HTTP_REQUEST "GET /wiki/Main_Page HTTP/1.1\r\n" \
                                "Host: " TLS_CLIENT_SERVER "\r\n" \
                                "Connection: close\r\n" \
                                "\r\n"
#define TLS_CLIENT_TIMEOUT_SECS 15

// Root certificate for verifying TLS connections (ISRG Root X1)
#define TLS_ROOT_CERT "-----BEGIN CERTIFICATE-----\n\
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n\
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n\
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n\
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n\
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n\
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n\
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n\
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n\
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n\
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n\
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n\
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n\
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n\
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n\
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n\
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n\
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n\
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n\
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n\
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n\
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n\
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n\
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n\
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n\
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n\
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n\
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n\
-----END CERTIFICATE-----\n"

extern bool run_tls_client_test(const uint8_t *cert, size_t cert_len, const char *server, const char *request, int timeout);


const uint LED_PIN_SUCCESS = 0;  // GP0 (Pin 1) : Green
const uint LED_PIN_WARNING = 1;  // GP1 (Pin 2) : Yellow
const uint LED_PIN_FAILURE = 2;  // GP2 (Pin 4) : Red
const uint PINS[] = {LED_PIN_SUCCESS, LED_PIN_WARNING, LED_PIN_FAILURE};

const uint LED_DUR_LONG  = 500;
const uint LED_DUR_MED   = 250;
const uint LED_DUR_SHORT = 100;

// Initialize each LED pin in the given array.
void init_leds(const uint *pins, const size_t pins_len, bool blink_on_init) {
    for (uint i = 0; i < pins_len; i++) {
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
// Flash the LED on the given pin for the given on/off cycle length.
void flash_led(uint pin, uint on_dur, uint off_dur) {

    for (uint i = 0; i < 10; i++) {
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


int main() {
    stdio_init_all();

    // Initialize LEDs
    const size_t pins_len = sizeof(PINS) / sizeof(PINS[0]);
    init_leds(PINS, pins_len, true);

    // Initialize WiFi driver
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        flash_led(LED_PIN_FAILURE, LED_DUR_LONG, LED_DUR_MED);
        printf("Failed to initialize...\n");
        return 1;
    }
    printf("Initialized!\n");

    // Put WiFi interface in STA mode
    cyw43_arch_enable_sta_mode();

    // Attempt WiFi connection
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000)) {
        flash_led(LED_PIN_FAILURE, LED_DUR_MED, LED_DUR_MED);
        printf("Failed to connect...\n");
        return 1;
    }
    printf("Connected!\n");


    /////////////////////////////////////////////////////////
    // DEBUG: Thinking...
    flash_led(LED_PIN_WARNING, LED_DUR_SHORT, LED_DUR_SHORT);


    // Connect to host, using root certificate to verify TLS connection
    const uint8_t root_cert[] = TLS_ROOT_CERT;
    bool pass = run_tls_client_test(root_cert, sizeof(root_cert), TLS_CLIENT_SERVER, TLS_CLIENT_HTTP_REQUEST, TLS_CLIENT_TIMEOUT_SECS);
    if (pass) {
        flash_led(LED_PIN_SUCCESS, LED_DUR_MED, LED_DUR_SHORT);
    } else {
        hold_led(LED_PIN_WARNING);
        flash_led(LED_PIN_FAILURE, LED_DUR_MED, LED_DUR_MED);
    }

    /* sleep a bit to let usb stdio write out any buffer to host */
    sleep_ms(100);

    cyw43_arch_deinit();
    printf("All done\n");
    hold_led(LED_PIN_SUCCESS);
    hold_led(LED_PIN_FAILURE);

    return pass ? 0 : 1;
}