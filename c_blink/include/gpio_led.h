#ifndef GPIO_LED_H
#define GPIO_LED_H

/*
 * Configures PA5 as push-pull output for the onboard LED.
 */
void gpio_led_init(void);

/*
 * Drives the LED output low (LED OFF on NUCLEO-L476RG).
 */
void gpio_led_off(void);

/*
 * Inverts the current LED output state.
 */
void gpio_led_toggle(void);

#endif /* GPIO_LED_H */
