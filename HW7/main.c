#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "ssd1306.h"
#include "uart.h"  // Custom UART handler

#define LED_PIN 25

// Helper function to blink an LED
void toggle_led(uint pin) {
    gpio_put(pin, !gpio_get(pin));
}

int main() {
    stdio_init_all();

    // Initialize I2C pins and bus
    i2c_init(i2c_default, 100000);  // 100 kHz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Setup OLED screen
    ssd1306_setup();

    // Configure onboard LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // UART INIT
    uart_init(UART_ID, 2400);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    // Set up UART RX interrupt
    int irq = (UART_ID == uart0) ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(irq, on_uart_rx);
    irq_set_enabled(irq, true);
    uart_set_irq_enables(UART_ID, true, false);

    uart_puts(UART_ID, "UART Comm Initialized\r\n");

    while (1) {
        toggle_led(LED_PIN);
        sleep_ms(250);
    }
}