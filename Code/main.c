#include "pico/stdlib.h"

#include "hardware/pwm.h"

#include "hardware.h"
#include "shell.h"
#include "led.h"
#include "led_matrix.h"
#include "rtc.h"

uint main_delay = 1000;

int main() {
    stdio_usb_init();

    Hardware_Init();
    Shell_Init();

    LED_Init();
    RTC_Init();
    LEDMatrix_Init();
    
    while (1)
    {
        Shell_CheckCommand();

        LEDMatrix_ShowNextRow();
        sleep_ms(main_delay);
    }

}


void _close() {}
void _lseek() {}