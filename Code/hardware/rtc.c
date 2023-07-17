#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "hardware.h"
#include "rtc.h"
#include "rtc_registers.h"


static bool RTC_ReadRegister(uint8_t reg, uint8_t* val)
{
    if (i2c_write_blocking(i2c0, RTC_ADDR, &reg, 1, true) < 1) return false;
    if (i2c_read_blocking(i2c0, RTC_ADDR, val, 1, false) < 1) return false;

    return true;
}

static bool RTC_WriteRegister(uint8_t reg, uint8_t val)
{
    uint8_t reg_val[2] = {reg, val};
    if (i2c_write_blocking(i2c0, RTC_ADDR, reg_val, 2, false) < 2) return false;
    return true;
}

bool RTC_Init(void)
{
    uint8_t reg;

    // Detect unconfigured RTC by checking oscillator enable bit
    if (!RTC_ReadRegister(RTCSEC, &reg)) return false;
    if (reg >= (1 << RTCSEC_ST)) return true;
    
    // Enable oscillator without changing seconds counter
    reg |= (1 << RTCSEC_ST);
    if (!RTC_WriteRegister(RTCSEC, reg)) return false;

    // Set to 24h mode without changing hours counter (just in case)
    if (!RTC_ReadRegister(RTCHOUR, &reg)) return false;
    reg &= ~(0 << RTCHOUR_1224);
    if (!RTC_WriteRegister(RTCHOUR, reg)) return false;

    // Enable VBAT
    if (!RTC_ReadRegister(RTCWKDAY, &reg)) return false;
    reg |= (1 << RTCWKDAY_VBATEN);
    if (!RTC_WriteRegister(RTCWKDAY, reg)) return false;

    return true;
}

bool RTC_SetTime(Time t)
{
    if (t.seconds > 60 || t.minutes > 60 || t.hours > 24) return false;

    uint8_t reg_val[4];
    reg_val[0] = 0x00;      // start writing at register 0x00
    reg_val[1] = (1 << RTCSEC_ST) | (t.seconds / 10 << 4) | (t.seconds % 10);
    reg_val[2] = (t.minutes / 10 << 4) | (t.minutes % 10);
    reg_val[3] = (t.hours / 10 << 4) | (t.hours % 10);

    if (i2c_write_blocking(i2c0, RTC_ADDR, reg_val, 4, false) < 4) return false;

    return true;
}

bool RTC_GetTime(Time* t)
{
    uint8_t values[3];
    uint8_t reg = RTCSEC;
    if (i2c_write_blocking(i2c0, RTC_ADDR, &reg, 1, true) < 1) return false;
    if (i2c_read_blocking(i2c0, RTC_ADDR, values, 3, false) < 3) return false;

    t->seconds = 10 * (values[0] >> 4 & 0b111) + (values[0] & 0b1111);
    t->minutes = 10 * (values[1] >> 4 & 0b111) + (values[1] & 0b1111);
    t->hours = 10 * (values[2] >> 4 & 0b11) + (values[2] & 0b1111);

    return true;
}

// Returns false on communication fail or if no power fail was stored since last request
bool RTC_GetPDPUTime(Time* t_pd, Time* t_pu)
{
    uint8_t wkday_reg;
    if (!RTC_ReadRegister(RTCWKDAY, &wkday_reg)) return false;
    if (!(wkday_reg & (1 << RTCWKDAY_PWRFAIL))) return false;

    uint8_t values[3];
    uint8_t reg = 0x18;     // Register PWRDNMIN
    if (i2c_write_blocking(i2c0, RTC_ADDR, &reg, 1, true) < 1) return false;
    if (i2c_read_blocking(i2c0, RTC_ADDR, values, 2, false) < 2) return false;

    t_pd->seconds = 0;
    t_pd->minutes = 10 * (values[0] >> 4 & 0b111) + (values[0] & 0b1111);
    t_pd->hours = 10 * (values[1] >> 4 & 0b11) + (values[1] & 0b1111);
    
    reg = 0x1c;             // Register PWRUPMIN
    if (i2c_write_blocking(i2c0, RTC_ADDR, &reg, 1, true) < 1) return false;
    if (i2c_read_blocking(i2c0, RTC_ADDR, values, 2, false) < 2) return false;

    t_pu->seconds = 0;
    t_pu->minutes = 10 * (values[0] >> 4 & 0b111) + (values[0] & 0b1111);
    t_pu->hours = 10 * (values[1] >> 4 & 0b11) + (values[1] & 0b1111);

    // Reset PWRFAIL flag
    reg |= (1 << RTCWKDAY_VBATEN);
    if (!RTC_WriteRegister(RTCWKDAY, reg)) return false;

    return true;
}

void RTC_PrintTime(Time* t, char* s, size_t strlen)
{
    if (t->seconds > 60 || t->minutes > 60 || t->hours > 24) snprintf(s, strlen, "invalid");
    else snprintf(s, strlen, "%02d:%02d:%02d", t->hours, t->minutes, t->seconds);
}