#include "led.h"

// Initialize both LEDs
void led_init(void) {
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);

    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
}

// Turn LED1 on
void led1_on(void) {
    gpio_put(LED1_PIN, true);
}

// Turn LED1 off
void led1_off(void) {
    gpio_put(LED1_PIN, false);
}

// Toggle LED1 state
void led1_toggle(void) {
    gpio_xor_mask(1u << LED1_PIN);
}

// Turn LED2 on
void led2_on(void) {
    gpio_put(LED2_PIN, true);
}

// Turn LED2 off
void led2_off(void) {
    gpio_put(LED2_PIN, false);
}

// Toggle LED2 state
void led2_toggle(void) {
    gpio_xor_mask(1u << LED2_PIN);
}
