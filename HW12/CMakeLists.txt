cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(hw12_servo_pwm C CXX ASM)
pico_sdk_init()

add_executable(hw12_servo_pwm
    main.c
)

target_link_libraries(hw12_servo_pwm
    pico_stdlib
    hardware_pwm
)

pico_enable_stdio_usb(hw12_servo_pwm 1)
pico_enable_stdio_uart(hw12_servo_pwm 0)

pico_add_extra_outputs(hw12_servo_pwm)
