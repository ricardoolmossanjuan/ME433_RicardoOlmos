#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "ssd1306.h"

#define PIN_LED         25
#define PIN_ADC         26
#define DISPLAY_ADDR    0x3C  // Default I2C addr for SSD1306

// Toggle any GPIO
void toggle_gpio(uint pin) {
    gpio_put(pin, !gpio_get(pin));
}

int main() {
    stdio_init_all();

    // --- I2C Setup ---
    i2c_init(i2c_default, 100000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // --- OLED Init ---
    ssd1306_setup();

    // --- ADC Init ---
    adc_init();
    adc_gpio_init(PIN_ADC);  // Set GPIO to ADC mode
    adc_select_input(0);     // Use ADC channel 0

    // --- LED Init ---
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 0);

    // --- Buffers ---
    char voltage_str[32];
    char fps_str[32];

    while (1) {
        toggle_gpio(PIN_LED);  // blink indicator

        absolute_time_t t_start = get_absolute_time();

        // Read ADC and convert to volts
        uint16_t raw = adc_read();
        float v = (raw * 3.3f) / 4095.0f;

        snprintf(voltage_str, sizeof(voltage_str), "Voltage: %.2f V", v);
        drawMessage(0, 10, voltage_str);

        absolute_time_t t_end = get_absolute_time();
        uint32_t dt_us = absolute_time_diff_us(t_start, t_end);

        float fps = 1000000.0f / dt_us;
        snprintf(fps_str, sizeof(fps_str), "FPS: %.1f", fps);
        drawMessage(0, 20, fps_str);
    }

    return 0;
}