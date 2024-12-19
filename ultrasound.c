#include "pico/stdlib.h"
#include "ultrasound.h"

void ultrasound_init(void) {
    // Initialize sensor trigger & echo pins
    gpio_init(ULTRASOUND_TRIG_PIN);
    gpio_set_dir(ULTRASOUND_TRIG_PIN, GPIO_OUT);

    gpio_init(ULTRASOUND_ECHO_PIN);
    gpio_set_dir(ULTRASOUND_ECHO_PIN, GPIO_IN);

    // Ensure the trigger pin is low
    gpio_put(ULTRASOUND_TRIG_PIN, false);
}

float ultrasound_read(void) {
    // Send a clean high signal to the trigger pin for at least 10 µs
    gpio_put(ULTRASOUND_TRIG_PIN, false);
    sleep_us(2);
    gpio_put(ULTRASOUND_TRIG_PIN, true);
    sleep_us(10);
    gpio_put(ULTRASOUND_TRIG_PIN, false);

    // Wait for the echo signal to start
    uint32_t timeout = 30000; // 30 ms timeout
    while (gpio_get(ULTRASOUND_ECHO_PIN) == 0 && timeout--) {
        sleep_us(1);
    }
    if (timeout == 0) {
        return -1; // Return -1 to indicate no signal
    }

    // Measure the pulse width
    uint32_t startTime = time_us_32();
    timeout = 30000; // Reset timeout
    while (gpio_get(ULTRASOUND_ECHO_PIN) == 1 && timeout--) {
        sleep_us(1);
    }
    if (timeout == 0) {
        return -1; // Return -1 to indicate no signal
    }

    uint32_t pulse_width = time_us_32() - startTime;
    float centimeters = pulse_width / 58.0f; // Convert pulse width to cm

    return centimeters;
}
