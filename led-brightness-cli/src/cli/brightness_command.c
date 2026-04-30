#include "cli/brightness_command.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool IsSpace(char c)
{
    return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n');
}

static bool IsDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

static char ToLowerAscii(char c)
{
    if ((c >= 'A') && (c <= 'Z'))
    {
        return (char)(c - ('A' - 'a'));
    }

    return c;
}

static const char *SkipSpaces(const char *text)
{
    while ((*text != '\0') && IsSpace(*text))
    {
        ++text;
    }

    return text;
}

static bool ConsumeWord(const char **cursor, const char *word)
{
    const char *text = *cursor;

    while (*word != '\0')
    {
        if (ToLowerAscii(*text) != *word)
        {
            return false;
        }

        ++text;
        ++word;
    }

    if ((*text != '\0') && !IsSpace(*text))
    {
        return false;
    }

    *cursor = text;
    return true;
}

BrightnessCommandParseStatus BrightnessCommand_Parse(const char *line,
                                                      BrightnessCommand *command)
{
    const char *cursor;
    uint32_t value = 0U;
    bool has_digit = false;

    if ((line == NULL) || (command == NULL))
    {
        return BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT;
    }

    cursor = SkipSpaces(line);
    if (*cursor == '\0')
    {
        return BRIGHTNESS_COMMAND_PARSE_EMPTY;
    }

    if (ConsumeWord(&cursor, "up"))
    {
        command->direction = BRIGHTNESS_COMMAND_UP;
    }
    else if (ConsumeWord(&cursor, "down"))
    {
        command->direction = BRIGHTNESS_COMMAND_DOWN;
    }
    else
    {
        return BRIGHTNESS_COMMAND_PARSE_UNKNOWN_COMMAND;
    }

    cursor = SkipSpaces(cursor);
    if (*cursor == '\0')
    {
        return BRIGHTNESS_COMMAND_PARSE_MISSING_PERCENT;
    }

    while (IsDigit(*cursor))
    {
        has_digit = true;
        value = (value * 10U) + (uint32_t)(*cursor - '0');

        if (value > 100U)
        {
            return BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT;
        }

        ++cursor;
    }

    if (!has_digit)
    {
        return BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT;
    }

    cursor = SkipSpaces(cursor);
    if (*cursor != '\0')
    {
        return BRIGHTNESS_COMMAND_PARSE_TRAILING_INPUT;
    }

    command->amount_percent = (uint8_t)value;
    return BRIGHTNESS_COMMAND_PARSE_OK;
}

const char *BrightnessCommand_ParseStatusMessage(BrightnessCommandParseStatus status)
{
    switch (status)
    {
    case BRIGHTNESS_COMMAND_PARSE_OK:
        return "ok";
    case BRIGHTNESS_COMMAND_PARSE_EMPTY:
        return "empty command";
    case BRIGHTNESS_COMMAND_PARSE_UNKNOWN_COMMAND:
        return "expected 'up <percent>' or 'down <percent>'";
    case BRIGHTNESS_COMMAND_PARSE_MISSING_PERCENT:
        return "missing percent value";
    case BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT:
        return "percent must be an integer from 0 to 100";
    case BRIGHTNESS_COMMAND_PARSE_TRAILING_INPUT:
        return "unexpected text after percent";
    default:
        return "unknown parser status";
    }
}
