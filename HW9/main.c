#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_INST uart0
#define BAUD 115200
#define TX_PIN 0
#define RX_PIN 1

#define MAX_LEN 100
char uart_buffer[MAX_LEN];
volatile int buf_index = 0;

void handle_uart_rx();

int main() {
    stdio_init_all();

    // Init UART at low baud first
    uart_init(UART_INST, 2400);

    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Set final baud rate
    uart_set_baudrate(UART_INST, BAUD);

    // No flow control, 8N1 format
    uart_set_hw_flow(UART_INST, false, false);
    uart_set_format(UART_INST, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_INST, false);

    // Set interrupt handler
    int irq = (UART_INST == uart0) ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(irq, handle_uart_rx);
    irq_set_enabled(irq, true);
    uart_set_irq_enables(UART_INST, true, false);

    // Wait for USB connection
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("UART Communication Initialized\n");

    while (1) {
        int val_from_pc;
        scanf("%d", &val_from_pc);
        printf("Received from PC: %d\n", val_from_pc);

        char msg[32];
        sprintf(msg, "%d\n", val_from_pc);
        uart_puts(UART_INST, msg);

        sleep_ms(250);
    }

    return 0;
}

void handle_uart_rx() {
    while (uart_is_readable(UART_INST)) {
        char ch = uart_getc(UART_INST);
        if (ch == '\n') {
            uart_buffer[buf_index] = '\0';
            printf("Received from Zero: %s\n", uart_buffer);
            buf_index = 0;
        } else if (buf_index < MAX_LEN - 1) {
            uart_buffer[buf_index++] = ch;
        }
    }
}