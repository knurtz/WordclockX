#include "hardware.h"
#include "led.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

pio_spi_inst_t adc_spi = {
    .pio = pio0,
    .sm = 0
};

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
    if (!set_sys_clock_khz(120000, false)) errors = true;
    
    // Init generic IOs
    Hardware_InitInputPullup(POWER_STATUS);

    // Init LED PWM
    gpio_set_function(ONBOARD_LED, GPIO_FUNC_PWM);
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&cfg, 240);                           // 500 kHz / wrap @ 1000 = 500 Hz PWM
    pwm_config_set_wrap(&cfg, 1000);
    pwm_init(LED_PWM_SLICE, &cfg, true);
    pwm_set_chan_level(LED_PWM_SLICE, LED_PWM_CHAN, 0);             // init at 0.0 % doodie cycle

    // Init RS485
    Hardware_InitOutput(RS485_DE, 0);
    //uart_init(RS485_UART, 115200);
    //gpio_set_function(RS485_RX, GPIO_FUNC_UART);
    //gpio_set_function(RS485_TX, GPIO_FUNC_UART);

    // Init ADCs
    /*
    spi_init(ADC_SPI, 1 MHz);                                                               // max. SPI clock speed per datasheet: 25 Mhz (pg. 10, 11)
    spi_set_format(ADC_SPI, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);                      // datasheet pg. 11, 38
    gpio_set_function(ADC_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(ADC_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(ADC_MISO, GPIO_FUNC_SPI);
    */
    
    // Little oopsie on the PCB -> use PIO SPI instead
    uint pio_spi_offset = pio_add_program(adc_spi.pio, &spi_cpha1_program);
    pio_spi_init(adc_spi.pio, adc_spi.sm, pio_spi_offset, 8, 3.75, 1, 0, ADC_SCLK, ADC_MOSI, ADC_MISO);     // 120 MHz / 4 instr. / 3.75 = 8 MHz

    Hardware_InitOutput(ADC1_CS, 1);
    Hardware_InitOutput(ADC2_CS, 1);
    Hardware_InitOutput(ADC1_EN, 0);
    Hardware_InitOutput(ADC2_EN, 0);

    Hardware_InitInputPullup(ADC1_DRDY);
    Hardware_InitInputPullup(ADC2_DRDY);


    // Init MOSFET driver
    /*spi_init(HEATER_SPI, 1 MHz);
    gpio_set_function(ADC_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(ADC_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(ADC_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(ADC_SCLK, GPIO_FUNC_SPI);*/


    // Init fan and dimmer PWM (disabled for now)
    Hardware_InitOutput(HALFBRIDGE_EN, 0);
    Hardware_InitOutput(FAN1_PWM, 0);
    Hardware_InitOutput(FAN2_PWM, 0);
    Hardware_InitOutput(DIMMER1_PWM, 0);
    Hardware_InitOutput(DIMMER2_PWM, 0);

    // Init ESP32
    uart_init(ESP_UART, 115200);
    gpio_set_function(ESP_TX, GPIO_FUNC_UART);
    gpio_set_function(ESP_RX, GPIO_FUNC_UART);

    if (errors)
    {
        LED_SetFPS(500);
        return false;
    }

    return true;
}