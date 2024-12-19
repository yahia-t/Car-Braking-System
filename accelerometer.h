#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


#include "pico/stdlib.h"

// Define Sensor GPIO Pin
#define ACCELEROMETER_PIN 12  // Correct the name to be generic

// Function Prototypes
void accelerometer_init(void);                     // Initializes the sensor pin
void accelerometer_isr(uint gpio, uint32_t events); // Interrupt Service Routine
uint32_t accelerometer_read(void);                 // Reads RPM

#endif // ACCELEROMETER_H
