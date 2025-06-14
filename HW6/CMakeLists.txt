cmake_minimum_required(VERSION 3.13)

# Import Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Project name
project(HW6_Display_ADC)

# Initialize the SDK
pico_sdk_init()

# Add executable and source files
add_executable(HW6_Display_ADC
    main.c
    ssd1306.c
    font.h
    ssd1306.h
)

# Link required Pico libraries
target_link_libraries(HW6_Display_ADC pico_stdlib hardware_i2c hardware_adc)

# Enable USB stdio for printf
pico_enable_stdio_usb(HW6_Display_ADC 1)
pico_enable_stdio_uart(HW6_Display_ADC 0)

# Create UF2 file and other outputs
pico_add_extra_outputs(HW6_Display_ADC)