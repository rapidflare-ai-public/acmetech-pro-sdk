# Moving from 1.x to 2.x

(Fictional demo content; see NOTICE.md.)

Version 2.0.0 replaces the clock API with a reference-counted clock manager.
Everything else keeps its name and behavior.

## Clocks

`atp_hal_clk.h` is gone. Include `atp_hal.h` as before; it now pulls in
`atp_hal_clkmgr.h`.

| 1.x | 2.x |
|---|---|
| `atp_hal_clk_enable(ATP_CLK_SRC_HFRC_96M)` | `atp_hal_clkmgr_request(ATP_CLKMGR_USER_APP, ATP_CLKMGR_CLK_HFRC_96M)` |
| `atp_hal_clk_disable(ATP_CLK_SRC_HFRC_96M)` | `atp_hal_clkmgr_release(ATP_CLKMGR_USER_APP, ATP_CLKMGR_CLK_HFRC_96M)` |
| `atp_hal_clk_core_set(src)` | `atp_hal_clkmgr_core_set(clk)` (no separate enable needed) |
| `atp_hal_clk_core_hz()` | `atp_hal_clkmgr_core_hz()` |
| `ATP_CLK_SRC_*` | `ATP_CLKMGR_CLK_*` |

In 1.x one `atp_hal_clk_disable()` turned a clock off for everyone. In 2.x a
clock stays on until every user that requested it has released it, so pick
the `ATP_CLKMGR_USER_*` value that matches your code and release with the same
one.

## Board code

If you copied `bsp_init()` from 1.x, replace the enable and `core_set` pair
with one `atp_hal_clkmgr_core_set()` call. See `boards/common/bsp.c`.

## Nothing else changes

UART, I2C, SPI, GPIO, timers, power and the camera and display drivers have
the same API as 1.4.2.
