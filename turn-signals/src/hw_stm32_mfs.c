#include <stdbool.h>
#include <stdint.h>

#include "stm32l476xx.h"
#include "ts_hw.h"

enum {
    HW_LED_ACTIVE_LOW = 1U,
    HW_BTN_ACTIVE_LOW = 1U,
    HW_BTN_DEBOUNCE_MS = 20U,
};

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin_no;
    uint16_t pin_mask;
} hw_pin_t;

typedef struct {
    hw_pin_t pin;
    bool stable;
    bool sample;
    uint32_t edge_ms;
} hw_btn_t;

static volatile uint32_t g_ms = 0U;

static const hw_pin_t k_leds[4] = {
    {GPIOA, 5U, 1U << 5}, // D13 / LED1
    {GPIOA, 6U, 1U << 6}, // D12 / LED2
    {GPIOA, 7U, 1U << 7}, // D11 / LED3
    {GPIOB, 6U, 1U << 6}, // D10 / LED4
};

static hw_btn_t g_btn_left = {{GPIOA, 1U, 1U << 1}, false, false, 0U};
static hw_btn_t g_btn_mid = {{GPIOA, 4U, 1U << 4}, false, false, 0U};
static hw_btn_t g_btn_right = {{GPIOB, 0U, 1U << 0}, false, false, 0U};

void SysTick_Handler(void)
{
    ++g_ms;
}

static void hw_gpio_out(hw_pin_t pin)
{
    const uint32_t shift = (uint32_t)pin.pin_no * 2U;
    const uint32_t mask = 0x3UL << shift;

    pin.port->MODER = (pin.port->MODER & ~mask) | (0x1UL << shift);
    pin.port->OTYPER &= ~(0x1UL << pin.pin_no);
    pin.port->OSPEEDR &= ~mask;
    pin.port->PUPDR &= ~mask;
}

static void hw_gpio_in_pullup(hw_pin_t pin)
{
    const uint32_t shift = (uint32_t)pin.pin_no * 2U;
    const uint32_t mask = 0x3UL << shift;

    pin.port->MODER &= ~mask;
    pin.port->PUPDR = (pin.port->PUPDR & ~mask) | (0x1UL << shift);
}

static bool hw_btn_is_pressed(hw_pin_t pin)
{
    const bool pin_high = (pin.port->IDR & pin.pin_mask) != 0U;

    return (HW_BTN_ACTIVE_LOW != 0U) ? !pin_high : pin_high;
}

static bool hw_btn_edge(hw_btn_t *btn, uint32_t now_ms)
{
    const bool sample = hw_btn_is_pressed(btn->pin);

    if (sample != btn->sample) {
        btn->sample = sample;
        btn->edge_ms = now_ms;
    }

    if ((uint32_t)(now_ms - btn->edge_ms) < HW_BTN_DEBOUNCE_MS) {
        return false;
    }

    if (btn->stable == btn->sample) {
        return false;
    }

    btn->stable = btn->sample;
    return btn->stable;
}

static void hw_led_write(hw_pin_t pin, bool on)
{
    if (HW_LED_ACTIVE_LOW != 0U) {
        pin.port->BSRR = on ? ((uint32_t)pin.pin_mask << 16U)
                            : (uint32_t)pin.pin_mask;
        return;
    }

    pin.port->BSRR = on ? (uint32_t)pin.pin_mask
                        : ((uint32_t)pin.pin_mask << 16U);
}

void ts_hw_init(void)
{
    uint32_t i;

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN;

    for (i = 0U; i < 4U; ++i) {
        hw_gpio_out(k_leds[i]);
    }

    hw_gpio_in_pullup(g_btn_left.pin);
    hw_gpio_in_pullup(g_btn_mid.pin);
    hw_gpio_in_pullup(g_btn_right.pin);

    ts_hw_apply_leds(0U);

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000U);
}

uint32_t ts_hw_now_ms(void)
{
    return g_ms;
}

ts_btns_t ts_hw_poll_btns(uint32_t now_ms)
{
    ts_btns_t btns = {false, false, false};

    btns.left = hw_btn_edge(&g_btn_left, now_ms);
    btns.middle = hw_btn_edge(&g_btn_mid, now_ms);
    btns.right = hw_btn_edge(&g_btn_right, now_ms);

    return btns;
}

void ts_hw_apply_leds(uint8_t led_mask)
{
    uint32_t i;

    for (i = 0U; i < 4U; ++i) {
        const bool on = (led_mask & (1U << i)) != 0U;
        hw_led_write(k_leds[i], on);
    }
}

void ts_hw_wait(void)
{
    __WFI();
}
