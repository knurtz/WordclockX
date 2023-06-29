#include "pico/stdlib.h"

#include "hardware.h"
#include "shell.h"
#include "led.h"


int main() {
    stdio_init_all();

    Hardware_Init();
    Shell_Init();

    LED_Init();
    
    while (1)
    {
        Shell_CheckCommand();
    }


}


void _close() {}
void _lseek() {}