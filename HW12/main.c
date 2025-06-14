#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define LED_PIN PICO_DEFAULT_LED_PIN

// Converts angle (0–180°) to PWM level (duty cycle)
uint16_t angle_to_level(uint16_t angle, uint16_t wrap) {
    // 2.5% → 12.5% duty = 0.5ms to 2.5ms at 20ms period (50Hz)
    float min_duty = 2.5;
    float max_duty = 12.5;
    float duty_percent = min_duty + (angle / 180.0f) * (max_duty - min_duty);
    return (uint16_t)((duty_percent / 100.0f) * wrap);
}

// Toggle onboard LED
void blink_led() {
    static bool state = true;
    gpio_put(LED_PIN, state);
    state = !state;
}

int main() {
    stdio_init_all();
    printf("Starting PWM servo test...\n");

    // Init LED for heartbeat
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Setup servo PWM pin
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

    // Set PWM for 50Hz: 125MHz / 40 / 62500 = 50Hz
    pwm_set_clkdiv(slice, 40.0f);
    uint16_t wrap = 62500;
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);

    while (1) {
        blink_led();

        // Move to 0°
        uint16_t level = angle_to_level(0, wrap);
        pwm_set_gpio_level(SERVO_PIN, level);
        sleep_ms(2000);

        // Move to 180°
        level = angle_to_level(180, wrap);
        pwm_set_gpio_level(SERVO_PIN, level);
        sleep_ms(1500);
    }

    return 0;
}