#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Configurable parameters
#define TOTAL_SAMPLES 200
#define SINE_CYCLES 2
#define TRIANGLE_CYCLES 1

// SPI helper functions
static inline void select_chip() {
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
    asm volatile("nop \n nop \n nop");
}

static inline void deselect_chip() {
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

// SPI write to MCP4912 DAC
void dac_write(int channel, uint16_t value) {
    uint8_t buf[2];
    uint8_t config = (channel << 7) | (0b111 << 4) | (value >> 6);
    uint8_t data = (value & 0x3F) << 2;
    buf[0] = config;
    buf[1] = data;

    select_chip();
    spi_write_blocking(spi_default, buf, 2);
    deselect_chip();
}

// Generate sine wave samples
void create_sine_wave(uint16_t *buffer, int samples, int cycles) {
    for (int i = 0; i < samples; i++) {
        double angle = (2.0 * M_PI * cycles * i) / samples;
        buffer[i] = (uint16_t)(512.0 * (sin(angle)) + 512.0);
    }
}

// Generate triangle wave samples
void create_triangle_wave(uint16_t *buffer, int samples, int cycles) {
    for (int i = 0; i < samples; i++) {
        double phase = (double)i / samples * cycles;
        double tri_value = fmod(phase, 1.0);
        buffer[i] = (uint16_t)(1023.0 * fabs(2.0 * tri_value - 1.0));
    }
}

int main() {
    stdio_init_all();

    // Initialize SPI
    spi_init(spi_default, 12000);  // Start with 12kHz for testing
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    // Prepare waveform data
    uint16_t sine_wave[TOTAL_SAMPLES];
    uint16_t triangle_wave[TOTAL_SAMPLES];

    create_sine_wave(sine_wave, TOTAL_SAMPLES, SINE_CYCLES);
    create_triangle_wave(triangle_wave, TOTAL_SAMPLES, TRIANGLE_CYCLES);

    // Main loop
    while (true) {
        for (int i = 0; i < TOTAL_SAMPLES; i++) {
            dac_write(0, sine_wave[i]);     // Channel A
            dac_write(1, triangle_wave[i]); // Channel B
            sleep_us(1000);  // 1ms delay to control update rate (~1kHz update rate)
        }
    }

    return 0;
}