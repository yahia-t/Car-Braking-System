#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"

// Define GPIO pins for the LEDs
#define LED1_PIN 14  // Default onboard LED
#define LED2_PIN 15  // External LED, change as required

// Function Prototypes
void led_init(void);           // Initialize both LEDs
void led1_on(void);            // Turn LED1 on
void led1_off(void);           // Turn LED1 off
void led1_toggle(void);        // Toggle LED1 state
void led2_on(void);            // Turn LED2 on
void led2_off(void);           // Turn LED2 off
void led2_toggle(void);        // Toggle LED2 state

#endif // LED_H
