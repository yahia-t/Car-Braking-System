# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

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
target_link_libraries(car_control
    pico_stdlib          # Standard library
    hardware_pwm         # Hardware PWM
    hardware_adc         # Hardware ADC
    hardware_gpio        # GPIO functions
    hardware_timer       # Timer functions
    hardware_irq         # IRQ handling
)

# Enable USB output for printf debugging
pico_enable_stdio_usb(car_control 1)
pico_enable_stdio_uart(car_control 0)

# Add FreeRTOS
set(FREERTOS_PATH ${CMAKE_SOURCE_DIR}/freeRTOS) # Path to your FreeRTOS folder
if (EXISTS ${FREERTOS_PATH})
    message(STATUS "Adding FreeRTOS from ${FREERTOS_PATH}")

    # Include FreeRTOS include, portable, and config directories
    target_include_directories(car_control PRIVATE ${FREERTOS_PATH}/include)
    target_include_directories(car_control PRIVATE ${FREERTOS_PATH}/portable/GCC/ARM_CM0)
    target_include_directories(car_control PRIVATE ${FREERTOS_PATH}/config)

    # Add FreeRTOS source files
    file(GLOB FREERTOS_SOURCES
        ${FREERTOS_PATH}/Source/*.c
        ${FREERTOS_PATH}/Source/portable/GCC/ARM_CM0/*.c
    )
    target_sources(car_control PRIVATE ${FREERTOS_SOURCES})
else()
    message(FATAL_ERROR "FreeRTOS directory not found at ${FREERTOS_PATH}. Please verify the location.")
endif()

# Generate output files (UF2, bin, etc.)
pico_add_extra_outputs(car_control)
