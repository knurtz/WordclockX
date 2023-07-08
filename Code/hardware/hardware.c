#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "hardware.h"
#include "led.h"

static void Hardware_InitOutput(uint pin, bool val)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, val);
}

static void Hardware_InitInputPullup(uint pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

bool Hardware_Init(void)
{    
    bool errors = false;
    
    // 120 Mhz clock speed instead of default 125 MHz is more useful to us
    // Calling this function limits peripheral clocks to 48 MHz, since it automatically gets tied to PLL_USB
    //if (!set_sys_clock_khz(120000, false)) errors = true;
    
    /*
    // Init LED PWM
    gpio_set_function(LED1, GPIO_FUNC_PWM);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&cfg, 250);                           // 125 MHz system / 250 = 500 kHz / wrap @ 1000 = 500 Hz PWM
    pwm_config_set_wrap(&cfg, 1000);
    pwm_init(pwm_gpio_to_slice_num(LED1), &cfg, true);
    pwm_set_chan_level(pwm_gpio_to_slice_num(LED1), pwm_gpio_to_channel(LED1), 0);           // init at 0.0 % doodie cycle
    */

    Hardware_InitOutput(LED1, 0);
    Hardware_InitOutput(LED2, 0);
    Hardware_InitOutput(LED3, 0);
    Hardware_InitOutput(LED4, 0);
  
    if (errors)
    {
        LED_SetFPS(500);
        return false;
    }

    return true;
}