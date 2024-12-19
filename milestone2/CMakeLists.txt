# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13)

# Include Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set project name and C/C++ standards
project(car_control_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(car_control
    main.c
    motor.c
    ultrasound.c
    accelerometer.c
    led.c
    ldr.c
    buzzer.c
)

# Link to necessary libraries
target_link_libraries(car_control pico_stdlib hardware_pwm hardware_adc)

# Enable USB output for printf debugging
pico_enable_stdio_usb(car_control 1)
pico_enable_stdio_uart(car_control 0)

# Generate output files (UF2, bin, etc.)
pico_add_extra_outputs(car_control)
