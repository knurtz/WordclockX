#include "pico/stdlib.h"

#include "hardware/pwm.h"

#include "hardware.h"
#include "shell.h"
#include "led.h"
#include "led_matrix.h"
#include "rtc.h"

int main() {
    stdio_usb_init();

    Hardware_Init();
    Shell_Init();

    LED_Init();
    RTC_Init();
    LEDMatrix_Init();

    LEDMatrix_SelectRow(1);
    
    while (1)
    {
        Shell_CheckCommand();
    }

}


void _close() {}
void _lseek() {}