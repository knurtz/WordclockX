#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

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
    
    // Init LEDs PWM
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&cfg, 250);                           // 125 MHz system / 250 = 500 kHz / wrap @ 1000 = 500 Hz PWM
    pwm_config_set_wrap(&cfg, 1000);

    pwm_init(pwm_gpio_to_slice_num(LED1), &cfg, true);
    pwm_init(pwm_gpio_to_slice_num(LED3), &cfg, true);

    for (uint led_i = LED1; led_i <= LED4; led_i++)
    {
        pwm_set_chan_level(pwm_gpio_to_slice_num(led_i), pwm_gpio_to_channel(led_i), 0);           // init at 0.0 % doodie cycle
        gpio_set_function(led_i, GPIO_FUNC_PWM);
    }

    // Init I2C    
    i2c_init(i2c0, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Init row selection shift reigster
    Hardware_InitOutput(ROWSEL_EN, 0);
    Hardware_InitOutput(ROWSEL_LATCH, 1);
    Hardware_InitOutput(ROWSEL_A0, 0);
    Hardware_InitOutput(ROWSEL_A1, 0);
    Hardware_InitOutput(ROWSEL_A2, 0);

    // Init driver enable lines
    Hardware_InitOutput(DRIVER1_NOE, 1);
    Hardware_InitOutput(DRIVER2_NOE, 1);
      
    if (errors)
    {
        LED_SetFPS(500);
        return false;
    }

    return true;
}