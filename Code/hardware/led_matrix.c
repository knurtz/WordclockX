#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "hardware.h"
#include "led_matrix.h"

#define PCA_AIF         7

void LEDMatrix_Init(void)
{
    // Init LED drivers
    uint8_t reg_mode1[2] = {0x00, 0x81};                        // 0x81: Enable auto increment and all call address
    uint8_t reg_ledout[7] = {(1 << PCA_AIF) | 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    i2c_write_blocking(i2c0, 0x70, reg_mode1, 2, false);        // Use all call address for this transaction to write both drivers at the same time
    i2c_write_blocking(i2c0, 0x70, reg_ledout, 7, false);
    sleep_us(600);

    gpio_put(DRIVER1_NOE, 0);
    gpio_put(DRIVER2_NOE, 0);
}

void LEDMatrix_SelectRow(uint8_t row)
{   
    // Disable row output
    gpio_put(ROWSEL_EN, 0);
    // Write new row to address pins
    gpio_put_masked(0b111 << ROWSEL_A0, row << ROWSEL_A0);
    // Latch
    gpio_put(ROWSEL_LATCH, 0);
    sleep_us(1);
    gpio_put(ROWSEL_LATCH, 1);
    // Enable output
    gpio_put(ROWSEL_EN, 1);
}

void LEDMatrix_DeselectRow(void)
{
    // Disable row output
    gpio_put(ROWSEL_EN, 0);
}

void LEDMatrix_SelectCol(uint8_t col, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t address = col < 8 ? 0x60 : 0x40;
    uint8_t payload[4] = {(1 << PCA_AIF) + 2 + col * 3, r, g, b};
    i2c_write_blocking(i2c0, address, payload, 4, false);
}