#include "ts_core.h"

static uint8_t ts_core_mode_mask(ts_mode_t mode)
{
    switch (mode) {
    case TS_MODE_LEFT:
        return TS_LED_LEFT;
    case TS_MODE_RIGHT:
        return TS_LED_RIGHT;
    case TS_MODE_PARKING:
        return TS_LED_PARK;
    case TS_MODE_NEUTRAL:
    default:
        return 0U;
    }
}

void ts_core_init(ts_core_t *core, uint32_t now_ms)
{
    if (core == 0) {
        return;
    }

    core->mode = TS_MODE_NEUTRAL;
    core->phase_ms = now_ms;
    core->lamps_on = false;
}

void ts_core_set_mode(ts_core_t *core, ts_mode_t mode, uint32_t now_ms)
{
    if (core == 0) {
        return;
    }

    if (core->mode == mode) {
        return;
    }

    core->mode = mode;
    core->phase_ms = now_ms;
    core->lamps_on = (mode != TS_MODE_NEUTRAL);
}

void ts_core_on_btns(ts_core_t *core, ts_btns_t btns, uint32_t now_ms)
{
    if (core == 0) {
        return;
    }

    if (btns.middle) {
        const ts_mode_t next_mode = (core->mode == TS_MODE_PARKING)
                                        ? TS_MODE_NEUTRAL
                                        : TS_MODE_PARKING;

        ts_core_set_mode(core, next_mode, now_ms);
        return;
    }

    if (btns.left == btns.right) {
        return;
    }

    ts_core_set_mode(core, btns.left ? TS_MODE_LEFT : TS_MODE_RIGHT, now_ms);
}

uint8_t ts_core_tick(ts_core_t *core, uint32_t now_ms)
{
    uint8_t mask;

    if (core == 0) {
        return 0U;
    }

    mask = ts_core_mode_mask(core->mode);
    if (mask == 0U) {
        core->lamps_on = false;
        core->phase_ms = now_ms;
        return 0U;
    }

    if ((uint32_t)(now_ms - core->phase_ms) >= TS_BLINK_MS) {
        core->phase_ms = now_ms;
        core->lamps_on = !core->lamps_on;
    }

    return core->lamps_on ? mask : 0U;
}
