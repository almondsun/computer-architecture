.syntax unified
.cpu cortex-m4
.thumb

.global main
.type main, %function

.extern gpio_led_init
.extern gpio_led_off
.extern gpio_led_toggle
.extern systick_init_1ms_interrupt
.extern systick_wait_for_interrupt
.extern systick_consume_elapsed_ms
.extern blink_compute_half_period_ms
.extern blink_frequency_hz

.section .text.main, "ax", %progbits
.align 2
main:
    /* Initialize hardware modules once before entering the control loop. */
    bl      gpio_led_init
    bl      gpio_led_off
    bl      systick_init_1ms_interrupt

    /* r4: elapsed time since last LED toggle [ms]. */
    movs    r4, #0

main_loop:
    /*
     * Sleep until an interrupt arrives instead of polling SysTick flags.
     * SysTick interrupt updates elapsed milliseconds in the background.
     */
    bl      systick_wait_for_interrupt
    bl      systick_consume_elapsed_ms
    cmp     r0, #0
    beq     main_loop
    adds    r4, r4, r0

    /* Load user frequency [Hz] and convert it to toggle half-period [ms]. */
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
