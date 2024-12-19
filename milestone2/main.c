#include "pico/stdlib.h"
#include "motor.h"
#include "ultrasound.h"
#include "accelerometer.h"
#include "led.h"
#include "ldr.h"
#include "buzzer.h"
#include <stdio.h>

#define OBSTACLE_DISTANCE_CM 20   // Distance threshold for obstacle detection
#define MIN_VALID_DISTANCE_CM 2  // Minimum valid distance to filter out noise
#define LED_ON_DURATION_MS 1000  // Duration to keep LEDs on during deceleration (1 second)
#define ULTRASOUND_DELAY_MS 100  // Delay between ultrasonic readings (100ms)

int main() {
    stdio_init_all();            // Initialize stdio for debugging
    motor_init();                // Initialize motor
    ultrasound_init();           // Initialize ultrasonic sensor
    accelerometer_init();        // Initialize accelerometer
    led_init();                  // Initialize LEDs
    ldr_init();
    buzzer_init();

    printf("System starting up...\n");
    motor_forward();             // Start the motor moving forward
    sleep_ms(2000);              // Grace period to move forward before checking obstacles

    uint32_t prev_rpm = 0;       // Store the previous RPM reading
    uint32_t current_rpm = 0;    // Store the current RPM reading
    uint32_t last_read_time = time_us_32(); // Track time of last RPM read
    uint32_t led_start_time = 0; // Track when LEDs were turned on
    bool obstacle_detected = false;

    while (true) {
        // Check for obstacles using the ultrasonic sensor
        float distance = ultrasound_read();
        //printf("Distance to obstacle: %.2f cm\n", distance);

        // Add a small delay between ultrasonic sensor readings
        sleep_ms(ULTRASOUND_DELAY_MS);

        // Only trigger deceleration if the distance is valid and below the threshold
        if (distance <= OBSTACLE_DISTANCE_CM && distance >= MIN_VALID_DISTANCE_CM && !obstacle_detected) {
            printf("Obstacle detected! Starting deceleration...\n");
            obstacle_detected = true;
            motor_start_slowdown(); // Begin non-blocking motor slowdown
        }

        // Handle motor slowdown in small steps
        motor_slowdown_until_stop_step();

        // Read RPM every 100ms
        if ((time_us_32() - last_read_time) >= 500000) {
            last_read_time = time_us_32();  // Update last read time
            current_rpm = accelerometer_read(); // Read the current RPM

            //printf("Current RPM: %u, Previous RPM: %u\n", current_rpm, prev_rpm);

            // Detect deceleration
            if ((current_rpm < prev_rpm - 10) && (current_rpm > 0) && distance <= OBSTACLE_DISTANCE_CM ) { 

                if(ldr_read_voltage() < 1.5){
                    led1_on(); 
                    led2_on();
                }
                else{
                    buzzer_on(); 
                }
                printf("Deceleration detected! Turning on LEDs.\n");
                
                led_start_time = time_us_32();  // Record the start time for LEDs
            }

            // Turn off LEDs after the hardcoded duration
            if (led_start_time > 0 && (time_us_32() - led_start_time) >= LED_ON_DURATION_MS * 3000) {
                printf("Turning off LEDs after deceleration duration.\n");
                led1_off();  // Turn off LED1
                led2_off();  // Turn off LED2
                buzzer_off();
                led_start_time = 0;  // Reset LED timer
            }

            prev_rpm = current_rpm; // Update the previous RPM for the next iteration
        }

        sleep_ms(10); // General small delay to prevent busy-waiting
    }

    return 0;
}
