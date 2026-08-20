/*
 * atp_hal_uart.c - UART.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include <stdbool.h>
#include "atp_hal.h"

#define UART_COUNT 2
#define UART_BASE(n) ((volatile uint32_t *)(0x40030000u + (n) * 0x1000u))
#define UART_DR    0
#define UART_FR    1
#define UART_BRD   2
#define UART_LCR   3
#define UART_CR    4
#define FR_RXFE    (1u << 4)
#define FR_TXFF    (1u << 5)

struct atp_uart_handle {
    volatile uint32_t *regs;
    bool               open;
};

static struct atp_uart_handle g_uart[UART_COUNT];

atp_status_t atp_hal_uart_init(uint32_t instance, const atp_uart_config_t *cfg, atp_uart_handle_t *handle)
{
    if (instance >= UART_COUNT || cfg == 0 || handle == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    if (g_uart[instance].open) {
        return ATP_STATUS_IN_USE;
    }
#if defined(ATP_CHIP_PRO29)
    if (cfg->flow_control) {
        return ATP_STATUS_INVALID_OPERATION;   /* no RTS/CTS on Pro-29 */
    }
#endif
    /* The UART runs from the core clock divided by 16. */
    uint32_t clk = atp_hal_clkmgr_core_hz() / 16u;
    uint32_t div = (clk + cfg->baudrate / 2u) / cfg->baudrate;
    uint32_t actual = clk / div;
    uint32_t err = actual > cfg->baudrate ? actual - cfg->baudrate : cfg->baudrate - actual;
    if (err * 50u > cfg->baudrate) {
        return ATP_HAL_UART_ERR_BAUD;
    }

    volatile uint32_t *r = UART_BASE(instance);
    r[UART_CR]  = 0;
    r[UART_BRD] = div;
    r[UART_LCR] = (uint32_t)(cfg->data_bits - 5u) | ((uint32_t)cfg->parity << 2) |
                  ((cfg->stop_bits == 2u ? 1u : 0u) << 4);
    r[UART_CR]  = 1u | (cfg->flow_control ? (1u << 3) : 0u);

    g_uart[instance].regs = r;
    g_uart[instance].open = true;
    *handle = &g_uart[instance];
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_uart_deinit(atp_uart_handle_t handle)
{
    if (handle == 0 || !handle->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    handle->regs[UART_CR] = 0;
    handle->open = false;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_uart_write(atp_uart_handle_t handle, const uint8_t *buf, size_t len)
{
    if (handle == 0 || !handle->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    for (size_t i = 0; i < len; i++) {
        while (handle->regs[UART_FR] & FR_TXFF) { }
        handle->regs[UART_DR] = buf[i];
    }
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_uart_read(atp_uart_handle_t handle, uint8_t *buf, size_t len, size_t *got, uint32_t timeout_ms)
{
    if (handle == 0 || !handle->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    size_t n = 0;
    uint32_t start = atp_hal_tick_ms();
    while (n < len) {
        if ((handle->regs[UART_FR] & FR_RXFE) == 0) {
            uint32_t dr = handle->regs[UART_DR];
            if (dr & (1u << 11)) {
                *got = n;
                return ATP_HAL_UART_ERR_OVERRUN;
            }
            buf[n++] = (uint8_t)dr;
            continue;   /* drain the FIFO before checking the clock (1.4.2 fix) */
        }
        if (atp_hal_tick_ms() - start >= timeout_ms) {
            break;
        }
    }
    *got = n;
    return n == len ? ATP_STATUS_SUCCESS : ATP_STATUS_TIMEOUT;
}
