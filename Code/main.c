#include "pico/stdlib.h"

#include "hardware/pwm.h"

#include "hardware.h"
#include "shell.h"
#include "led.h"
#include "rtc.h"

int main() {
    stdio_init_all();

    Hardware_Init();
    Shell_Init();

    LED_Init();
    if (!RTC_Init()) LED_SetFPS(200);
    
    while (1)
    {
        Shell_CheckCommand();
    }

}


void _close() {}
void _lseek() {}