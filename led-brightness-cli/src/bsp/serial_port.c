#include "bsp/serial_port.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bsp/board.h"
#include "stm32l4xx_hal.h"

#define SERIAL_TX_TIMEOUT_MS 100U

static UART_HandleTypeDef huart_cli;

static void SERIAL_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio.Pin = BSP_CLI_UART_TX_PIN | BSP_CLI_UART_RX_PIN;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = BSP_CLI_UART_GPIO_AF;

    HAL_GPIO_Init(BSP_CLI_UART_GPIO_PORT, &gpio);
}

BSP_SERIAL_PORT_Status BSP_SERIAL_PORT_Init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    SERIAL_GPIO_Init();

    huart_cli.Instance = BSP_CLI_UART;
    huart_cli.Init.BaudRate = BSP_CLI_UART_BAUDRATE;
    huart_cli.Init.WordLength = UART_WORDLENGTH_8B;
    huart_cli.Init.StopBits = UART_STOPBITS_1;
    huart_cli.Init.Parity = UART_PARITY_NONE;
    huart_cli.Init.Mode = UART_MODE_TX_RX;
    huart_cli.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_cli.Init.OverSampling = UART_OVERSAMPLING_16;
    huart_cli.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart_cli.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    return (HAL_UART_Init(&huart_cli) == HAL_OK) ? BSP_SERIAL_PORT_OK : BSP_SERIAL_PORT_ERROR;
}

bool BSP_SERIAL_PORT_ReadByte(uint8_t *byte)
{
    if (byte == NULL)
    {
        return false;
    }

    return HAL_UART_Receive(&huart_cli, byte, 1U, 0U) == HAL_OK;
}

void BSP_SERIAL_PORT_WriteString(const char *text)
{
    if (text == NULL)
    {
        return;
    }

    (void)HAL_UART_Transmit(&huart_cli,
                            (uint8_t *)(uintptr_t)text,
                            (uint16_t)strlen(text),
                            SERIAL_TX_TIMEOUT_MS);
}
