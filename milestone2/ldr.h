#ifndef LDR_H
#define LDR_H

#include "pico/stdlib.h"
#include "hardware/adc.h"

// Define the LDR ADC pin
#define LDR_PIN 26

// Function prototypes
void ldr_init(void);           // Initialize the LDR (ADC pin and ADC setup)
float ldr_read_voltage(void);  // Read the LDR voltage

#endif // LDR_H
