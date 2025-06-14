cmake_minimum_required(VERSION 3.13)

# Pull in the Pico SDK
include(pico_sdk_import.cmake)

project(HW7_UART_OLED C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Define the executable and source files
add_executable(HW7_UART_OLED
    main.c
    uart.c
    ssd1306.c
    ssd1306.h
    font.h
    uart.h
)

# Link necessary libraries
target_link_libraries(HW7_UART_OLED
    pico_stdlib
    hardware_uart
    hardware_i2c
)

# Enable USB output (for debugging) and disable UART stdout
pico_enable_stdio_usb(HW7_UART_OLED 1)
pico_enable_stdio_uart(HW7_UART_OLED 0)

# Create UF2 and binary file
pico_add_extra_outputs(HW7_UART_OLED)
