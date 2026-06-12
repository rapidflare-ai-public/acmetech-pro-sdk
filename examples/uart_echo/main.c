/*
 * uart_echo - echo every byte received on the debug UART.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Uses UART1 on its own pins so the debug UART stays free for messages.
 */
#include "bsp.h"

#if defined(ATP_BOARD_PRO29_EVK)
#define ECHO_TX 14
#define ECHO_RX 15
#else
#define ECHO_TX 6
#define ECHO_RX 7
#endif

int main(void)
{
    bsp_init();
    bsp_printf("uart_echo: UART1 TX=%d RX=%d at 921600\n", ECHO_TX, ECHO_RX);

    atp_gpio_cfg_t tx = { .fn = ATP_GPIO_FN_UART1_TX };
    atp_gpio_cfg_t rx = { .fn = ATP_GPIO_FN_UART1_RX, .pull = ATP_GPIO_PULL_UP };
    atp_hal_gpio_pinconfig(ECHO_TX, &tx);
    atp_hal_gpio_pinconfig(ECHO_RX, &rx);

    atp_uart_config_t cfg = { .baudrate = 921600, .data_bits = 8, .stop_bits = 1 };
    atp_uart_handle_t uart;
    atp_status_t st = atp_hal_uart_init(1, &cfg, &uart);
    if (st != ATP_STATUS_SUCCESS) {
        bsp_printf("uart init failed: 0x%08lx\n", (unsigned long)st);
        for (;;) { }
    }

    for (;;) {
        uint8_t b;
        size_t got = 0;
        if (atp_hal_uart_read(uart, &b, 1, &got, 1000) == ATP_STATUS_SUCCESS && got == 1) {
            atp_hal_uart_write(uart, &b, 1);
        }
    }
}
