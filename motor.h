#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"

// Define GPIO pins for Motor 1
#define MOTOR1_IN1 2
#define MOTOR1_IN2 3
#define MOTOR1_ENA 4  // PWM pin for speed control

// Define GPIO pins for Motor 2
#define MOTOR2_IN3 5
#define MOTOR2_IN4 6
#define MOTOR2_ENB 7  // PWM pin for speed control

// Function prototypes
void motor_init();                    // Initialize motor control pins
void motor_forward();                 // Drive motors forward
void motor_slowdown_until_stop();     // Slow down motors until stop

#endif // MOTOR_H