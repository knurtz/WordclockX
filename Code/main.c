#include "pico/stdlib.h"

#include "hardware/pwm.h"

#include "hardware.h"
#include "shell.h"
#include "led.h"
#include "led_matrix.h"
#include "rtc.h"
#include "fast_hsv2rgb.h"
#include "clock.h"

uint main_delay = 30;

uint main_counter = 0;
uint8_t minute_counter = 0;

int main() {
    stdio_usb_init();

    Hardware_Init();
    Shell_Init();

    LED_Init();
    RTC_Init();
    LEDMatrix_Init();

    Clock_EsIst();

    /*
    HSVColor c = {0, 255, 20};
    uint8_t r, g, b;

    for (uint8_t x = 0; x < 16; x++)
    {
        c.h = x * 50;
        for  (uint8_t y = 0; y < 8; y++)
        {
            fast_hsv2rgb(c.h, c.s, c.v, &r, &g, &b);
            LEDMatrix_PutPixel(x, y, r, g, b);
            c.h += 192;     // 45 deg
            c.h %= 1536;    // 360 deg
        }
    }
    */
    
    while (1)
    {
        Shell_CheckCommand();

        LEDMatrix_ShowNextRow();
        sleep_us(main_delay);

        if (main_counter++ > 500)
        {
            main_counter = 0;
            Clock_ShowMinutes(minute_counter, 0, 0, 0);
            if (minute_counter++ > 19) minute_counter = 1;
            Clock_ShowMinutes(minute_counter, 10, 10, 10);
        }
    }

}


void _close() {}
void _lseek() {}