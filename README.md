# AcmeTech Pro SDK

> **Fictional demo content.** AcmeTech and every product in this repository are
> made up. Rapidflare publishes it to demonstrate code search. Nothing here
> builds, runs or describes real hardware. See [NOTICE.md](NOTICE.md).

The AcmeTech Pro SDK is the hardware abstraction layer (HAL), board support and
peripheral drivers for the AcmeTech Pro family of microcontrollers.

**Version 1.4.2**

## Supported parts

| Part | Short name | Max clock | Notes |
|---|---|---|---|
| AcmeTech Microcontrollers Pro-29 | ATP29 | 96 MHz | 8 timers, 2 UARTs, 2 I2C, 1 SPI |
| AcmeTech Microcontrollers Pro-100 | ATP100 | 192 MHz | 12 timers, 4 UARTs, 3 I2C, 2 SPI |

| Board | Folder |
|---|---|
| Pro-29 Evaluation Kit | `boards/pro29_evk/` |
| Pro-100 Evaluation Kit | `boards/pro100_evk/` |

| Peripheral | Driver |
|---|---|
| AcmeTech Cameras Pro-37 (ACAM37) | `drivers/camera_pro37/` |
| AcmeTech Displays Pro-84 (ADSP84) | `drivers/display_pro84/` |

## Layout

```
include/        status codes shared by every module
hal/            the HAL: clocks, power, GPIO, UART, I2C, SPI, timers
boards/         pin maps and board init for each evaluation kit
drivers/        drivers for AcmeTech cameras and displays
examples/       small programs, one folder each
tools/atpflash  the flashing tool
docs/           getting started guide
```

## Getting started

See [docs/getting_started.md](docs/getting_started.md). Pick your chip with
`ATP_CHIP_PRO29` or `ATP_CHIP_PRO100` and your board with `ATP_BOARD_PRO29_EVK`
or `ATP_BOARD_PRO100_EVK`.
