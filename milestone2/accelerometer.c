#include <stdio.h>
#include "pico/stdlib.h"
#include "accelerometer.h"


// Global variables
uint32_t count = 0;        // Revolution count


// Interrupt Service Routine (ISR)
void accelerometer_isr(uint gpio, uint32_t events) {
    count++;
    

}


// Initialize the accelerometer
void accelerometer_init(void) {
    gpio_init(ACCELEROMETER_PIN);                            // Initialize GPIO pin
    gpio_set_dir(ACCELEROMETER_PIN, GPIO_IN);                // Set as input
    gpio_set_irq_enabled_with_callback(ACCELEROMETER_PIN, 
                                       GPIO_IRQ_EDGE_FALL, 
                                       true, 
                                       &accelerometer_isr);  // Enable interrupt
                             
}

// Read the RPM
uint32_t accelerometer_read() {
    gpio_set_irq_enabled(ACCELEROMETER_PIN, GPIO_IRQ_EDGE_FALL, false); // Disable interrupts

    uint32_t revolutions = count ;      // Example: 20 pulses per revolution

    count = 0;

    printf("Calculated RPM: %u\n", revolutions);


    //uint32_t rpm = (elapsedTime > 0) ? (revolutions * 60000000) / elapsedTime : 0;

    // Log pulse count and elapsed time
    //printf("Pulse Count: %u, Elapsed Time: %u us, Calculated RPM: %u\n", count, elapsedTime, rpm);
    //printf("Calculated RPM: %u\n", rpm);

    // count = 0;               // Reset revolution count
    // startTime = time_us_32(); // Reset start time

    gpio_set_irq_enabled_with_callback(ACCELEROMETER_PIN, GPIO_IRQ_EDGE_FALL, true, &accelerometer_isr); // Re-enable interrupts

    return revolutions; // Return the calculated RPM
}

