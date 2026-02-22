.syntax unified
.cpu cortex-m4
.thumb

#include "stm32l476_regs.inc"

.extern SystemCoreClock
.extern SystemCoreClockUpdate

.global systick_init_1ms_polling
.global systick_wait_1ms

.type systick_init_1ms_polling, %function
.type systick_wait_1ms, %function

.section .text.systick_polling, "ax", %progbits
.align 2

/*
 * systick_init_1ms_polling
 * Purpose: Configure SysTick for a 1 ms period and polling-only operation.
 * Side effects: Writes SysTick LOAD/VAL/CTRL, updates SystemCoreClock.
 */
systick_init_1ms_polling:
    push    {lr}

    /* Refresh SystemCoreClock so timing uses the active clock tree. */
    bl      SystemCoreClockUpdate

    ldr     r0, =SystemCoreClock
    ldr     r0, [r0]                    /* SystemCoreClock [Hz]. */
    ldr     r1, =1000
    udiv    r0, r0, r1                  /* Ticks per millisecond. */
    cmp     r0, #0
    beq     systick_config_error

    subs    r0, r0, #1
    ldr     r1, =SYSTICK_LOAD_MAX
    cmp     r0, r1
    bhi     systick_config_error

    ldr     r1, =SYSTICK_BASE
    str     r0, [r1, #SYSTICK_LOAD_OFFSET]
    movs    r0, #0
    str     r0, [r1, #SYSTICK_VAL_OFFSET]
    movs    r0, #(SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_ENABLE)
    str     r0, [r1, #SYSTICK_CTRL_OFFSET]

    pop     {pc}

systick_config_error:
    b       systick_config_error

.size systick_init_1ms_polling, . - systick_init_1ms_polling

/*
 * systick_wait_1ms
 * Purpose: Busy-wait until SysTick COUNTFLAG indicates a 1 ms tick elapsed.
 */
systick_wait_1ms:
    ldr     r1, =SYSTICK_BASE
    ldr     r2, =SYSTICK_CTRL_COUNTFLAG

systick_wait_loop:
    ldr     r0, [r1, #SYSTICK_CTRL_OFFSET]
    tst     r0, r2
    beq     systick_wait_loop
    bx      lr

.size systick_wait_1ms, . - systick_wait_1ms
