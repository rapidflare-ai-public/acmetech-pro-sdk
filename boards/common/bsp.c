/*
 * bsp.c - board support shared by every evaluation kit.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include <stdarg.h>
#include <stdio.h>
#include "bsp.h"

static atp_uart_handle_t g_debug_uart;

static atp_status_t open_debug_uart(void)
{
    atp_gpio_cfg_t tx = { .fn = ATP_GPIO_FN_UART0_TX };
    atp_gpio_cfg_t rx = { .fn = ATP_GPIO_FN_UART0_RX, .pull = ATP_GPIO_PULL_UP };
    atp_hal_gpio_pinconfig(BSP_DEBUG_UART_TX, &tx);
    atp_hal_gpio_pinconfig(BSP_DEBUG_UART_RX, &rx);

    atp_uart_config_t cfg = {
        .baudrate  = 115200,
        .data_bits = 8,
        .parity    = ATP_UART_PARITY_NONE,
        .stop_bits = 1,
    };
    return atp_hal_uart_init(BSP_DEBUG_UART, &cfg, &g_debug_uart);
}

atp_status_t bsp_init(void)
{
    atp_status_t st;

    st = atp_hal_clk_enable(ATP_CLK_SRC_HFRC_96M);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }
#if defined(ATP_CHIP_PRO100)
    st = atp_hal_clk_enable(ATP_CLK_SRC_PLL_192M);
    if (st == ATP_STATUS_SUCCESS) {
        st = atp_hal_clk_core_set(ATP_CLK_SRC_PLL_192M);
    }
#else
    st = atp_hal_clk_core_set(ATP_CLK_SRC_HFRC_96M);
#endif
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }

    atp_hal_timer_tick_init();

    atp_gpio_cfg_t led = { .fn = ATP_GPIO_FN_GPIO, .dir = ATP_GPIO_DIR_OUT };
    atp_hal_gpio_pinconfig(BSP_LED0, &led);
    atp_hal_gpio_pinconfig(BSP_LED1, &led);
    atp_hal_gpio_write(BSP_LED0, false);
    atp_hal_gpio_write(BSP_LED1, false);

    return open_debug_uart();
}

int bsp_printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) {
        atp_hal_uart_write(g_debug_uart, (const uint8_t *)buf, (size_t)(n < 256 ? n : 255));
    }
    return n;
}

void bsp_led_set(uint32_t led, bool on)
{
    atp_hal_gpio_write(led, on);
}

void bsp_led_toggle(uint32_t led)
{
    atp_hal_gpio_toggle(led);
}

void bsp_prepare_sleep(void)
{
    atp_hal_uart_deinit(g_debug_uart);
}

void bsp_resume_from_sleep(void)
{
    open_debug_uart();
}
