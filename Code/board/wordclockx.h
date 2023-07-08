/* 
 * PowerControl 2023 board file for Pico SDK
 *
 * Based on pico.h by Raspberry Pi (Trading) Ltd.
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// -----------------------------------------------------
// NOTE: THIS HEADER IS ALSO INCLUDED BY ASSEMBLER SO
//       SHOULD ONLY CONSIST OF PREPROCESSOR DIRECTIVES
// -----------------------------------------------------

#ifndef POWER_CTRL_2023_H
#define POWER_CTRL_2023_H

// For board detection
#define POWER_CTRL_2023

// --- UART ---
#define PICO_STDIO_DEFAULT_CRLF                 0

// --- FLASH ---
#define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H     1

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES                   (2 * 1024 * 1024)
#endif

#endif /* POWER_CTRL_2023_H */
