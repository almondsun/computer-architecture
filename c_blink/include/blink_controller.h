#ifndef BLINK_CONTROLLER_H
#define BLINK_CONTROLLER_H

#include <stdint.h>

/*
 * Resets the internal blink-control state machine.
 */
void blink_controller_init(void);

/*
 * Advances blink logic by elapsed time [ms].
 * When elapsed time reaches the configured half-period, LED is toggled.
 */
void blink_controller_process_elapsed_ms(uint32_t elapsed_ms);

#endif /* BLINK_CONTROLLER_H */
