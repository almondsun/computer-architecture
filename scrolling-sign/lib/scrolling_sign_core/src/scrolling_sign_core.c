#include <string.h>

#include "scrolling_sign_core.h"
#include "generated_ascii_asset.h"

typedef struct {
    const char *cursor;
} scrolling_sign_csv_parser_t;

static size_t scrolling_sign_core_cycle_length(const scrolling_sign_core_t *core)
{
    return core->message_length + SCROLLING_SIGN_DIGIT_COUNT;
}

static char scrolling_sign_ascii_upper(char c)
{
    if ((c >= 'a') && (c <= 'z')) {
        return (char)(c - ('a' - 'A'));
    }

    return c;
}

static void scrolling_sign_font_clear(uint8_t *ascii_font)
{
    uint32_t i;

    for (i = 0U; i < 128U; ++i) {
        ascii_font[i] = 0U;
    }
}

static int scrolling_sign_csv_is_line_end(char c)
{
    return (c == '\n') || (c == '\r') || (c == '\0');
}

static void scrolling_sign_csv_skip_line_endings(scrolling_sign_csv_parser_t *parser)
{
    while ((parser->cursor[0] == '\n') || (parser->cursor[0] == '\r')) {
        ++parser->cursor;
    }
}

static int scrolling_sign_csv_read_field(
        scrolling_sign_csv_parser_t *parser,
        char *buffer,
        size_t capacity)
{
    size_t length = 0U;
    int quoted = 0;

    if ((parser == 0) || (buffer == 0) || (capacity == 0U)) {
        return 0;
    }

    if (parser->cursor[0] == '"') {
        quoted = 1;
        ++parser->cursor;
    }

    while (parser->cursor[0] != '\0') {
        const char current = parser->cursor[0];

        if (quoted != 0) {
            if (current == '"') {
                if (parser->cursor[1] == '"') {
                    if ((length + 1U) >= capacity) {
                        return 0;
                    }

                    buffer[length++] = '"';
                    parser->cursor += 2;
                    continue;
                }

                ++parser->cursor;
                quoted = 0;
                break;
            }
        } else if ((current == ',') || scrolling_sign_csv_is_line_end(current)) {
            break;
        }

        if ((length + 1U) >= capacity) {
            return 0;
        }

        buffer[length++] = current;
        ++parser->cursor;
    }

    if (quoted != 0) {
        return 0;
    }

    buffer[length] = '\0';
    return 1;
}

static int scrolling_sign_csv_expect_delimiter(
        scrolling_sign_csv_parser_t *parser,
        char delimiter)
{
    if ((parser == 0) || (parser->cursor[0] != delimiter)) {
        return 0;
    }

    ++parser->cursor;
    return 1;
}

static uint8_t scrolling_sign_parse_representation(
        const char *representation,
        int *ok)
{
    uint8_t mask = 0U;
    const char *cursor = representation;

    *ok = 1;

    while (cursor[0] != '\0') {
        if (cursor[0] == ' ') {
            ++cursor;
            continue;
        }

        if ((cursor[0] == 'D') && (cursor[1] == 'P')
                && ((cursor[2] == '\0') || (cursor[2] == ' '))) {
            mask |= SCROLLING_SIGN_SEG_DP;
            cursor += 2;
            continue;
        }

        switch (cursor[0]) {
        case 'A':
            mask |= SCROLLING_SIGN_SEG_A;
            break;
        case 'B':
            mask |= SCROLLING_SIGN_SEG_B;
            break;
        case 'C':
            mask |= SCROLLING_SIGN_SEG_C;
            break;
        case 'D':
            mask |= SCROLLING_SIGN_SEG_D;
            break;
        case 'E':
            mask |= SCROLLING_SIGN_SEG_E;
            break;
        case 'F':
            mask |= SCROLLING_SIGN_SEG_F;
            break;
        case 'G':
            mask |= SCROLLING_SIGN_SEG_G;
            break;
        default:
            *ok = 0;
            return 0U;
        }

        ++cursor;
        if ((cursor[0] != '\0') && (cursor[0] != ' ')) {
            *ok = 0;
            return 0U;
        }
    }

    return mask;
}

