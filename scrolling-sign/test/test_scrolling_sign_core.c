#include <assert.h>

#include "scrolling_sign_core.h"

static void assert_frame(
        const scrolling_sign_frame_t *frame,
        uint8_t digit0,
        uint8_t digit1,
        uint8_t digit2,
        uint8_t digit3)
{
    assert(frame->digits[0] == digit0);
    assert(frame->digits[1] == digit1);
    assert(frame->digits[2] == digit2);
    assert(frame->digits[3] == digit3);
}

static void test_initial_frame_places_first_character_on_rightmost_digit(void)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;

    scrolling_sign_core_init(&core, "AB", 0U);
    scrolling_sign_core_frame(&core, &frame);

    assert_frame(
            &frame,
            0U,
            0U,
            0U,
            (uint8_t)(SCROLLING_SIGN_SEG_A
                    | SCROLLING_SIGN_SEG_B
                    | SCROLLING_SIGN_SEG_C
                    | SCROLLING_SIGN_SEG_E
                    | SCROLLING_SIGN_SEG_F
                    | SCROLLING_SIGN_SEG_G));
}

static void test_scroll_moves_message_left_across_digits(void)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;
    const uint8_t a_segments = (uint8_t)(SCROLLING_SIGN_SEG_A
            | SCROLLING_SIGN_SEG_B
            | SCROLLING_SIGN_SEG_C
            | SCROLLING_SIGN_SEG_E
            | SCROLLING_SIGN_SEG_F
            | SCROLLING_SIGN_SEG_G);
    const uint8_t b_segments = (uint8_t)(SCROLLING_SIGN_SEG_C
            | SCROLLING_SIGN_SEG_D
            | SCROLLING_SIGN_SEG_E
            | SCROLLING_SIGN_SEG_F
            | SCROLLING_SIGN_SEG_G);

    scrolling_sign_core_init(&core, "AB", 10U);

    scrolling_sign_core_tick(&core, 10U + SCROLLING_SIGN_SCROLL_STEP_MS);
    scrolling_sign_core_frame(&core, &frame);
    assert_frame(&frame, 0U, 0U, a_segments, b_segments);

    scrolling_sign_core_tick(&core, 10U + (3U * SCROLLING_SIGN_SCROLL_STEP_MS));
    scrolling_sign_core_frame(&core, &frame);
    assert_frame(&frame, a_segments, b_segments, 0U, 0U);
}

static void test_scroll_wraps_after_message_leaves_display(void)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;
    const uint8_t z_segments = (uint8_t)(SCROLLING_SIGN_SEG_A
            | SCROLLING_SIGN_SEG_B
            | SCROLLING_SIGN_SEG_D
            | SCROLLING_SIGN_SEG_E
            | SCROLLING_SIGN_SEG_G);

    scrolling_sign_core_init(&core, "Z", 0U);
    scrolling_sign_core_tick(&core, 5U * SCROLLING_SIGN_SCROLL_STEP_MS);
    scrolling_sign_core_frame(&core, &frame);

    assert_frame(&frame, 0U, 0U, 0U, z_segments);
}

static void test_lowercase_and_unsupported_characters_are_handled(void)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;
    const uint8_t h_segments = (uint8_t)(SCROLLING_SIGN_SEG_B
            | SCROLLING_SIGN_SEG_C
            | SCROLLING_SIGN_SEG_E
            | SCROLLING_SIGN_SEG_F
            | SCROLLING_SIGN_SEG_G);

    scrolling_sign_core_init(&core, "h!", 0U);
    scrolling_sign_core_tick(&core, SCROLLING_SIGN_SCROLL_STEP_MS);
    scrolling_sign_core_frame(&core, &frame);

    assert_frame(&frame, 0U, 0U, h_segments, 0U);
}

static void test_quoted_csv_character_entry_is_loaded_for_decimal_point(void)
{
    scrolling_sign_core_t core;
    scrolling_sign_frame_t frame;

    scrolling_sign_core_init(&core, ".", 0U);
    scrolling_sign_core_frame(&core, &frame);

    assert_frame(&frame, 0U, 0U, 0U, SCROLLING_SIGN_SEG_DP);
}

int main(void)
{
    test_initial_frame_places_first_character_on_rightmost_digit();
    test_scroll_moves_message_left_across_digits();
    test_scroll_wraps_after_message_leaves_display();
    test_lowercase_and_unsupported_characters_are_handled();
    test_quoted_csv_character_entry_is_loaded_for_decimal_point();
    return 0;
}
