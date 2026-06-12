/*
 * atp_hal_uart.h - UART.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Pro-29 has UART0 and UART1. Pro-100 has UART0-UART3; the HAL supports
 * UART0 and UART1 on both. Pins are set separately with
 * atp_hal_gpio_pinconfig() and the ATP_GPIO_FN_UARTn_TX/RX functions.
 */
#ifndef ATP_HAL_UART_H
#define ATP_HAL_UART_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "atp_status.h"

#define ATP_HAL_UART_ERR_FRAMING  (ATP_STATUS_BASE(ATP_MODULE_UART) | 0x0001u)
#define ATP_HAL_UART_ERR_PARITY   (ATP_STATUS_BASE(ATP_MODULE_UART) | 0x0002u)
#define ATP_HAL_UART_ERR_OVERRUN  (ATP_STATUS_BASE(ATP_MODULE_UART) | 0x0003u)
#define ATP_HAL_UART_ERR_BAUD     (ATP_STATUS_BASE(ATP_MODULE_UART) | 0x0004u) /* more than 2% off at this core clock */

typedef enum { ATP_UART_PARITY_NONE, ATP_UART_PARITY_EVEN, ATP_UART_PARITY_ODD } atp_uart_parity_e;

typedef struct {
    uint32_t          baudrate;       /* 1200 - 3000000 */
    uint8_t           data_bits;      /* 7 or 8 */
    atp_uart_parity_e parity;
    uint8_t           stop_bits;      /* 1 or 2 */
    bool              flow_control;   /* RTS/CTS, Pro-100 only */
} atp_uart_config_t;

typedef struct atp_uart_handle *atp_uart_handle_t;

atp_status_t atp_hal_uart_init(uint32_t instance, const atp_uart_config_t *cfg, atp_uart_handle_t *handle);
atp_status_t atp_hal_uart_deinit(atp_uart_handle_t handle);
atp_status_t atp_hal_uart_write(atp_uart_handle_t handle, const uint8_t *buf, size_t len);
/* Reads up to len bytes, waiting at most timeout_ms. *got is the count read. */
atp_status_t atp_hal_uart_read(atp_uart_handle_t handle, uint8_t *buf, size_t len, size_t *got, uint32_t timeout_ms);

#endif /* ATP_HAL_UART_H */
