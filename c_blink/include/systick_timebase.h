#ifndef SYSTICK_TIMEBASE_H
#define SYSTICK_TIMEBASE_H

#include <stdint.h>

/*
 * Configures SysTick for a 1 ms timebase using the selected backend.
 * The function does not return if the configuration is invalid.
 */
void systick_timebase_init_1ms(void);

/*
 * Waits for time to advance and returns elapsed milliseconds [ms].
 * Interrupt backend: can return values > 1 after wake-up.
 * Polling backend  : always returns 1.
 */
uint32_t systick_timebase_wait_and_get_elapsed_ms(void);

#endif /* SYSTICK_TIMEBASE_H */