static int scrolling_sign_load_ascii_font(uint8_t *ascii_font)
{
    char letter_field[8];
    char representation_field[64];
    scrolling_sign_csv_parser_t parser;

    if (ascii_font == 0) {
        return 0;
    }

    scrolling_sign_font_clear(ascii_font);

    parser.cursor = k_scrolling_sign_ascii_csv;
    if (!scrolling_sign_csv_read_field(&parser, letter_field, sizeof(letter_field))
            || (strcmp(letter_field, "letter") != 0)
            || !scrolling_sign_csv_expect_delimiter(&parser, ',')
            || !scrolling_sign_csv_read_field(
                    &parser,
                    representation_field,
                    sizeof(representation_field))
            || (strcmp(representation_field, "representation") != 0)) {
        return 0;
    }

    scrolling_sign_csv_skip_line_endings(&parser);

    while (parser.cursor[0] != '\0') {
        int ok;
        unsigned char ascii_code;

        if (!scrolling_sign_csv_read_field(&parser, letter_field, sizeof(letter_field))
                || !scrolling_sign_csv_expect_delimiter(&parser, ',')
                || !scrolling_sign_csv_read_field(
                        &parser,
                        representation_field,
                        sizeof(representation_field))) {
            return 0;
        }

        if (strlen(letter_field) != 1U) {
            return 0;
        }

        ascii_code = (unsigned char)letter_field[0];
        if (ascii_code >= 128U) {
            return 0;
        }

        ascii_font[ascii_code] =
                scrolling_sign_parse_representation(representation_field, &ok);
        if (ok == 0) {
            return 0;
        }

        if ((parser.cursor[0] != '\0')
                && !scrolling_sign_csv_is_line_end(parser.cursor[0])) {
            return 0;
        }

        scrolling_sign_csv_skip_line_endings(&parser);
    }

    return 1;
}

static uint8_t scrolling_sign_core_encode_char(
        const scrolling_sign_core_t *core,
        char c)
{
    const unsigned char ascii = (unsigned char)scrolling_sign_ascii_upper(c);

    if ((core == 0) || (ascii >= 128U)) {
        return 0U;
    }

    return core->ascii_font[ascii];
}

void scrolling_sign_core_init(
        scrolling_sign_core_t *core,
        const char *message,
        uint32_t now_ms)
{
    if (core == 0) {
        return;
    }

    (void)scrolling_sign_load_ascii_font(core->ascii_font);
    core->message = (message != 0) ? message : "";
    core->message_length = strlen(core->message);
    core->window_shift = 0U;
    core->phase_ms = now_ms;
}

void scrolling_sign_core_tick(scrolling_sign_core_t *core, uint32_t now_ms)
{
    uint32_t elapsed;
    const size_t cycle_length =
            (core != 0) ? scrolling_sign_core_cycle_length(core) : 0U;

    if ((core == 0) || (cycle_length == 0U)) {
        return;
    }

    elapsed = (uint32_t)(now_ms - core->phase_ms);
    while (elapsed >= SCROLLING_SIGN_SCROLL_STEP_MS) {
        core->phase_ms += SCROLLING_SIGN_SCROLL_STEP_MS;
        elapsed -= SCROLLING_SIGN_SCROLL_STEP_MS;
        ++core->window_shift;
        if (core->window_shift >= cycle_length) {
            core->window_shift = 0U;
        }
    }
}

void scrolling_sign_core_frame(
        const scrolling_sign_core_t *core,
        scrolling_sign_frame_t *frame)
{
    uint32_t digit;

    if ((core == 0) || (frame == 0)) {
        return;
    }

    for (digit = 0U; digit < SCROLLING_SIGN_DIGIT_COUNT; ++digit) {
        char current = ' ';
        const size_t source_offset = core->window_shift + (size_t)digit + 1U;

        if (source_offset >= SCROLLING_SIGN_DIGIT_COUNT) {
            const size_t message_index =
                    source_offset - SCROLLING_SIGN_DIGIT_COUNT;

            if (message_index < core->message_length) {
                current = core->message[message_index];
            }
        }

        frame->digits[digit] = scrolling_sign_core_encode_char(core, current);
    }
}
