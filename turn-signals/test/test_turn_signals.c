#include <assert.h>
#include <stdint.h>

#include "ts_core.h"

static void test_neutral_state_stays_dark(void)
{
    ts_core_t core;

    ts_core_init(&core, 100U);
    assert(ts_core_tick(&core, 100U) == 0U);
    assert(ts_core_tick(&core, 900U) == 0U);
}

static void test_left_blink_pattern(void)
{
    ts_core_t core;

    ts_core_init(&core, 0U);
    ts_core_on_btns(&core, (ts_btns_t){true, false, false}, 10U);

    assert(ts_core_tick(&core, 10U) == TS_LED_LEFT);
    assert(ts_core_tick(&core, 509U) == TS_LED_LEFT);
    assert(ts_core_tick(&core, 510U) == 0U);
    assert(ts_core_tick(&core, 1009U) == 0U);
    assert(ts_core_tick(&core, 1010U) == TS_LED_LEFT);
}

static void test_middle_button_toggles_parking_and_neutral(void)
{
    ts_core_t core;

    ts_core_init(&core, 0U);
    ts_core_on_btns(&core, (ts_btns_t){false, true, false}, 50U);

    assert(core.mode == TS_MODE_PARKING);
    assert(ts_core_tick(&core, 50U) == TS_LED_PARK);

    ts_core_on_btns(&core, (ts_btns_t){false, true, false}, 75U);

    assert(core.mode == TS_MODE_NEUTRAL);
    assert(ts_core_tick(&core, 75U) == 0U);
}

static void test_left_and_right_override_parking(void)
{
    ts_core_t core;

    ts_core_init(&core, 0U);
    ts_core_set_mode(&core, TS_MODE_PARKING, 0U);

    ts_core_on_btns(&core, (ts_btns_t){false, false, true}, 25U);

    assert(core.mode == TS_MODE_RIGHT);
    assert(ts_core_tick(&core, 25U) == TS_LED_RIGHT);
}

int main(void)
{
    test_neutral_state_stays_dark();
    test_left_blink_pattern();
    test_middle_button_toggles_parking_and_neutral();
    test_left_and_right_override_parking();
    return 0;
}
