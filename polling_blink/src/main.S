.syntax unified
.cpu cortex-m4
.thumb

.global main
.type main, %function

.extern gpio_led_init
.extern gpio_led_off
.extern gpio_led_toggle
.extern systick_init_1ms_polling
.extern systick_wait_1ms
.extern blink_compute_half_period_ms
.extern blink_frequency_hz

.section .text.main, "ax", %progbits
.align 2
main:
    /* Initialize hardware modules once. */
    bl      gpio_led_init
    bl      gpio_led_off
    bl      systick_init_1ms_polling

    /* r4: elapsed time since last toggle [ms]. */
    movs    r4, #0

main_loop:
    /* Poll SysTick for one 1 ms time step. */
    bl      systick_wait_1ms
    adds    r4, r4, #1

    /*
     * Load user frequency and convert it to the toggle period.
     * One blink = one full ON->OFF cycle.
     */
    ldr     r0, =blink_frequency_hz
    ldr     r0, [r0]
    bl      blink_compute_half_period_ms

    cmp     r4, r0
    blo     main_loop

    /* Period reached: toggle LED and restart elapsed counter. */
    bl      gpio_led_toggle
    movs    r4, #0
    b       main_loop

.size main, . - main
