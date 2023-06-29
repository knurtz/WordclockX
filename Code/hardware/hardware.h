#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

#include "hardware/i2c.h"
#include "hardware/uart.h"


#define kHz * 1000
#define MHz * 1000 * 1000


// Generic IOs
#define ONBOARD_LED         23      // Output
#define LED_PWM_SLICE       3
#define LED_PWM_CHAN        PWM_CHAN_B
#define POWER_STATUS        2       // Input, pull-up



// Initializes all GPIOs
bool Hardware_Init(void);


#endif /* HARDWARE_H */
