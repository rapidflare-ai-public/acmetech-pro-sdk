# Changelog

All notable changes to the AcmeTech Pro SDK. (Fictional demo content; see NOTICE.md.)

## 2.1.0

See [docs/release_notes/2.1.0.md](docs/release_notes/2.1.0.md) for a required step on Pro-100 rev B.

- New `atp_hal_pwr_trim_apply()` and `atp_hal_pwr_chip_rev()`.
- SPI: `ATP_SPI_CLK_32MHZ` on Pro-100.
- `atpflash --verify`.
- I2C: `atp_hal_i2c_recover()` stops as soon as SDA is released.

## 2.0.0

**Breaking:** see [docs/migration/v1-to-v2.md](docs/migration/v1-to-v2.md).

- New clock manager (`atp_hal_clkmgr.h`) replaces `atp_hal_clk.h`. Clocks are
  reference counted per user.
- Pro-29: HFRC_96M is calibrated against the LFRC once a second
  (`ATP_CFG_CLKMGR_AUTOCAL`, on by default). 921600-baud UART now holds across
  temperature.
- `bsp_init()` uses the clock manager.

## 1.4.2

- UART: fixed a lost byte when the RX FIFO filled during `atp_hal_uart_read()`.
- ACAM37: `acam37_init()` now waits 20 ms after reset before the first register write.

## 1.4.1

- Pro-100 EVK: corrected the user LED pin.
- `atpflash`: clearer message when no board is connected.

## 1.4.0

- New driver for AcmeTech Displays Pro-84 (`drivers/display_pro84`).
- New example `display_hello`.
- SPI: 24 MHz clock option.

## 1.3.0

- New driver for AcmeTech Cameras Pro-37 (`drivers/camera_pro37`).
- New examples `camera_capture` and `i2c_scan`.
