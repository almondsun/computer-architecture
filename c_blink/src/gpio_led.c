#include "gpio_led.h"

#include <stdint.h>

#include "stm32l4xx.h"

/* NUCLEO-L476RG user LED is connected to GPIOA pin 5. */
enum
{
    LED_PIN_INDEX = 5u,
    LED_PIN_MODE_SHIFT = (LED_PIN_INDEX * 2u),
    LED_PIN_MODE_MASK = (3u << LED_PIN_MODE_SHIFT),
    LED_PIN_MODE_OUTPUT = (1u << LED_PIN_MODE_SHIFT),
    LED_PIN_MASK = (1u << LED_PIN_INDEX),
    LED_PIN_RESET_MASK = (1u << (LED_PIN_INDEX + 16u))
};

void gpio_led_init(void)
{
    /* Enable GPIOA clock before accessing its configuration registers. */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void)RCC->AHB2ENR;

    /* Select output mode for PA5 while preserving the rest of GPIOA setup. */
    GPIOA->MODER &= ~LED_PIN_MODE_MASK;
    GPIOA->MODER |= LED_PIN_MODE_OUTPUT;

    /* Force push-pull output and disable pull-up/pull-down resistors. */
    GPIOA->OTYPER &= ~LED_PIN_MASK;
    GPIOA->PUPDR &= ~LED_PIN_MODE_MASK;
}

void gpio_led_off(void)
{
    GPIOA->BSRR = LED_PIN_RESET_MASK;
}

void gpio_led_toggle(void)
{
    GPIOA->ODR ^= LED_PIN_MASK;
}
