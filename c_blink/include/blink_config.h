#ifndef BLINK_CONFIG_H
#define BLINK_CONFIG_H

#include <stdint.h>

/*
 * Initializes user-configurable blink parameters to their defaults.
 */
void blink_config_init(void);

/*
 * Returns the configured blink frequency [blinks/s].
 */
uint32_t blink_config_get_frequency_hz(void);

/*
 * Updates the blink frequency [blinks/s].
 * Note: A value of 0 is accepted; timing logic will clamp it safely.
 */
void blink_config_set_frequency_hz(uint32_t blink_frequency_hz);

#endif /* BLINK_CONFIG_H */
