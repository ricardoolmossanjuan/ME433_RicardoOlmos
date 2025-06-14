#include "pico/stdlib.h"

#define LED_PIN 25

int main() {
    // Initialize GPIO pin for the LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Blink LED 5 times per second (100ms on, 100ms off)
    const uint led_on_time_ms = 100;
    const uint led_off_time_ms = 100;

    while (true) {
        gpio_put(LED_PIN, true);   // Turn LED ON
        sleep_ms(led_on_time_ms);

        gpio_put(LED_PIN, false);  // Turn LED OFF
        sleep_ms(led_off_time_ms);
    }
}
