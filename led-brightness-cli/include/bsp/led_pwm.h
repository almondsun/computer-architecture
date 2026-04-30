#ifndef BSP_LED_PWM_H
#define BSP_LED_PWM_H

#include <stdint.h>

#define BSP_LED_PWM_MAX_DUTY_PERCENT 100U
#define BSP_LED_PWM_FREQUENCY_HZ 1000U

typedef enum
{
    BSP_LED_PWM_OK = 0,
    BSP_LED_PWM_ERROR
} BSP_LED_PWM_Status;

BSP_LED_PWM_Status BSP_LED_PWM_Init(void);
void BSP_LED_PWM_SetDutyPercent(uint8_t duty_percent);
uint8_t BSP_LED_PWM_GetDutyPercent(void);

#endif /* BSP_LED_PWM_H */
