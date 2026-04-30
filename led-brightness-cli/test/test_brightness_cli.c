#include "app/brightness_cli_app.h"
#include "cli/brightness_command.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

static uint8_t observed_duty;
static char observed_output[512];
static size_t observed_output_len;

static void ResetOutput(void)
{
    observed_duty = 0U;
    observed_output[0] = '\0';
    observed_output_len = 0U;
}

static void SetDuty(uint8_t duty_percent)
{
    observed_duty = duty_percent;
}

static void WriteText(const char *text)
{
    size_t text_len = strlen(text);

    assert((observed_output_len + text_len) < sizeof(observed_output));
    memcpy(&observed_output[observed_output_len], text, text_len + 1U);
    observed_output_len += text_len;
}

static void TestParserAcceptsCommands(void)
{
    BrightnessCommand command;

    assert(BrightnessCommand_Parse("up 25", &command) == BRIGHTNESS_COMMAND_PARSE_OK);
    assert(command.direction == BRIGHTNESS_COMMAND_UP);
    assert(command.amount_percent == 25U);

    assert(BrightnessCommand_Parse("  DOWN\t100  ", &command) == BRIGHTNESS_COMMAND_PARSE_OK);
    assert(command.direction == BRIGHTNESS_COMMAND_DOWN);
    assert(command.amount_percent == 100U);
}

static void TestParserRejectsBadInput(void)
{
    BrightnessCommand command;

    assert(BrightnessCommand_Parse("", &command) == BRIGHTNESS_COMMAND_PARSE_EMPTY);
    assert(BrightnessCommand_Parse("up", &command) == BRIGHTNESS_COMMAND_PARSE_MISSING_PERCENT);
    assert(BrightnessCommand_Parse("down -1", &command) == BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT);
    assert(BrightnessCommand_Parse("up 101", &command) == BRIGHTNESS_COMMAND_PARSE_INVALID_PERCENT);
    assert(BrightnessCommand_Parse("left 10", &command) == BRIGHTNESS_COMMAND_PARSE_UNKNOWN_COMMAND);
    assert(BrightnessCommand_Parse("up 10 now", &command) == BRIGHTNESS_COMMAND_PARSE_TRAILING_INPUT);
}

static void TestAppClampsBrightness(void)
{
    BrightnessCliApp app;

    ResetOutput();
    BrightnessCliApp_Init(&app, SetDuty, WriteText);
    assert(observed_duty == 0U);

    BrightnessCliApp_ProcessLine(&app, "up 40");
    assert(app.brightness_percent == 40U);
    assert(observed_duty == 40U);

    BrightnessCliApp_ProcessLine(&app, "up 80");
    assert(app.brightness_percent == 100U);
    assert(observed_duty == 100U);

    BrightnessCliApp_ProcessLine(&app, "down 30");
    assert(app.brightness_percent == 70U);
    assert(observed_duty == 70U);

    BrightnessCliApp_ProcessLine(&app, "down 90");
    assert(app.brightness_percent == 0U);
    assert(observed_duty == 0U);
}

static void TestByteInputBuildsLines(void)
{
    BrightnessCliApp app;
    const char *input = "up 7\r";
    size_t index;

    ResetOutput();
    BrightnessCliApp_Init(&app, SetDuty, WriteText);

    for (index = 0U; input[index] != '\0'; ++index)
    {
        BrightnessCliApp_ProcessByte(&app, (uint8_t)input[index]);
    }

    assert(app.brightness_percent == 7U);
    assert(observed_duty == 7U);
}

int main(void)
{
    TestParserAcceptsCommands();
    TestParserRejectsBadInput();
    TestAppClampsBrightness();
    TestByteInputBuildsLines();
    return 0;
}
