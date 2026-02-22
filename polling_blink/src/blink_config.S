.syntax unified
.cpu cortex-m4
.thumb

#include "blink_app.inc"

.global blink_frequency_hz

.section .data.blink_config, "aw", %progbits
.align 2

/*
 * User-configurable blink frequency [blinks/s].
 * Change this value (or write to this symbol at runtime) to adjust blink rate.
 */
blink_frequency_hz:
    .word BLINK_DEFAULT_FREQUENCY_HZ
