/*
 * atp_hal_gpio.h - pins, pin functions and GPIO.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Each pin can take only some peripheral functions. The allowed pins for each
 * function are listed per chip in hal/src/atp_pinmux_pro29.c and
 * hal/src/atp_pinmux_pro100.c. atp_hal_gpio_pinconfig() returns
 * ATP_HAL_GPIO_ERR_FUNC_NOT_ON_PIN for any other pin.
 */
#ifndef ATP_HAL_GPIO_H
#define ATP_HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include "atp_status.h"

#define ATP_HAL_GPIO_ERR_FUNC_NOT_ON_PIN (ATP_STATUS_BASE(ATP_MODULE_GPIO) | 0x0001u)
#define ATP_HAL_GPIO_ERR_PIN_RESERVED    (ATP_STATUS_BASE(ATP_MODULE_GPIO) | 0x0002u) /* SWD pins */

typedef enum {
    ATP_GPIO_FN_GPIO = 0,
    ATP_GPIO_FN_UART0_TX, ATP_GPIO_FN_UART0_RX,
    ATP_GPIO_FN_UART1_TX, ATP_GPIO_FN_UART1_RX,
    ATP_GPIO_FN_I2C0_SCL, ATP_GPIO_FN_I2C0_SDA,
    ATP_GPIO_FN_I2C1_SCL, ATP_GPIO_FN_I2C1_SDA,
    ATP_GPIO_FN_SPI0_SCK, ATP_GPIO_FN_SPI0_MOSI, ATP_GPIO_FN_SPI0_MISO,
    ATP_GPIO_FN_TIMER_OUT,          /* PWM output of the timer chosen in the timer config */
    ATP_GPIO_FN_COUNT
} atp_gpio_fn_e;

typedef enum { ATP_GPIO_DIR_IN, ATP_GPIO_DIR_OUT } atp_gpio_dir_e;
typedef enum { ATP_GPIO_PULL_NONE, ATP_GPIO_PULL_UP, ATP_GPIO_PULL_DOWN } atp_gpio_pull_e;

typedef struct {
    atp_gpio_fn_e   fn;
    atp_gpio_dir_e  dir;     /* GPIO function only */
    atp_gpio_pull_e pull;
    bool            wake_on_edge;  /* GPIO input only: wake from deep sleep */
} atp_gpio_cfg_t;

atp_status_t atp_hal_gpio_pinconfig(uint32_t pin, const atp_gpio_cfg_t *cfg);
void         atp_hal_gpio_write(uint32_t pin, bool level);
bool         atp_hal_gpio_read(uint32_t pin);
void         atp_hal_gpio_toggle(uint32_t pin);

/* Pins a function can use on this chip. Returns the count; pins[] gets up to max. */
uint32_t     atp_hal_gpio_pins_for(atp_gpio_fn_e fn, uint8_t *pins, uint32_t max);

#endif /* ATP_HAL_GPIO_H */
