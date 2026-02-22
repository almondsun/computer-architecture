#include "systick_timebase.h"

#include <stdint.h>

#include "blink_app.h"
#include "stm32l4xx.h"

#if BLINK_TIMEBASE_USE_INTERRUPT
/* Elapsed milliseconds accumulated by SysTick interrupt handler [ms]. */
static volatile uint32_t s_systick_elapsed_ms = 0u;
#endif

/*
 * Traps execution in a known-safe state if SysTick configuration fails.
 * This mirrors the behavior of the original assembly implementations.
 */
static void systick_configuration_error_trap(void)
{
    while (1)
    {
    }
}

#if BLINK_TIMEBASE_USE_INTERRUPT
/*
 * Atomically reads and clears elapsed milliseconds produced by SysTick ISR.
 */
static uint32_t systick_consume_elapsed_ms(void)
{
    const uint32_t primask_state = __get_PRIMASK();
    __disable_irq();

    const uint32_t elapsed_ms = s_systick_elapsed_ms;
    s_systick_elapsed_ms = 0u;

    __set_PRIMASK(primask_state);

    return elapsed_ms;
}
#else
/*
 * Polls SysTick COUNTFLAG until one 1 ms period has elapsed.
 */
static void systick_wait_1ms_polling(void)
{
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0u)
    {
    }
}
#endif

void systick_timebase_init_1ms(void)
{
    /* Refresh current core clock value before deriving SysTick reload value. */
    SystemCoreClockUpdate();

    const uint32_t ticks_per_ms = SystemCoreClock / 1000u;
    if ((ticks_per_ms == 0u) || ((ticks_per_ms - 1u) > SysTick_LOAD_RELOAD_Msk))
    {
        systick_configuration_error_trap();
    }

#if BLINK_TIMEBASE_USE_INTERRUPT
    s_systick_elapsed_ms = 0u;
#endif

    SysTick->LOAD = ticks_per_ms - 1u;
    SysTick->VAL = 0u;

#if BLINK_TIMEBASE_USE_INTERRUPT
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
#else
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
#endif
}

uint32_t systick_timebase_wait_and_get_elapsed_ms(void)
{
#if BLINK_TIMEBASE_USE_INTERRUPT
    /*
     * Sleep until next interrupt to avoid active polling.
     * SysTick ISR updates elapsed milliseconds in the background.
     */
    __WFI();
    return systick_consume_elapsed_ms();
#else
    systick_wait_1ms_polling();
    return 1u;
#endif
}

#if BLINK_TIMEBASE_USE_INTERRUPT
void SysTick_Handler(void)
{
    s_systick_elapsed_ms += 1u;
}
#endif
