#include <stdint.h>

#include "scrolling_sign_app.h"
#include "scrolling_sign_core.h"
#include "scrolling_sign_hw.h"

void scrolling_sign_app_run(const char *message)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;
    uint32_t last_scan_ms;
    uint8_t scan_digit;

    scrolling_sign_hw_init();
    scrolling_sign_core_init(&core, message, scrolling_sign_hw_now_ms());

    last_scan_ms = 0xFFFFFFFFU;
    scan_digit = 0U;

    while (1) {
        const uint32_t now_ms = scrolling_sign_hw_now_ms();

        scrolling_sign_core_tick(&core, now_ms);
        scrolling_sign_core_frame(&core, &frame);

        if (now_ms != last_scan_ms) {
            scrolling_sign_hw_show_digit(scan_digit, frame.digits[scan_digit]);
            scan_digit =
                    (uint8_t)((scan_digit + 1U) % SCROLLING_SIGN_DIGIT_COUNT);
            last_scan_ms = now_ms;
        }

        scrolling_sign_hw_wait();
    }
}
