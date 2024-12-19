#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"

// Define the GPIO pin for the buzzer
#define BUZZER_PIN 0

// Function prototypes
void buzzer_init(void);          // Initialize the buzzer
void buzzer_on(void);  // Turn the buzzer on with a specific frequency (Hz)
void buzzer_off(void);           // Turn the buzzer off

#endif // BUZZER_H
