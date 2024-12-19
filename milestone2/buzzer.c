#include "buzzer.h"

// Initialize the buzzer GPIO pin
void buzzer_init(void) {
    gpio_init(BUZZER_PIN);          // Initialize the GPIO pin
    gpio_set_dir(BUZZER_PIN, GPIO_OUT); // Set it as an output pin
    gpio_put(BUZZER_PIN, false);    // Ensure the buzzer starts off
}

// Turn the buzzer on
void buzzer_on(void) {
    gpio_put(BUZZER_PIN, true);     // Set the GPIO pin HIGH to activate the buzzer
}

// Turn the buzzer off
void buzzer_off(void) {
    gpio_put(BUZZER_PIN, false);    // Set the GPIO pin LOW to deactivate the buzzer
}
