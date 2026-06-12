/*
 * atp_pinmux_pro100.c - which pins each function can use on the Pro-100.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Pro-100 has 64 pins, 0-63. Pins 62 and 63 are SWDIO and SWCLK and are
 * reserved while a debugger is attached. UART2/UART3, I2C2 and SPI1 also
 * exist on Pro-100 but this SDK version has no HAL support for them.
 */
#if defined(ATP_CHIP_PRO100)

#include "atp_hal.h"
#include "atp_pinmux.h"

const atp_pinmux_entry_t g_atp_pinmux[] = {
    { ATP_GPIO_FN_UART0_TX,  { 14, 40 } },
    { ATP_GPIO_FN_UART0_RX,  { 15, 41 } },
    { ATP_GPIO_FN_UART1_TX,  { 6, 32, 50 } },
    { ATP_GPIO_FN_UART1_RX,  { 7, 33, 51 } },
    { ATP_GPIO_FN_I2C0_SCL,  { 4, 36 } },
    { ATP_GPIO_FN_I2C0_SDA,  { 5, 37 } },
    { ATP_GPIO_FN_I2C1_SCL,  { 20, 44 } },
    { ATP_GPIO_FN_I2C1_SDA,  { 21, 45 } },
    { ATP_GPIO_FN_SPI0_SCK,  { 8, 24 } },
    { ATP_GPIO_FN_SPI0_MOSI, { 9, 25 } },
    { ATP_GPIO_FN_SPI0_MISO, { 10, 26 } },
    { ATP_GPIO_FN_TIMER_OUT, { 0, 1, 2, 3, 16, 17, 18, 19, 52, 53, 54, 55 } },
};

const uint32_t g_atp_pinmux_count = sizeof(g_atp_pinmux) / sizeof(g_atp_pinmux[0]);
const uint32_t g_atp_pin_count = 64;
const uint8_t  g_atp_swd_pins[2] = { 62, 63 };

#endif /* ATP_CHIP_PRO100 */
