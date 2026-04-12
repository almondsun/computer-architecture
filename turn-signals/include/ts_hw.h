#ifndef TS_HW_H
#define TS_HW_H

#include <stdint.h>

#include "ts_core.h"

/**
 * @brief Initialize the selected hardware adapter.
 */
void ts_hw_init(void);

/**
 * @brief Read the current monotonic board time.
 *
 * @return Monotonic time in milliseconds.
 */
uint32_t ts_hw_now_ms(void);

/**
 * @brief Poll debounced press edges from the hardware buttons.
 *
 * @param now_ms Current monotonic time in milliseconds.
 *
 * @return Press edges captured since the previous poll.
 */
ts_btns_t ts_hw_poll_btns(uint32_t now_ms);

/**
 * @brief Apply the logical LED mask to the physical board.
 *
 * @param led_mask Logical LED mask produced by the core.
 */
void ts_hw_apply_leds(uint8_t led_mask);

/**
 * @brief Yield until the next relevant board event.
 */
void ts_hw_wait(void);

#endif // TS_HW_H
