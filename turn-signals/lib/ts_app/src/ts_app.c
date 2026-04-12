#include <stdint.h>

#include "ts_app.h"
#include "ts_core.h"
#include "ts_hw.h"

void ts_app_run(void)
{
    ts_core_t core;

    ts_hw_init();
    ts_core_init(&core, ts_hw_now_ms());

    while (1) {
        const uint32_t now_ms = ts_hw_now_ms();
        const ts_btns_t btns = ts_hw_poll_btns(now_ms);
        uint8_t led_mask;

        ts_core_on_btns(&core, btns, now_ms);
        led_mask = ts_core_tick(&core, now_ms);
        ts_hw_apply_leds(led_mask);
        ts_hw_wait();
    }
}
