#ifndef APP_BRIGHTNESS_CLI_APP_H
#define APP_BRIGHTNESS_CLI_APP_H

#include <stddef.h>
#include <stdint.h>

#define BRIGHTNESS_CLI_MAX_PERCENT 100U

typedef void (*BrightnessCliSetDutyFn)(uint8_t duty_percent);
typedef void (*BrightnessCliWriteFn)(const char *text);

typedef struct
{
    uint8_t brightness_percent;
    char line[32];
    size_t line_length;
    BrightnessCliSetDutyFn set_duty;
    BrightnessCliWriteFn write;
} BrightnessCliApp;

void BrightnessCliApp_Init(BrightnessCliApp *app,
                           BrightnessCliSetDutyFn set_duty,
                           BrightnessCliWriteFn write);
void BrightnessCliApp_ProcessByte(BrightnessCliApp *app, uint8_t byte);
void BrightnessCliApp_ProcessLine(BrightnessCliApp *app, const char *line);

#endif /* APP_BRIGHTNESS_CLI_APP_H */
