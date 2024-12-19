#ifndef ULTRASOUND_H
#define ULTRASOUND_H

// Define Sensor GPIO Pins
#define ULTRASOUND_TRIG_PIN 10
#define ULTRASOUND_ECHO_PIN 11

// Function Prototypes
void ultrasound_init(void);// Initializes the sensor pins, returns none.
float ultrasound_read(void);  // Reads the sensor, returns distance.

#endif // ULTRASOUND_H