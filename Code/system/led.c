#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "hardware.h"
#include "led.h"

#define LED_DEFAULT_FPS  75

uint frame_counter = 0;
struct repeating_timer led_timer;

static bool LED_Callback(struct repeating_timer *t)
{    
    if (frame_counter == 0) pwm_set_chan_level(LED_PWM_SLICE, LED_PWM_CHAN, 300);
    if (frame_counter == 90) pwm_set_chan_level(LED_PWM_SLICE, LED_PWM_CHAN, 700);

    if (++frame_counter >= 100) frame_counter = 0;
}

void LED_Init(void)
{
    add_repeating_timer_ms(1000 / LED_DEFAULT_FPS, LED_Callback, NULL, &led_timer);
}

void LED_SetFPS(uint fps)
{
    led_timer.delay_us = 1000000 / fps;
}