#include "ldr.h"
#include <stdio.h>

// Initialize the LDR and ADC
void ldr_init(void) {
    // Initialize ADC
    adc_init();
    
    // Initialize the GPIO for ADC input
    adc_gpio_init(LDR_PIN);
    
    // Set the ADC to read from the correct input (GPIO 26 = ADC input 0)
    adc_select_input(0);
}

// Read the LDR voltage
float ldr_read_voltage(void) {
    // Read the raw ADC value (12-bit resolution: 0 to 4095)
    uint16_t raw_adc_value = adc_read();

    // Convert the raw ADC value to a voltage (assuming V_in = 3.3V)
    float voltage = raw_adc_value * (3.3f / 4095.0f);
    printf("Calculated RPM: %u\n", voltage);

    return voltage; // Return the calculated voltage
}
