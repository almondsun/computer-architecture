#include "blink_timing.h"

#include "blink_app.h"

uint32_t blink_compute_half_period_ms(uint32_t blink_frequency_hz)
{
    uint32_t safe_frequency_hz = blink_frequency_hz;

    /* Prevent divide-by-zero when user-provided frequency is zero. */
    if (safe_frequency_hz == 0u)
    {
        safe_frequency_hz = 1u;
    }

    uint32_t half_period_ms = BLINK_HALF_PERIOD_NUMERATOR_MS / safe_frequency_hz;

    /* Ensure minimum non-zero delay so loop logic always progresses. */
    if (half_period_ms == 0u)
    {
        half_period_ms = 1u;
    }

    return half_period_ms;
}
