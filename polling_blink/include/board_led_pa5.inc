#ifndef BOARD_LED_PA5_INC
#define BOARD_LED_PA5_INC

#include "stm32l476_regs.inc"

/* Board-specific LED mapping (NUCLEO-L476RG: LED on PA5). */
.equ LED_GPIO_BASE,                  GPIOA_BASE
.equ LED_PIN_INDEX,                  5

/* Precomputed masks for PA5 GPIO configuration and control. */
.equ LED_MODER_Msk,                  (3 << (LED_PIN_INDEX * 2))
.equ LED_MODER_Output,               (1 << (LED_PIN_INDEX * 2))
.equ LED_OTYPER_Msk,                 (1 << LED_PIN_INDEX)
.equ LED_PUPDR_Msk,                  (3 << (LED_PIN_INDEX * 2))
.equ LED_ODR_Msk,                    (1 << LED_PIN_INDEX)
.equ LED_BSRR_Reset_Msk,             (1 << (LED_PIN_INDEX + 16))

#endif /* BOARD_LED_PA5_INC */
