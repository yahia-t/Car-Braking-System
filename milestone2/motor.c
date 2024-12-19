#include "motor.h"
#include "hardware/pwm.h"

static int motor_speed = 65535;  // Initial motor speed
static bool motor_stopping = false;  // Indicates if the motor is slowing down


// Function to initialize GPIO pins and PWM
void motor_init() {
    uint32_t pwm_wrap = 6249;  // Wrap value for 20 kHz frequency

    // Initialize Motor 1 direction pins
    gpio_init(MOTOR1_IN1);
    gpio_set_dir(MOTOR1_IN1, GPIO_OUT);
    gpio_init(MOTOR1_IN2);
    gpio_set_dir(MOTOR1_IN2, GPIO_OUT);

    // Initialize Motor 2 direction pins
    gpio_init(MOTOR2_IN3);
    gpio_set_dir(MOTOR2_IN3, GPIO_OUT);
    gpio_init(MOTOR2_IN4);
    gpio_set_dir(MOTOR2_IN4, GPIO_OUT);

    // Initialize PWM for Motor 1
    gpio_set_function(MOTOR1_ENA, GPIO_FUNC_PWM);
    uint slice_num1 = pwm_gpio_to_slice_num(MOTOR1_ENA);
    pwm_set_wrap(slice_num1, pwm_wrap);  // Set wrap for 20 kHz frequency
    pwm_set_gpio_level(MOTOR1_ENA, pwm_wrap);  // Full duty cycle
    pwm_set_enabled(slice_num1, true);

    // Initialize PWM for Motor 2
    gpio_set_function(MOTOR2_ENB, GPIO_FUNC_PWM);
    uint slice_num2 = pwm_gpio_to_slice_num(MOTOR2_ENB);
    pwm_set_wrap(slice_num2, pwm_wrap);  // Set wrap for 20 kHz frequency
    pwm_set_gpio_level(MOTOR2_ENB, pwm_wrap);  // Full duty cycle
    pwm_set_enabled(slice_num2, true);
}


// Function to drive both motors forward
void motor_forward() {
    uint32_t pwm_wrap = 6249;  // Ensure this matches the value in motor_init()

    // Motor 1 forward
    gpio_put(MOTOR1_IN1, true);
    gpio_put(MOTOR1_IN2, false);
    pwm_set_gpio_level(MOTOR1_ENA, pwm_wrap);  // Set full duty cycle for Motor 1

    // Motor 2 forward
    gpio_put(MOTOR2_IN3, true);
    gpio_put(MOTOR2_IN4, false);
    pwm_set_gpio_level(MOTOR2_ENB, pwm_wrap);  // Set full duty cycle for Motor 2
}



void motor_slowdown_until_stop_step() {
    uint32_t pwm_wrap = 6249;  // Ensure this matches the value in motor_init()
    uint slice_num1 = pwm_gpio_to_slice_num(MOTOR1_ENA);
    uint slice_num2 = pwm_gpio_to_slice_num(MOTOR2_ENB);

    if (motor_stopping && motor_speed > 0) {
        motor_speed -= 500;  // Adjust this decrement value as needed for smooth slowdown
        if (motor_speed < 0) motor_speed = 0;

        // Scale motor_speed to match the wrap value
        uint32_t scaled_speed = (motor_speed * pwm_wrap) / 65535;

        pwm_set_gpio_level(MOTOR1_ENA, scaled_speed);
        pwm_set_gpio_level(MOTOR2_ENB, scaled_speed);

        if (motor_speed == 0) {
            // Stop motors completely
            gpio_put(MOTOR1_IN1, false);
            gpio_put(MOTOR1_IN2, false);
            gpio_put(MOTOR2_IN3, false);
            gpio_put(MOTOR2_IN4, false);
            motor_stopping = false;  // Mark stopping process as complete
        }
    }
}


void motor_start_slowdown() {
    motor_stopping = true;
}
