#ifndef BLINK_TIMING_H
#define BLINK_TIMING_H

#include <stdint.h>

/*
 * Converts blink frequency [blinks/s] into LED toggle half-period [ms].
 * The result is always >= 1 ms.
 */
uint32_t blink_compute_half_period_ms(uint32_t blink_frequency_hz);

#endif /* BLINK_TIMING_H */
