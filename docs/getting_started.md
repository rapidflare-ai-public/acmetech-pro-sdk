# Getting started

(Fictional demo content; see NOTICE.md.)

## 1. Pick your chip and board

Every build defines exactly one chip and one board:

| Define | Use it for |
|---|---|
| `ATP_CHIP_PRO29` | AcmeTech Microcontrollers Pro-29 |
| `ATP_CHIP_PRO100` | AcmeTech Microcontrollers Pro-100 |
| `ATP_BOARD_PRO29_EVK` | Pro-29 Evaluation Kit |
| `ATP_BOARD_PRO100_EVK` | Pro-100 Evaluation Kit |

## 2. Initialize the board

```c
#include "atp_hal.h"
#include "bsp.h"

int main(void)
{
    bsp_init();          // clocks, debug UART, LEDs
    bsp_printf("hello\n");
    for (;;) { }
}
```

`bsp_init()` brings the core up to its full clock, opens the debug UART at
115200 baud and configures the user LEDs.

## 3. Flash it

```
python tools/atpflash/atpflash.py --board pro29_evk build/app.bin
```

## Where to look next

- `examples/blinky` toggles the user LED.
- `examples/uart_echo` echoes characters on the debug UART.
- `examples/deepsleep_wake` sleeps and wakes on a timer.
