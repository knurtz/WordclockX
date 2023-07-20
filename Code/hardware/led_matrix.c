#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "hardware.h"
#include "led_matrix.h"

#define AIF_FLAG         7

/*
Framebuffer layout:

R0 R1 .. R6 R7 G7 G6 .. G1 G0 B0 B1 .. B6 B7 R8 R9 .. R14 R15 G15 G14 .. G9 G8 B8 B9 .. B14 B15
.
.

*/

uint8_t led_framebuffer[3 * MATRIX_ROWS * MATRIX_COLS];

uint8_t current_row = 0;

void LEDMatrix_PutPixel(uint8_t col, uint8_t row, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t* offset = led_framebuffer + row * 3 * MATRIX_COLS;
    if (col >= MATRIX_COLS / 2)
    {
        offset += 3 * MATRIX_COLS / 2;
        col -= MATRIX_COLS / 2;
    }
    *(offset + col) = r;
    *(offset + 15 - col) = g;
    *(offset + 16 + col) = b;
}

void LEDMatrix_Init(void)
{
    // Init LED drivers
    uint8_t reg_mode1[2] = {0x00, 0x81};                        // 0x81: Enable auto increment and all call address
    uint8_t reg_ledout[7] = {(1 << AIF_FLAG) | 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    i2c_write_blocking(i2c0, 0x70, reg_mode1, 2, false);        // Use all call address for these transactions to write to both drivers at the same time
    i2c_write_blocking(i2c0, 0x70, reg_ledout, 7, false);
    sleep_us(600);

    gpio_put(DRIVER1_NOE, 0);
    gpio_put(DRIVER2_NOE, 0);

    // test pattern
    /*
    for (int x = 0; x < MATRIX_COLS; x++)
        for (int y = 0; y < MATRIX_ROWS; y++)
            LEDMatrix_PutPixel(x, y, 20, 70, 100);
            */

    memset(led_framebuffer, 50, sizeof(led_framebuffer));
}

void LEDMatrix_DeselectRow(void)
{
    // Disable row output
    gpio_put(ROWSEL_EN, 0);
}

void LEDMatrix_SelectRow(uint8_t row)
{   
    // Disable row output
    //gpio_put(ROWSEL_EN, 0);
    // Write new row to address pins
    gpio_put_masked(0b111 << ROWSEL_A0, row << ROWSEL_A0);
    // Latch
    gpio_put(ROWSEL_LATCH, 0);
    //sleep_us(1);
    gpio_put(ROWSEL_LATCH, 1);
    // Enable output
    gpio_put(ROWSEL_EN, 1);
}

void LEDMatrix_ShowNextRow(void)
{
    if (++current_row >= MATRIX_ROWS) current_row = 0;
    // Disable LED outputs while sending new data
    LEDMatrix_DeselectRow();
    //uint8_t reg = (1 << AIF_FLAG) | 0x02;
    uint8_t data[25];
    data[0] = (1 << AIF_FLAG) | 0x02;
    memcpy(&data[1], led_framebuffer + current_row * 3 * MATRIX_COLS, 3 * MATRIX_COLS / 2);
    // Write left side
    //i2c_write_blocking(i2c0, 0x60, &reg, 1, true);
    //i2c_write_blocking(i2c0, 0x60, led_framebuffer + current_row * 3 * MATRIX_COLS, 3 * MATRIX_COLS / 2, true);
    i2c_write_blocking(i2c0, 0x60, data, 25, true);
    memcpy(&data[1], led_framebuffer + current_row * 3 * MATRIX_COLS + 3 * MATRIX_COLS / 2, 3 * MATRIX_COLS / 2);
    // Write right side
    //i2c_write_blocking(i2c0, 0x40, &reg, 1, true);
    //i2c_write_blocking(i2c0, 0x40, led_framebuffer + current_row * 3 * MATRIX_COLS + 3 * MATRIX_COLS / 2, 3 * MATRIX_COLS / 2, false);
    i2c_write_blocking(i2c0, 0x40, data, 25, false);
    // Enable next row
    LEDMatrix_SelectRow(current_row);
}