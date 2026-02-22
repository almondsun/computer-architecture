# STM32 Assembly Project: Shield Buttons to Shield LEDs

## Goal
Implement the following behavior on an STM32 NUCLEO board with Arduino shield headers:

- `S1` button on `A1` -> `D1` LED ON while pressed.
- `S2` button on `A2` -> `D2` LED ON while pressed.
- `S3` button on `A3` -> `D3` LED ON while pressed.

## Project structure
- `platformio.ini`: Build target (`nucleo_l476rg`) and framework (`cmsis`).
- `include/stm32l476rg_shield_map.inc`: Register addresses, pin mapping, and bit masks.
- `src/main.S`: Full assembly implementation (clock enable, GPIO setup, polling loop).
- `lib/`, `test/`: Reserved for future modular extensions and tests.

## Hardware mapping assumptions
This version targets the common **Arduino Multi Function Shield** layout:

- Shield buttons: `S1 -> A1`, `S2 -> A2`, `S3 -> A3` (active-low)
- Shield LEDs: `D1 -> D13`, `D2 -> D12`, `D3 -> D11` (active-low)

For `nucleo_l476rg` that means:

- `A1 -> PA1`, `A2 -> PA4`, `A3 -> PB0`
- `D13 -> PA5`, `D12 -> PA6`, `D11 -> PA7`

Important:
- Multi Function Shield LEDs are active-low, so writing `LOW` turns them on.
- If your shield revision has different routing, only edit constants in
  `include/stm32l476rg_shield_map.inc`.

## Build
```bash
~/.platformio/penv/bin/pio run
```

## Upload
```bash
~/.platformio/penv/bin/pio run -t upload
```
