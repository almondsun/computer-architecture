#ifndef CLI_BRIGHTNESS_COMMAND_H
#define CLI_BRIGHTNESS_COMMAND_H

#include <stdint.h>

typedef enum
{
    BRIGHTNESS_COMMAND_UP = 0,
    BRIGHTNESS_COMMAND_DOWN
} BrightnessCommandDirection;

typedef struct
{
    BrightnessCommandDirection direction;
    uint8_t amount_percent;
} BrightnessCommand;

typedef enum
{
    BRIGHTNESS_COMMAND_PARSE_OK = 0,
    BRIGHTNESS_COMMAND_PARSE_EMPTY,
    BRIGHTNESS_COMMAND_PARSE_UNKNOWN_COMMAND,
    BRIGHTNESS_COMMAND_PARSE_MISSING_PERCENT,
    BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT,
    BRIGHTNESS_COMMAND_PARSE_TRAILING_INPUT
} BrightnessCommandParseStatus;

BrightnessCommandParseStatus BrightnessCommand_Parse(const char *line,
                                                      BrightnessCommand *command);
const char *BrightnessCommand_ParseStatusMessage(BrightnessCommandParseStatus status);

#endif /* CLI_BRIGHTNESS_COMMAND_H */
