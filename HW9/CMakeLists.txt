cmake_minimum_required(VERSION 3.13)

# Import Pico SDK
include(pico_sdk_import.cmake)

# Name of your project
project(HW9_PicoSide)

# Initialize the SDK
pico_sdk_init()

# Define the executable and source files
add_executable(HW9_PicoSide
    main.c
)

# Link necessary libraries
target_link_libraries(HW9_PicoSide
    pico_stdlib
    hardware_uart
)

# Enable USB stdio for printf
pico_enable_stdio_usb(HW9_PicoSide 1)
pico_enable_stdio_uart(HW9_PicoSide 0)

# Generate UF2 + bin files
pico_add_extra_outputs(HW9_PicoSide)
