# Turn Signals

This project uses the LEDs of the MFS connected to an STM32 Nucleo L476RG board to model the states of car turn signals in plain C.

The code is organized so the portable logic stays isolated from the selected board support. Migrating to different hardware should mainly mean replacing the `hw` adapter while keeping the `core` and `app` modules unchanged.

## Controls

- Left button: selects left turn indicators
- Middle button: toggles between parking blink mode and neutral
- Right button: selects right turn indicators

## Architecture

- `lib/ts_core/src`: pure turn-signal state machine and public core contract
- `lib/ts_app/src`: orchestration layer that wires the core to the hardware contract
- `include/ts_hw.h`: hardware boundary used by the app layer
- `src/hw_stm32_mfs.c`: STM32 + MFS hardware adapter
- `src/main.c`: minimal entry point

## Validation

- Host-side core tests: compile `test/test_turn_signals.c` with `lib/ts_core/src/ts_core.c`
- Firmware build: `pio run -e nucleo_l476rg`
