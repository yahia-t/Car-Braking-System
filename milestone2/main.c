#include "pico/stdlib.h"
#include "motor.h"
#include "ultrasound.h"
#include "accelerometer.h"
#include "led.h"
#include "ldr.h"
#include "buzzer.h"
#include "freeRTOS.h
#include "task.h"
#include <stdio.h>

#define OBSTACLE_DISTANCE_CM 20   // Distance threshold for obstacle detection
#define MIN_VALID_DISTANCE_CM 2  // Minimum valid distance to filter out noise

#define ACCELEROMETER_DELAY 500
#define ULTRASOUND_DELAY 100
#define ACT_DURATION 3000 // Duration of actuators activity
#define MOTOR_GRACE 2000 // Grace period to move forward before checking obstacles

float distance = -1;
bool decelerating = false;

void accelerometer_task(void * pvParameters) {
    accelerometer_init();
    uint32_t current_rpm = 0, prev_rpm = 0;

    while (1) {
        current_rpm = accelerometer_read(); // Read the current RPM
        decelerating = (current_rpm < prev_rpm - 10) && (current_rpm > 0) && distance <= OBSTACLE_DISTANCE_CM;
        prev_rpm = current_rpm;
        vTaskDelay(pdMS_TO_TICKS(ACCELEROMETER_DELAY));
    }
}

void act_task(void * pvParameters) {
    led_init();
    buzzer_init();

    while (1) {
        if (decelerating) {
            if(ldr_read_voltage() < 1.5){
                printf("Deceleration detected! Turned on LEDs.\n");
                led1_on(); 
                led2_on();
                vTaskDelay(pdMS_TO_TICKS(ACT_DURATION));
                printf("Turned off LEDs after deceleration duration.\n");
                led1_off(); 
                led2_off();
            }
            else{
                printf("Deceleration detected! Turned on buzzer.\n");
                buzzer_on(); 
                vTaskDelay(pdMS_TO_TICKS(ACT_DURATION));
                printf("Turned off buzzer after deceleration duration.\n");
                buzzer_off();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(ACCELEROMETER_DELAY));
    }
}

void ultrasound_task(void * pvParameters) {
    ultrasound_init();

    while (1) {
        distance = ultrasound_read();
        vTaskDelay(pdMS_TO_TICKS(ULTRASOUND_DELAY));
    }
}

void motor_task(void * pvParameters) {
    motor_init();
    motor_forward();
    bool obstacled = false;
    vTaskDelay(pdMS_TO_TICKS(MOTOR_GRACE));

    while (1) {
        obstacled = distance <= OBSTACLE_DISTANCE_CM && distance >= MIN_VALID_DISTANCE_CM && !obstacled;
        if (obstacled) {
            printf("Obstacle detected! Starting deceleration...\n");
            motor_start_slowdown(); // Begin non-blocking deceleration
        }
        motor_slowdown_until_stop_step(); // Gradually decrease speed in steps IF decelerating
        vTaskDelay(pdMS_TO_TICKS(ULTRASOUND_DELAY));
    }
}

int main() {
    stdio_init_all(); // Initialized for debugging
    ldr_init(); // No task for the ldr as it isn't periodically executed

    printf("System starting up...\n");
    xTaskCreate(accelerometer_task, "Accelerometer Task", 256, NULL, 1, NULL);
    xTaskCreate(act_task, "Actuators Task", 256, NULL, 2, NULL);
    xTaskCreate(ultrasound_task, "Ultrasound Task", 256, NULL, 1, NULL);
    xTaskCreate(motor_task, "Motor Task", 256, NULL, 2, NULL);

    vTaskStartScheduler(); // Found in FreeRTOS Kernel Control Kernel Control - FreeRTOS™
    while (1) {};
}
