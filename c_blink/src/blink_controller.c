#include "blink_controller.h"

#include "blink_config.h"
#include "blink_timing.h"
#include "gpio_led.h"

/* Module-private accumulated elapsed time since the last LED toggle [ms]. */
static uint32_t s_elapsed_since_toggle_ms = 0u;

void blink_controller_init(void)
{
    s_elapsed_since_toggle_ms = 0u;
}

void blink_controller_process_elapsed_ms(uint32_t elapsed_ms)
{
    if (elapsed_ms == 0u)
    {
        return;
    }

    /* Keep the same accumulation strategy used in the assembly projects. */
    s_elapsed_since_toggle_ms += elapsed_ms;

    const uint32_t blink_frequency_hz = blink_config_get_frequency_hz();
    const uint32_t half_period_ms = blink_compute_half_period_ms(blink_frequency_hz);

    if (s_elapsed_since_toggle_ms < half_period_ms)
    {
        return;
    }

    /* Half-period reached: invert LED state and restart elapsed counter. */
    gpio_led_toggle();
    s_elapsed_since_toggle_ms = 0u;
}
