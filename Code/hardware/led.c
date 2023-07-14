#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "hardware.h"

#define LED_DEFAULT_FPS  50

#define LED_ON_LEVEL    300
#define LED_OFF_LEVEL   0

uint8_t frame_counter = 0;
repeating_timer_t led_timer;

static inline void LED_SetLevel(uint led, uint level)
{
    pwm_set_chan_level(pwm_gpio_to_slice_num(led), pwm_gpio_to_channel(led), level);
}

static bool LED_Callback(repeating_timer_t *t)
{
    switch(++frame_counter)
    {
        case 1:
            LED_SetLevel(LED4, LED_ON_LEVEL);
            break;
        case 5:
            LED_SetLevel(LED4, LED_OFF_LEVEL);
            LED_SetLevel(LED3, LED_ON_LEVEL);
            break;

        case 10:
            LED_SetLevel(LED3, LED_OFF_LEVEL);
            LED_SetLevel(LED2, LED_ON_LEVEL);
            break;

        case 15:
            LED_SetLevel(LED2, LED_OFF_LEVEL);
            LED_SetLevel(LED1, LED_ON_LEVEL);
            break;

        case 20:
            LED_SetLevel(LED1, LED_OFF_LEVEL);
            break;
        
        case 100:
            frame_counter = 0;
    }

    return true;
}

void LED_Init(void)
{
    add_repeating_timer_ms(1000 / LED_DEFAULT_FPS, LED_Callback, NULL, &led_timer);
}

void LED_SetFPS(uint fps)
{
    led_timer.delay_us = 1000000 / fps;
}