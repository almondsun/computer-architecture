# Scrolling Sign

This project uses the four-digit seven-segment BCD display on the
Multifunction Shield connected to an STM32 Nucleo L476RG board to simulate a
scrolling sign.

`src/main.c` owns the message contract through the `MESSAGE` string. The
firmware scrolls that text from right to left across the four multiplexed
digits, looping continuously. The font asset is embedded in flash as CSV text
and parsed during core initialization on the STM32.

## Architecture

- `lib/scrolling_sign_core/src`: pure scrolling and character-encoding logic
- `ascii.csv`: editable seven-segment font source with `letter` and
  `representation` columns
- `scripts/generate_ascii_asset.py`: embeds `ascii.csv` into the firmware image
  as a raw CSV asset
- `lib/scrolling_sign_app/src`: app loop that combines the core with the
  hardware boundary
- `include/scrolling_sign_hw.h`: hardware contract used by the app layer
- `src/hw_stm32_mfs.c`: STM32 + MFS shift-register adapter
- `src/main.c`: minimal entry point and message definition

## Hardware Notes

The display uses the MFS seven-segment chain driven from the
Arduino-compatible pins exposed by the Nucleo board:

- `D4` / `PB5`: latch
- `D7` / `PA8`: clock
- `D8` / `PA9`: data

## Validation

- Host-side core tests:
  `python3 scripts/generate_ascii_asset.py && gcc -std=c11 -Wall -Wextra -pedantic -Ilib/scrolling_sign_core/src test/test_scrolling_sign_core.c lib/scrolling_sign_core/src/scrolling_sign_core.c -o /tmp/scrolling_sign_core_test && /tmp/scrolling_sign_core_test`
- Firmware build:
  `pio run -e nucleo_l476rg`
