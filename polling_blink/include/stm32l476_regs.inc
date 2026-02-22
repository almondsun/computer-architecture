#ifndef STM32L476_REGS_INC
#define STM32L476_REGS_INC

/* RCC registers and bits (STM32L476). */
.equ RCC_AHB2ENR,                    0x4002104C
.equ RCC_AHB2ENR_GPIOAEN,            (1 << 0)

/* GPIO register layout (offsets from GPIOx base). */
.equ GPIOA_BASE,                     0x48000000
.equ GPIO_MODER_OFFSET,              0x00
.equ GPIO_OTYPER_OFFSET,             0x04
.equ GPIO_PUPDR_OFFSET,              0x0C
.equ GPIO_ODR_OFFSET,                0x14
.equ GPIO_BSRR_OFFSET,               0x18

/* SysTick registers and bits. */
.equ SYSTICK_BASE,                   0xE000E010
.equ SYSTICK_CTRL_OFFSET,            0x00
.equ SYSTICK_LOAD_OFFSET,            0x04
.equ SYSTICK_VAL_OFFSET,             0x08
.equ SYSTICK_CTRL_ENABLE,            (1 << 0)
.equ SYSTICK_CTRL_CLKSOURCE,         (1 << 2)
.equ SYSTICK_CTRL_COUNTFLAG,         (1 << 16)
.equ SYSTICK_LOAD_MAX,               0x00FFFFFF

#endif /* STM32L476_REGS_INC */
