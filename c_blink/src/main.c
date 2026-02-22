#include <stdint.h>

#include "blink_config.h"
#include "blink_controller.h"
#include "gpio_led.h"
#include "systick_timebase.h"

int main(void)
{
    /* Hardware and application initialization before entering control loop. */
    gpio_led_init();
    gpio_led_off();
    blink_config_init();
    blink_controller_init();
    systick_timebase_init_1ms();

    for (;;)
    {
        const uint32_t elapsed_ms = systick_timebase_wait_and_get_elapsed_ms();
        blink_controller_process_elapsed_ms(elapsed_ms);
    }
}
