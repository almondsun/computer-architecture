#include "app/brightness_cli_app.h"
#include "bsp/led_pwm.h"
#include "bsp/serial_port.h"
#include "platform/error_handler.h"
#include "platform/system_clock.h"
#include "stm32l4xx_hal.h"

static BrightnessCliApp app;

static void ApplyLedDuty(uint8_t duty_percent)
{
    BSP_LED_PWM_SetDutyPercent(duty_percent);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    if (BSP_LED_PWM_Init() != BSP_LED_PWM_OK)
    {
        Error_Handler();
    }

    if (BSP_SERIAL_PORT_Init() != BSP_SERIAL_PORT_OK)
    {
        Error_Handler();
    }

    BrightnessCliApp_Init(&app, ApplyLedDuty, BSP_SERIAL_PORT_WriteString);

    while (1)
    {
        uint8_t byte;

        if (BSP_SERIAL_PORT_ReadByte(&byte))
        {
            BrightnessCliApp_ProcessByte(&app, byte);
        }
    }
}
