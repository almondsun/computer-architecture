.syntax unified
.cpu cortex-m4
.thumb

#include "blink_app.inc"

.global blink_compute_half_period_ms
.type blink_compute_half_period_ms, %function

.section .text.blink_timing, "ax", %progbits
.align 2

/*
 * blink_compute_half_period_ms
 * Input : r0 = blink frequency [blinks/s].
 * Output: r0 = half-period [ms], saturated to >= 1 ms.
 * Notes : If input is 0, it is clamped to 1 to avoid divide-by-zero.
 */
blink_compute_half_period_ms:
    cmp     r0, #0
    bne     frequency_ok
    movs    r0, #1

frequency_ok:
    ldr     r1, =BLINK_HALF_PERIOD_NUMERATOR_MS
    udiv    r0, r1, r0
    cmp     r0, #0
    bne     half_period_ok
    movs    r0, #1

half_period_ok:
    bx      lr

.size blink_compute_half_period_ms, . - blink_compute_half_period_ms
