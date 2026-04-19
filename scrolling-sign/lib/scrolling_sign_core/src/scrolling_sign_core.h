#ifndef SCROLLING_SIGN_CORE_H
#define SCROLLING_SIGN_CORE_H

#include <stddef.h>
#include <stdint.h>

enum {
    SCROLLING_SIGN_DIGIT_COUNT = 4U,
    SCROLLING_SIGN_SEG_A = 1U << 0,
    SCROLLING_SIGN_SEG_B = 1U << 1,
    SCROLLING_SIGN_SEG_C = 1U << 2,
    SCROLLING_SIGN_SEG_D = 1U << 3,
    SCROLLING_SIGN_SEG_E = 1U << 4,
    SCROLLING_SIGN_SEG_F = 1U << 5,
    SCROLLING_SIGN_SEG_G = 1U << 6,
    SCROLLING_SIGN_SEG_DP = 1U << 7,
    SCROLLING_SIGN_SCROLL_STEP_MS = 250U,
};

/**
 * @brief Logical 4-digit display frame.
 */
typedef struct {
    uint8_t digits[SCROLLING_SIGN_DIGIT_COUNT];
} scrolling_sign_frame_t;

/**
 * @brief Mutable state for the scrolling-sign renderer.
 *
 * `message` is borrowed for the lifetime of the core. Callers must keep the
 * pointed storage alive and immutable while the core is in use.
 */
typedef struct {
    const char *message;
    size_t message_length;
    size_t window_shift;
    uint32_t phase_ms;
    uint8_t ascii_font[128];
} scrolling_sign_core_t;

/**
 * @brief Initialize the scrolling-sign core.
 *
 * A null `message` is treated as an empty string.
 *
 * @param core Mutable core instance.
 * @param message Borrowed zero-terminated text to scroll.
 * @param now_ms Current monotonic time in milliseconds.
 */
void scrolling_sign_core_init(
        scrolling_sign_core_t *core,
        const char *message,
        uint32_t now_ms);

/**
 * @brief Advance the scroll position based on elapsed time.
 *
 * @param core Mutable core instance.
 * @param now_ms Current monotonic time in milliseconds.
 */
void scrolling_sign_core_tick(scrolling_sign_core_t *core, uint32_t now_ms);

/**
 * @brief Materialize the current 4-digit logical frame.
 *
 * Unsupported characters render as blank digits. ASCII lowercase letters are
 * normalized to uppercase approximations before encoding.
 *
 * @param core Immutable core instance.
 * @param frame Output frame.
 */
void scrolling_sign_core_frame(
        const scrolling_sign_core_t *core,
        scrolling_sign_frame_t *frame);

#endif // SCROLLING_SIGN_CORE_H
