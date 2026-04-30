#include "app/brightness_cli_app.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "cli/brightness_command.h"

static void AppWrite(const BrightnessCliApp *app, const char *text)
{
    if ((app != NULL) && (app->write != NULL))
    {
        app->write(text);
    }
}

static void ApplyBrightness(BrightnessCliApp *app)
{
    if ((app != NULL) && (app->set_duty != NULL))
    {
        app->set_duty(app->brightness_percent);
    }
}

static uint8_t IncreaseClamped(uint8_t current, uint8_t amount)
{
    const uint16_t sum = (uint16_t)current + (uint16_t)amount;

    return (sum > BRIGHTNESS_CLI_MAX_PERCENT) ? BRIGHTNESS_CLI_MAX_PERCENT : (uint8_t)sum;
}

static uint8_t DecreaseClamped(uint8_t current, uint8_t amount)
{
    return (amount > current) ? 0U : (uint8_t)(current - amount);
}

static void WritePercent(BrightnessCliApp *app)
{
    char message[] = "brightness: 000%\r\n> ";
    uint8_t value = app->brightness_percent;

    message[12] = (char)('0' + (value / 100U));
    value %= 100U;
    message[13] = (char)('0' + (value / 10U));
    message[14] = (char)('0' + (value % 10U));

    AppWrite(app, message);
}

static void WriteError(BrightnessCliApp *app, BrightnessCommandParseStatus status)
{
    AppWrite(app, "error: ");
    AppWrite(app, BrightnessCommand_ParseStatusMessage(status));
    AppWrite(app, "\r\n> ");
}

void BrightnessCliApp_Init(BrightnessCliApp *app,
                           BrightnessCliSetDutyFn set_duty,
                           BrightnessCliWriteFn write)
{
    if (app == NULL)
    {
        return;
    }

    memset(app, 0, sizeof(*app));
    app->set_duty = set_duty;
    app->write = write;

    ApplyBrightness(app);
    AppWrite(app, "\r\nLD2 brightness CLI\r\n");
    AppWrite(app, "commands: up <0-100>, down <0-100>\r\n> ");
}

void BrightnessCliApp_ProcessLine(BrightnessCliApp *app, const char *line)
{
    BrightnessCommand command;
    BrightnessCommandParseStatus status;

    if ((app == NULL) || (line == NULL))
    {
        return;
    }

    status = BrightnessCommand_Parse(line, &command);
    if (status != BRIGHTNESS_COMMAND_PARSE_OK)
    {
        WriteError(app, status);
        return;
    }

    if (command.direction == BRIGHTNESS_COMMAND_UP)
    {
        app->brightness_percent = IncreaseClamped(app->brightness_percent,
                                                  command.amount_percent);
    }
    else
    {
        app->brightness_percent = DecreaseClamped(app->brightness_percent,
                                                  command.amount_percent);
    }

    ApplyBrightness(app);
    WritePercent(app);
}

void BrightnessCliApp_ProcessByte(BrightnessCliApp *app, uint8_t byte)
{
    if (app == NULL)
    {
        return;
    }

    if ((byte == '\r') || (byte == '\n'))
    {
        if (app->line_length > 0U)
        {
            app->line[app->line_length] = '\0';
            BrightnessCliApp_ProcessLine(app, app->line);
            app->line_length = 0U;
        }
        return;
    }

    if ((byte == '\b') || (byte == 0x7FU))
    {
        if (app->line_length > 0U)
        {
            --app->line_length;
        }
        return;
    }

    if ((byte < 0x20U) || (byte > 0x7EU))
    {
        return;
    }

    if (app->line_length >= (sizeof(app->line) - 1U))
    {
        app->line_length = 0U;
        AppWrite(app, "\r\nerror: command too long\r\n> ");
        return;
    }

    app->line[app->line_length] = (char)byte;
    ++app->line_length;
}
