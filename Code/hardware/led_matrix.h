#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "pico/stdlib.h"

void LEDMatrix_Init(void);

void LEDMatrix_SelectRow(uint8_t row);
void LEDMatrix_DeselectRow(void);

void LEDMatrix_SelectCol(uint8_t col, uint8_t r, uint8_t g, uint8_t b);

#endif /* LED_MATRIX_H */