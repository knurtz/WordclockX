cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Set custom board file
set(PICO_BOARD_HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/board")
set(PICO_BOARD wordclockx)

# Import Pico SDK
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
#include(pico_extras_import.cmake)

project(WordclockX C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add all source code files
add_executable(WordclockX    
    main.c

    hardware/hardware.c
    hardware/led.c
    hardware/led_matrix.c
    hardware/rtc.c

    system/shell.c
    system/shell_commands.c
    system/clock.c

    system/fast_hsv2rgb.c
)

target_compile_definitions(WordclockX PRIVATE
#   PICO_DEFAULT_UART=0
#   PICO_DEFAULT_UART_TX_PIN=28
#   PICO_DEFAULT_UART_RX_PIN=29
)

target_include_directories(WordclockX PRIVATE
    .
    system
    hardware
)

# Import pico_stdlib which imports commonly used features
target_link_libraries(WordclockX 
    pico_stdlib
    hardware_i2c
    hardware_pwm
    hardware_uart
    hardware_timer
    hardware_dma
    hardware_clocks
    hardware_pio
)

# Enable a summary of used memory at the end of linking process
#target_link_options(WordclockX PUBLIC "-Wl,--print-memory-usage,--no-warn-rwx-segments")
target_link_options(WordclockX PUBLIC "-Wl,--print-memory-usage")
add_custom_command(TARGET WordclockX
    POST_BUILD
    COMMENT "Code size:"
    COMMAND arm-none-eabi-size WordclockX.elf
    VERBATIM
)

pico_enable_stdio_uart(WordclockX 0)
pico_enable_stdio_usb(WordclockX 1)

# Create map/bin/hex file etc.
pico_add_extra_outputs(WordclockX)