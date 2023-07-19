#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"

#define MATRIX_ROWS     8
#define MATRIX_COLS     16

void LEDMatrix_PutPixel(uint8_t col, uint8_t row, uint8_t r, uint8_t g, uint8_t b);

void LEDMatrix_Init(void);

void LEDMatrix_DeselectRow(void);
void LEDMatrix_SelectRow(uint8_t row);

void LEDMatrix_ShowNextRow(void);

#endif /* LED_MATRIX_H */