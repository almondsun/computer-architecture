.syntax unified
.cpu cortex-m4
.thumb

#include "board_led_pa5.inc"

.global gpio_led_init
.global gpio_led_off
.global gpio_led_toggle

.type gpio_led_init, %function
.type gpio_led_off, %function
.type gpio_led_toggle, %function

.section .text.gpio_led, "ax", %progbits
.align 2

/*
 * gpio_led_init
 * Purpose: Configure PA5 as push-pull output with no pull resistors.
 * Side effects: Enables GPIOA clock and updates GPIOA mode/type/pull registers.
 */
gpio_led_init:
    /* Enable GPIOA clock before touching GPIO registers. */
    ldr     r0, =RCC_AHB2ENR
    ldr     r1, [r0]
    ldr     r2, =RCC_AHB2ENR_GPIOAEN
    orr     r1, r1, r2
    str     r1, [r0]
    ldr     r1, [r0]                    /* Read-after-write synchronization. */

    ldr     r0, =LED_GPIO_BASE

    /* Select output mode for LED pin. */
    ldr     r1, [r0, #GPIO_MODER_OFFSET]
    ldr     r2, =LED_MODER_Msk
    bic     r1, r1, r2
    ldr     r2, =LED_MODER_Output
    orr     r1, r1, r2
    str     r1, [r0, #GPIO_MODER_OFFSET]

    /* Force push-pull output type. */
    ldr     r1, [r0, #GPIO_OTYPER_OFFSET]
    ldr     r2, =LED_OTYPER_Msk
    bic     r1, r1, r2
    str     r1, [r0, #GPIO_OTYPER_OFFSET]

    /* Disable pull-up/pull-down resistors for this pin. */
    ldr     r1, [r0, #GPIO_PUPDR_OFFSET]
    ldr     r2, =LED_PUPDR_Msk
    bic     r1, r1, r2
    str     r1, [r0, #GPIO_PUPDR_OFFSET]

    bx      lr

.size gpio_led_init, . - gpio_led_init

/*
 * gpio_led_off
 * Purpose: Drive PA5 low, turning the LED off on this board.
 */
gpio_led_off:
    ldr     r0, =LED_GPIO_BASE
    ldr     r1, =LED_BSRR_Reset_Msk
    str     r1, [r0, #GPIO_BSRR_OFFSET]
    bx      lr

.size gpio_led_off, . - gpio_led_off

/*
 * gpio_led_toggle
 * Purpose: Invert LED output state by toggling ODR bit.
 */
gpio_led_toggle:
    ldr     r0, =LED_GPIO_BASE
    ldr     r1, [r0, #GPIO_ODR_OFFSET]
    eor     r1, r1, #LED_ODR_Msk
    str     r1, [r0, #GPIO_ODR_OFFSET]
    bx      lr

.size gpio_led_toggle, . - gpio_led_toggle
