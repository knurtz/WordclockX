#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "hardware.h"
#include "led_matrix.h"

LEDMatrix_SelectRow(uint8_t row)
{   
    // Disable row output
    gpio_put(ROWSEL_EN, 0);
    gpio_put_masked(0b111 << ROWSEL_A0, row << ROWSEL_A0);
}