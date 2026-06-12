/*
 * atp_pinmux_pro29.c - which pins each function can use on the Pro-29.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Pro-29 has 32 pins, 0-31. Pins 30 and 31 are SWDIO and SWCLK and are
 * reserved while a debugger is attached.
 */
#if defined(ATP_CHIP_PRO29)

#include "atp_hal.h"
#include "atp_pinmux.h"

const atp_pinmux_entry_t g_atp_pinmux[] = {
    { ATP_GPIO_FN_UART0_TX,  { 6, 18, 24 } },
    { ATP_GPIO_FN_UART0_RX,  { 7, 19, 25 } },
    { ATP_GPIO_FN_UART1_TX,  { 14, 26 } },
    { ATP_GPIO_FN_UART1_RX,  { 15, 27 } },
    { ATP_GPIO_FN_I2C0_SCL,  { 4, 20 } },
    { ATP_GPIO_FN_I2C0_SDA,  { 5, 21 } },
    { ATP_GPIO_FN_I2C1_SCL,  { 12 } },
    { ATP_GPIO_FN_I2C1_SDA,  { 13 } },
    { ATP_GPIO_FN_SPI0_SCK,  { 8, 28 } },
    { ATP_GPIO_FN_SPI0_MOSI, { 9, 29 } },
    { ATP_GPIO_FN_SPI0_MISO, { 10 } },
    { ATP_GPIO_FN_TIMER_OUT, { 0, 1, 2, 3, 16, 17, 22, 23 } },
};

const uint32_t g_atp_pinmux_count = sizeof(g_atp_pinmux) / sizeof(g_atp_pinmux[0]);
const uint32_t g_atp_pin_count = 32;
const uint8_t  g_atp_swd_pins[2] = { 30, 31 };

#endif /* ATP_CHIP_PRO29 */
