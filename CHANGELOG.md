# Changelog

All notable changes to the AcmeTech Pro SDK. (Fictional demo content; see NOTICE.md.)

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
