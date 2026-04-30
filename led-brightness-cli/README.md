# Brightness control via PWM for LD2 on STM32L476RG

This project controls the Nucleo-L476RG LD2 LED with TIM2 PWM on PA5. Commands
arrive through the ST-LINK virtual COM port on USART2 at 115200 baud.

## Command convention

Use the PlatformIO or VS Code serial monitor and send one command per line:

```text
up <percent>
down <percent>
```

Examples:

```text
up 25
down 10
```

The firmware keeps brightness in the inclusive range `0..100`. Values that
would exceed the range are clamped to `0%` or `100%`.

## Build

```sh
pio run -e nucleo_l476rg
```

On this machine, if `pio` is not on `PATH`, the available PlatformIO command may
be in a project virtual environment such as:

```sh
../turn-signals/.venv/bin/pio run -e nucleo_l476rg
```

## Outputs

Store PulseView captures in `outputs/` as JPG files after sending commands from
the serial monitor. Suggested names are `up-25.jpg`, `down-10.jpg`, or another
name that records the command used for the capture.
