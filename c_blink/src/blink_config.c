#include "blink_config.h"

#include "blink_app.h"

/* Module-private user frequency configuration [blinks/s]. */
static uint32_t s_blink_frequency_hz = BLINK_DEFAULT_FREQUENCY_HZ;

void blink_config_init(void)
{
    s_blink_frequency_hz = BLINK_DEFAULT_FREQUENCY_HZ;
}

uint32_t blink_config_get_frequency_hz(void)
{
    return s_blink_frequency_hz;
}

void blink_config_set_frequency_hz(uint32_t blink_frequency_hz)
{
    s_blink_frequency_hz = blink_frequency_hz;
}
