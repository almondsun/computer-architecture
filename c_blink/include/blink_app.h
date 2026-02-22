#ifndef BLINK_APP_H
#define BLINK_APP_H

/*
 * Application-level constants for blinking behavior.
 * One blink is one complete ON->OFF cycle.
 */
#define BLINK_DEFAULT_FREQUENCY_HZ      (1u)
#define BLINK_HALF_PERIOD_NUMERATOR_MS  (500u)

/*
 * Timebase backend selection:
 * 1 -> SysTick interrupt + WFI (low CPU usage)
 * 0 -> SysTick COUNTFLAG polling (busy wait)
 *
 * The symbol can be overridden from build flags.
 */
#ifndef BLINK_TIMEBASE_USE_INTERRUPT
#define BLINK_TIMEBASE_USE_INTERRUPT    (1u)
#endif

#if (BLINK_TIMEBASE_USE_INTERRUPT != 0u) && (BLINK_TIMEBASE_USE_INTERRUPT != 1u)
#error "BLINK_TIMEBASE_USE_INTERRUPT must be 0 or 1."
#endif

#endif /* BLINK_APP_H */
