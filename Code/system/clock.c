#include "led_matrix.h"

/*
     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15

7    E  S     I  S  T     Z  W  E  I  N  E  U  N  
6    D  R  E  I  V  I  E  R  F  Ü  N  F  A  C  H  T  
5    S  E  C  H  S  I  E  B  E  N     Z  E  H  N  
4    E  L  F  Z  W  Ö  L  F     M  I  N  U  T  E  N  
3    N  A  C  H  V  O  R     H  A  L  B     U  H  R  
2    Z  W  E  I  N  S  I  E  B  E  N  E  U  N  
1    E  L  F  Ü  N  F  Z  W  Ö  L  F  S  E  C  H  S  
0    D  R  E  I  V  I  E  R  A  C  H  T  Z  E  H  N  

*/

typedef struct text_position {
    uint8_t start_x;
    uint8_t start_y;
    uint8_t length;
} TextPosition;

TextPosition minutes[] =
{
    {9, 7, 4},      // EINE
    {7, 7, 4},      // ZWEI
    {0, 6, 4},      // DREI
    {4, 6, 4},      // VIER
    {8, 6, 4},      // FÜNF
    {0, 5, 5},      // SECHS
    {4, 5, 6},      // SIEBEN
    {12, 6, 4},     // ACHT
    {11, 7, 4},     // NEUN
    {11, 5, 4},     // ZEHN
    {0, 4, 3},      // ELF
    {3, 4, 5}       // ZWÖLF
};

// min: values from 1 - 19 allowed
void Clock_ShowMinutes(uint8_t min, uint8_t r, uint8_t g, uint8_t b)
{
    if (min < 1 || min > 19) return;

    TextPosition t = min <= 12 ? minutes[min - 1] : minutes[min - 11];
    
    // Draw text for ones place or 10 - 12
    for (uint8_t i = 0; i < t.length; i++)
    {
        LEDMatrix_PutPixel(t.start_x + i, t.start_y, r, g, b);
    }

    // For numbers from 13 and up draw an additional text for 10
    if (min > 12)
    {
        t = minutes[9];
        for (uint8_t i = 0; i < t.length; i++)
        {
            LEDMatrix_PutPixel(t.start_x + i, t.start_y, r, g, b);
        }

        // special case 17
        if (min == 17)
        {
            LEDMatrix_PutPixel(8, 5, 0, 0, 0);
            LEDMatrix_PutPixel(9, 5, 0, 0, 0);
        }
    }
}

void Clock_EsIst(void)
{
    // ES
    LEDMatrix_PutPixel(0, 7, 10, 10, 10);
    LEDMatrix_PutPixel(1, 7, 10, 10, 10);

    // IST
    LEDMatrix_PutPixel(3, 7, 10, 10, 10);
    LEDMatrix_PutPixel(4, 7, 10, 10, 10);
    LEDMatrix_PutPixel(5, 7, 10, 10, 10);
}


