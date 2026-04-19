#ifndef SCROLLING_SIGN_HW_H
#define SCROLLING_SIGN_HW_H

#include <stdint.h>

/**
 * @brief Boot the board support needed by the MFS display.
 */
void scrolling_sign_hw_init(void);

/**
 * @brief Read the monotonic millisecond counter used by the app loop.
 *
 * @return Milliseconds since boot.
 */
uint32_t scrolling_sign_hw_now_ms(void);

/**
 * @brief Refresh one multiplexed display digit with the requested segment mask.
 *
 * Segment bits use the logical layout `abcdefg.` in bits 0..7.
 *
 * @param digit_index Zero-based digit index in the range [0, 3].
 * @param segment_mask Logical segment mask for the selected digit.
 */
void scrolling_sign_hw_show_digit(uint8_t digit_index, uint8_t segment_mask);

/**
 * @brief Sleep until the next interrupt.
 */
void scrolling_sign_hw_wait(void);

#endif // SCROLLING_SIGN_HW_H
