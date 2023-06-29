#include "shell_commands.h"

#include <stdlib.h>
#include <string.h>

#include "hardware/clocks.h"
#include "pico/bootrom.h"

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

// Test command
static void test_cb(int arglen, char* argv)
{
    printf("arglen: %d, argv: %s\n", arglen, argv);
}

// UF2 command
static void uf2_cb(int arglen, char* argv)
{
    reset_usb_boot(0, 2);
}

// Assemble complete command set into a single array
ShellCommand command_set[] = {
    {"echo", echo_cb},
    {"clocks", clocks_cb},
    {"test", test_cb},
    {"uf2", uf2_cb}
};
size_t command_cnt = count_of(command_set);