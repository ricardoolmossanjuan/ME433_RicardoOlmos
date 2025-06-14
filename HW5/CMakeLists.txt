cmake_minimum_required(VERSION 3.13)

# Include Pico SDK (local copy or relative path)
include(pico_sdk_import.cmake)

# Set project name
project(HW5_I2C_GPIO_Expander)

# Initialize the SDK
pico_sdk_init()

# Define the executable (matches your .c file name)
add_executable(HW5_I2C_GPIO_Expander
    i2c_io_expander.c
)

# Link necessary libraries
target_link_libraries(HW5_I2C_GPIO_Expander pico_stdlib hardware_i2c)

# Enable USB for printf output
pico_enable_stdio_usb(HW5_I2C_GPIO_Expander 1)
pico_enable_stdio_uart(HW5_I2C_GPIO_Expander 0)

# Generate .uf2 and other outputs
pico_add_extra_outputs(HW5_I2C_GPIO_Expander)