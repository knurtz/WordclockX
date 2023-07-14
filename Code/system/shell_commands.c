#include <stdlib.h>
#include <string.h>

#include "pico/bootrom.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "hardware.h"
#include "shell_commands.h"

// Echo command
static void echo_cb(int arglen, char* argv)
{
    printf("%s\n", argv);
}

// Clocks command
static void clocks_cb(int arglen, char* argv)
{
    printf("System clock: %d kHz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
}

// Test arguments command
static void test_cb(int arglen, char* argv)
{
    printf("arglen: %d, argv: %s\n", arglen, argv);
}

// UF2 command
static void uf2_cb(int arglen, char* argv)
{
    reset_usb_boot(0, 2);
}

// I2C scan command
static void i2c_scan_cb(int arglen, char* argv)
{
    i2c_init(i2c0, 100 kHz);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}


// Assemble complete command set into a single array
ShellCommand command_set[] = {
    {"echo", echo_cb},
    {"clocks", clocks_cb},
    {"test", test_cb},
    {"uf2", uf2_cb},
    {"i2c scan", i2c_scan_cb}
};

size_t command_cnt = count_of(command_set);