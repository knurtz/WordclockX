#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

#define kHz * 1000
#define MHz * 1000 * 1000

// Generic IOs
#define LED1                    10
#define LED2                    11
#define LED3                    12
#define LED4                    13

// Initializes all GPIOs
bool Hardware_Init(void);

#endif /* HARDWARE_H */
