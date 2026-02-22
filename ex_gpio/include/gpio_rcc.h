#ifndef GPIO_RCC_H
#define GPIO_RCC_H

/* RCC base and AHB2 peripheral clock enable register. */
#define RCC_BASE                0x40021000
#define RCC_AHB2ENR             (RCC_BASE + 0x4C)
#define RCC_AHB2ENR_GPIOAEN     (1 << 0)
#define RCC_AHB2ENR_GPIOCEN     (1 << 2)

/* GPIO ports used by the NUCLEO-L476RG board (LED on PA5, button on PC13). */
#define GPIOA_BASE              0x48000000
#define GPIOC_BASE              0x48000800

/* Common GPIO register offsets. */
#define GPIO_MODER_OFFSET       0x00
#define GPIO_IDR_OFFSET         0x10
#define GPIO_ODR_OFFSET         0x14

/* Registers for PA5 LED control. */
#define GPIOA_MODER             (GPIOA_BASE + GPIO_MODER_OFFSET)
#define GPIOA_ODR               (GPIOA_BASE + GPIO_ODR_OFFSET)

/* Registers for PC13 button input. */
#define GPIOC_MODER             (GPIOC_BASE + GPIO_MODER_OFFSET)
#define GPIOC_IDR               (GPIOC_BASE + GPIO_IDR_OFFSET)

/* Pin indices and bit masks. */
#define GPIO_PIN5               5
#define GPIO_PIN13              13
#define GPIO_2BIT_MASK          0x3

/* MODER field masks and values. */
#define GPIOA_MODER_PIN5_MASK   (GPIO_2BIT_MASK << (GPIO_PIN5 * 2))
#define GPIOA_MODER_PIN5_OUTPUT (0x1 << (GPIO_PIN5 * 2))
#define GPIOC_MODER_PIN13_MASK  (GPIO_2BIT_MASK << (GPIO_PIN13 * 2))

/* ODR/IDR single-bit masks. */
#define GPIOA_ODR_PIN5          (1 << GPIO_PIN5)
#define GPIOC_IDR_PIN13         (1 << GPIO_PIN13)

#endif /* GPIO_RCC_H */
