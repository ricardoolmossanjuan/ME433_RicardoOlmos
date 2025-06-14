#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// MCP23008 I2C address (A2-A0 = 0)
#define MCP23008_ADDR 0x20

// MCP23008 register addresses
#define IODIR_REG 0x00
#define GPIO_REG  0x09
#define OLAT_REG  0x0A

// Pico onboard LED
#define HEARTBEAT_LED 25

// === I2C write to a register ===
void write_i2c_reg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(i2c_default, MCP23008_ADDR, buf, 2, false);
}

// === I2C read from a register ===
uint8_t read_i2c_reg(uint8_t reg) {
    uint8_t val = 0;
    i2c_write_blocking(i2c_default, MCP23008_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MCP23008_ADDR, &val, 1, false);
    return val;
}

// === Setup chip ===
void setup_expander() {
    // GP7 = output, GP0–GP6 = input
    write_i2c_reg(IODIR_REG, 0b01111111);
}

// === Control LED on GP7 ===
void set_expander_led(uint8_t on) {
    uint8_t val = on ? 0b10000000 : 0x00;
    write_i2c_reg(OLAT_REG, val);
}

// === Read button state from GP0 ===
uint8_t button_pressed() {
    uint8_t input = read_i2c_reg(GPIO_REG);
    return (input & 0b00000001) ? 1 : 0;
}

int main() {
    stdio_init_all();

    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);  // 100 kHz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // Pico onboard LED as heartbeat
    gpio_init(HEARTBEAT_LED);
    gpio_set_dir(HEARTBEAT_LED, GPIO_OUT);

    // Initialize expander
    setup_expander();

    while (1) {
        // Button logic
        if (button_pressed()) {
            set_expander_led(0);  // button is pressed → turn off GP7
        } else {
            set_expander_led(1);  // button is not pressed → turn on GP7
        }

        // Heartbeat LED toggle
        gpio_put(HEARTBEAT_LED, 1);
        sleep_ms(100);
        gpio_put(HEARTBEAT_LED, 0);
        sleep_ms(100);
    }

    return 0;
}