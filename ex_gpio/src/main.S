.syntax unified
.cpu cortex-m4
.thumb

#include "gpio_rcc.h"

.section .bss
.align 2
.global g_button_state
g_button_state:
    .space 4

.section .text
.align 2

.global main
.type main, %function
main:
    /* Configure GPIO clocks and modes once at startup. */
    bl gpio_init

super_loop:
    /* Super loop with decoupled input read and output update steps. */
    bl read_button
    bl update_led
    b super_loop
.size main, . - main

.global gpio_init
.type gpio_init, %function
gpio_init:
    /* Enable GPIOA and GPIOC clocks through RCC AHB2ENR. */
    ldr r0, =RCC_AHB2ENR
    ldr r1, [r0]
    ldr r2, =(RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN)
    orr r1, r1, r2
    str r1, [r0]

    /* Configure PA5 MODER bits as general-purpose output (01). */
    ldr r0, =GPIOA_MODER
    ldr r1, [r0]
    ldr r2, =GPIOA_MODER_PIN5_MASK
    bic r1, r1, r2
    ldr r2, =GPIOA_MODER_PIN5_OUTPUT
    orr r1, r1, r2
    str r1, [r0]

    /* Configure PC13 MODER bits as input (00). */
    ldr r0, =GPIOC_MODER
    ldr r1, [r0]
    ldr r2, =GPIOC_MODER_PIN13_MASK
    bic r1, r1, r2
    str r1, [r0]

    bx lr
.size gpio_init, . - gpio_init

.global read_button
.type read_button, %function
read_button:
    /* Sample PC13 and collapse its state to 0/1 in the global variable. */
    ldr r0, =GPIOC_IDR
    ldr r1, [r0]
    ldr r2, =GPIOC_IDR_PIN13
    and r1, r1, r2
    lsrs r1, r1, #GPIO_PIN13

    ldr r0, =g_button_state
    str r1, [r0]

    bx lr
.size read_button, . - read_button

.global update_led
.type update_led, %function
update_led:
    /* Read cached button state and map 0->LED off, 1->LED on. */
    ldr r0, =g_button_state
    ldr r1, [r0]

    ldr r0, =GPIOA_ODR
    ldr r2, [r0]
    ldr r3, =GPIOA_ODR_PIN5
    cmp r1, #0
    beq led_off

led_on:
    orr r2, r2, r3
    b led_write

led_off:
    bic r2, r2, r3

led_write:
    str r2, [r0]
    bx lr
.size update_led, . - update_led
