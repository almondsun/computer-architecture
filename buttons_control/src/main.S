.syntax unified
.cpu cortex-m4
.thumb

#include "stm32l476rg_shield_map.inc"

.section .text.main
.global main
.type main, %function

main:
    bl gpio_initialize

main_loop:
    bl update_leds_from_buttons
    b main_loop

.type gpio_initialize, %function
gpio_initialize:
    push {lr}

    /* Enable GPIOA and GPIOB peripheral clocks */
    ldr r0, =RCC_AHB2ENR
    ldr r1, [r0]
    ldr r2, =RCC_AHB2ENR_GPIOABEN_MASK
    orr r1, r1, r2
    str r1, [r0]

    /* Configure LED pins as outputs */
    ldr r0, =GPIOA_MODER
    ldr r1, [r0]
    ldr r2, =GPIOA_MODER_LED_CLEAR_MASK
    bic r1, r1, r2
    ldr r2, =GPIOA_MODER_LED_OUTPUT_MASK
    orr r1, r1, r2
    str r1, [r0]

    /* Configure button pins as inputs */
    ldr r0, =GPIOA_MODER
    ldr r1, [r0]
    ldr r2, =GPIOA_MODER_BTN_CLEAR_MASK
    bic r1, r1, r2
    str r1, [r0]

    ldr r0, =GPIOB_MODER
    ldr r1, [r0]
    ldr r2, =GPIOB_MODER_BTN_CLEAR_MASK
    bic r1, r1, r2
    str r1, [r0]

    /* Use pull-ups so a pressed button reads LOW (active-low) */
    ldr r0, =GPIOA_PUPDR
    ldr r1, [r0]
    ldr r2, =GPIOA_PUPDR_BTN_CLEAR_MASK
    bic r1, r1, r2
    ldr r2, =GPIOA_PUPDR_BTN_PULLUP_MASK
    orr r1, r1, r2
    str r1, [r0]

    ldr r0, =GPIOB_PUPDR
    ldr r1, [r0]
    ldr r2, =GPIOB_PUPDR_BTN_CLEAR_MASK
    bic r1, r1, r2
    ldr r2, =GPIOB_PUPDR_BTN_PULLUP_MASK
    orr r1, r1, r2
    str r1, [r0]

    /* Start with all LEDs off */
    ldr r0, =GPIOA_BSRR
    ldr r1, =GPIOA_LED_OFF_ALL_MASK
    str r1, [r0]

    pop {pc}

.type update_leds_from_buttons, %function
update_leds_from_buttons:
    push {lr}

    /* S1-A1 controls D1 on GPIOA */
    ldr r0, =GPIOA_IDR
    ldr r1, [r0]
    tst r1, #BTN_S1_A1_MASK
    ldr r0, =GPIOA_BSRR
    beq s1_pressed
s1_released:
    ldr r2, =LED_D1_OFF_MASK
    b s1_write
s1_pressed:
    ldr r2, =LED_D1_ON_MASK
s1_write:
    str r2, [r0]

    /* S2-A2 controls D2 on GPIOA */
    ldr r0, =GPIOA_IDR
    ldr r1, [r0]
    tst r1, #BTN_S2_A2_MASK
    ldr r0, =GPIOA_BSRR
    beq s2_pressed
s2_released:
    ldr r2, =LED_D2_OFF_MASK
    b s2_write
s2_pressed:
    ldr r2, =LED_D2_ON_MASK
s2_write:
    str r2, [r0]

    /* S3-A3 controls D3 on GPIOA */
    ldr r0, =GPIOB_IDR
    ldr r1, [r0]
    tst r1, #BTN_S3_A3_MASK
    ldr r0, =GPIOA_BSRR
    beq s3_pressed
s3_released:
    ldr r2, =LED_D3_OFF_MASK
    b s3_write
s3_pressed:
    ldr r2, =LED_D3_ON_MASK
s3_write:
    str r2, [r0]

    pop {pc}

.size main, .-main
