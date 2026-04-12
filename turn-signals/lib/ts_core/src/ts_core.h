#ifndef TS_CORE_H
#define TS_CORE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Logical operating modes for the turn-signal controller.
 */
typedef enum {
    TS_MODE_NEUTRAL = 0,
    TS_MODE_LEFT,
    TS_MODE_RIGHT,
    TS_MODE_PARKING,
} ts_mode_t;

enum {
    TS_LED_1 = 1U << 0,
    TS_LED_2 = 1U << 1,
    TS_LED_3 = 1U << 2,
    TS_LED_4 = 1U << 3,
    TS_LED_LEFT = TS_LED_1 | TS_LED_2,
    TS_LED_RIGHT = TS_LED_3 | TS_LED_4,
    TS_LED_PARK = TS_LED_LEFT | TS_LED_RIGHT,
    TS_BLINK_MS = 500U,
};

/**
 * @brief Debounced button press edges for one polling cycle.
 */
typedef struct {
    bool left;
    bool middle;
    bool right;
} ts_btns_t;

/**
 * @brief Mutable state for the pure turn-signal controller.
 */
typedef struct {
    ts_mode_t mode;
    uint32_t phase_ms;
    bool lamps_on;
} ts_core_t;

/**
 * @brief Initialize the controller in the neutral state.
 *
 * @param core Mutable controller instance.
 * @param now_ms Current monotonic time in milliseconds.
 */
void ts_core_init(ts_core_t *core, uint32_t now_ms);

/**
 * @brief Force the controller into a specific mode.
 *
 * @param core Mutable controller instance.
 * @param mode Requested mode.
 * @param now_ms Current monotonic time in milliseconds.
 */
void ts_core_set_mode(ts_core_t *core, ts_mode_t mode, uint32_t now_ms);

/**
 * @brief Apply debounced button presses to the controller.
 *
 * @param core Mutable controller instance.
 * @param btns Debounced button-press edges for this iteration.
 * @param now_ms Current monotonic time in milliseconds.
 */
void ts_core_on_btns(ts_core_t *core, ts_btns_t btns, uint32_t now_ms);

/**
 * @brief Advance the blink phase and return the active logical LED mask.
 *
 * @param core Mutable controller instance.
 * @param now_ms Current monotonic time in milliseconds.
 *
 * @return LED mask that should be active for the current phase.
 */
uint8_t ts_core_tick(ts_core_t *core, uint32_t now_ms);

#endif // TS_CORE_H
