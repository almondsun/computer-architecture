#ifndef BSP_SERIAL_PORT_H
#define BSP_SERIAL_PORT_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BSP_SERIAL_PORT_OK = 0,
    BSP_SERIAL_PORT_ERROR
} BSP_SERIAL_PORT_Status;

BSP_SERIAL_PORT_Status BSP_SERIAL_PORT_Init(void);
bool BSP_SERIAL_PORT_ReadByte(uint8_t *byte);
void BSP_SERIAL_PORT_WriteString(const char *text);

#endif /* BSP_SERIAL_PORT_H */
