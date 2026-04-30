#include "platform/system_clock.h"

#include "platform/error_handler.h"
#include "stm32l4xx_hal.h"

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef rcc_osc = {0};
    RCC_ClkInitTypeDef rcc_clk = {0};

    __HAL_RCC_PWR_CLK_ENABLE();

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    rcc_osc.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    rcc_osc.MSIState = RCC_MSI_ON;
    rcc_osc.MSICalibrationValue = 0U;
    rcc_osc.MSIClockRange = RCC_MSIRANGE_6;
    rcc_osc.PLL.PLLState = RCC_PLL_ON;
    rcc_osc.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    rcc_osc.PLL.PLLM = 1U;
    rcc_osc.PLL.PLLN = 40U;
    rcc_osc.PLL.PLLP = RCC_PLLP_DIV7;
    rcc_osc.PLL.PLLQ = RCC_PLLQ_DIV2;
    rcc_osc.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&rcc_osc) != HAL_OK)
    {
        Error_Handler();
    }

    rcc_clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                        RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rcc_clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    rcc_clk.APB1CLKDivider = RCC_HCLK_DIV1;
    rcc_clk.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&rcc_clk, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}
