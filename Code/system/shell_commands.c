#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "shell_commands.h"
#include "hardware.h"
#include "rtc.h"
#include "led_matrix.h"

// Numbers extracted from argument string
uint par[4];
// Extracts up to 4 numbers from argument string. Returns amount of numbers extracted.
static size_t ExtractParameters(char* args)
{
    size_t i;
    char* endstr = args;
    for (i = 0; i < 4 && *endstr; i++)
    {
        par[i] = strtol(endstr, &endstr, 0);
    }

    return i;
}

// Echo command
static void echo_cb(int arglen, char* argv)
{
    printf("%s\n\n", argv);
}

// Clocks command
static void clocks_cb(int arglen, char* argv)
{
    printf("System clock: %d kHz\n\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
}

// Test arguments command
static void test_cb(int arglen, char* argv)
{
    printf("arglen: %d, argv: %s\n", arglen, argv);
    printf("Extracted %d numbers: 0x%x 0x%x 0x%x 0x%x\n\n", ExtractParameters(argv), par[0], par[1], par[2], par[3]);
}

// UF2 command
static void uf2_cb(int arglen, char* argv)
{
    reset_usb_boot(0, 2);
}

// I2C scan command
static void i2c_scan_cb(int arglen, char* argv)
{
    printf("I2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.
        int ret;
        uint8_t rxdata;
        ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n\n");
}

// I2C read register command
static void i2c_read_cb(int arglen, char* argv)
{
    // Arguments: i2c read <addr> <reg> [<len>]
    size_t num = ExtractParameters(argv);
    if (num < 2)
    {
        printf("Missing parameters\n\n");
        return;
    }

    // Write register address to slave
    uint8_t addr =  par[0];
    uint8_t reg = par[1];
    bool error = false;
    if (i2c_write_blocking(i2c0, addr, &reg, 1, true) < 0) error = true;
    // Read from slave
    uint8_t reg_value;
    if (i2c_read_blocking(i2c0, addr, &reg_value, 1, false) < 0) error = true;

    printf("Read value: 0x%02x%s\n\n", reg_value, error ? " (error)" : "");
}

// I2C write register command
static void i2c_write_cb(int arglen, char* argv)
{
    // Arguments: i2c write <addr> <reg> <value>
    size_t num = ExtractParameters(argv);
    if (num < 3)
    {
        printf("Missing parameters\n\n");
        return;
    }

    uint8_t addr =  par[0];
    uint8_t reg_val[2] = {par[1], par[2]};
    bool error = false;
    if (i2c_write_blocking(i2c0, addr, reg_val, 2, false) < 0) error = true;

    printf("Wrote register%s\n\n", error ? " (error)" : "");
}

// RTC set time command
static void rtc_set_cb(int arglen, char* argv)
{
    // Arguments: rtc set <hours> <minutes> <seconds>
    size_t num = ExtractParameters(argv);
    if (num < 3)
    {
        printf("Missing parameters\n\n");
        return;
    }
    char time_str[9];
    Time t = {par[0], par[1], par[2]};
    RTC_PrintTime(&t, time_str, 9);

    if (RTC_SetTime(t)) printf("Successfully set time to %s\n\n", time_str);
    else printf("Failed to set time to %s\n\n", time_str);
}

// RTC get time command
static void rtc_get_cb(int arglen, char* argv)
{
    Time t;
    char time_str[9];

    bool success = RTC_GetTime(&t);
    RTC_PrintTime(&t, time_str, 9);

    if (success) printf("Time is %s\n\n", time_str);
    else printf("Failed to get time\n\n");
}

// RTC power fail stats command
static void rtc_pwrfail_cb(int arglen, char* argv)
{
    Time t_pd, t_pu;
    char pd_str[9], pu_str[9];

    bool success = RTC_GetPDPUTime(&t_pd, &t_pu);
    RTC_PrintTime(&t_pd, pd_str, 9);
    RTC_PrintTime(&t_pu, pu_str, 9);

    if (success) printf("Powerdown at %s, power restored at %s\n\n", pd_str, pu_str);
    else printf("Failed to get pd / pu times\n\n");
}

static void set_row_cb(int arglen, char* argv)
{
    // Arguments: set row <row_num>
    size_t num = ExtractParameters(argv);
    if (num < 1)
    {
        LEDMatrix_DeselectRow();
        return;
    }
    LEDMatrix_SelectRow(par[0]);
}

static void set_col_cb(int arglen, char* argv)
{
    // Arguments: set col <col_num> <r> <g> <b>
    size_t num = ExtractParameters(argv);
    if (num < 4)
    {
        printf("Missing parameters\n\n");
        return;
    }
    LEDMatrix_SelectCol(par[0], par[1], par[2], par[3]);
}

// Assemble complete command set into a single array
ShellCommand command_set[] = {
    {"echo", echo_cb},
    {"clocks", clocks_cb},
    {"test", test_cb},
    {"uf2", uf2_cb},
    {"i2c scan", i2c_scan_cb},
    {"i2c read", i2c_read_cb},
    {"i2c write", i2c_write_cb},
    {"rtc set", rtc_set_cb},
    {"rtc get", rtc_get_cb},
    {"rtc pwrfail", rtc_pwrfail_cb},
    {"set row", set_row_cb},
    {"set col", set_col_cb}
};

size_t command_cnt = count_of(command_set);