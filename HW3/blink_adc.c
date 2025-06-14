#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Pin definitions
#define USER_BUTTON_PIN 16
#define USER_LED_PIN 25

int main() {
    // Initialize stdio for USB output
    stdio_init_all();

    // Initialize ADC
    adc_init();
    adc_gpio_init(26);  // GPIO 26 -> ADC0
    adc_select_input(0);

    // Initialize LED
    gpio_init(USER_LED_PIN);
    gpio_set_dir(USER_LED_PIN, GPIO_OUT);
    gpio_put(USER_LED_PIN, true);  // LED ON initially

    // Initialize Button
    gpio_init(USER_BUTTON_PIN);
    gpio_set_dir(USER_BUTTON_PIN, GPIO_IN);
    gpio_pull_down(USER_BUTTON_PIN);  // Using pull-down

    // Wait until USB is ready
    while (!stdio_usb_connected()) {
        sleep_ms(200);
    }

    printf("System ready. Press button to start sampling.\n");

    while (true) {
        // Wait for button press
        if (gpio_get(USER_BUTTON_PIN)) {
            // Turn LED OFF when button pressed
            gpio_put(USER_LED_PIN, false);

            int num_samples = 0;
            printf("Enter number of samples to take (1-100): ");
            scanf("%d", &num_samples);

            // Clamp number of samples
            if (num_samples < 1) num_samples = 1;
            if (num_samples > 100) num_samples = 100;

            printf("Collecting %d samples at 20Hz (50 ms interval)...\n", num_samples);

            for (int i = 0; i < num_samples; i++) {
                uint16_t raw_adc = adc_read();
                float voltage = raw_adc * 3.3f / 4095.0f;
                printf("Sample %02d: %.3f V\n", i + 1, voltage);
                sleep_ms(50);  // 20 Hz sampling
            }

            printf("Sampling complete. Waiting for next button press.\n");

            // Turn LED back ON after sampling
            gpio_put(USER_LED_PIN, true);

            // Wait for button release to avoid bouncing / repeat triggers
            while (gpio_get(USER_BUTTON_PIN)) {
                sleep_ms(10);
            }
        }
        sleep_ms(10);  // Poll button at 100 Hz
    }

    return 0;
}