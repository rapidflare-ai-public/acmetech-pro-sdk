/*
 * atp_hal_gpio.c - pins, pin functions and GPIO.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "atp_hal.h"
#include "atp_pinmux.h"

#define GPIO_OUT     ((volatile uint32_t *)0x40020000u)
#define GPIO_IN      ((volatile uint32_t *)0x40020010u)
#define GPIO_PADCFG  ((volatile uint32_t *)0x40020100u)

static bool fn_allowed_on_pin(atp_gpio_fn_e fn, uint32_t pin)
{
    if (fn == ATP_GPIO_FN_GPIO) {
        return true;
    }
    for (uint32_t i = 0; i < g_atp_pinmux_count; i++) {
        if (g_atp_pinmux[i].fn != fn) {
            continue;
        }
        for (uint32_t j = 0; j < ATP_PINMUX_MAX_PINS; j++) {
            if (j > 0 && g_atp_pinmux[i].pins[j] == 0) {
                break;
            }
            if (g_atp_pinmux[i].pins[j] == pin) {
                return true;
            }
        }
    }
    return false;
}

atp_status_t atp_hal_gpio_pinconfig(uint32_t pin, const atp_gpio_cfg_t *cfg)
{
    if (pin >= g_atp_pin_count || cfg == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    if (pin == g_atp_swd_pins[0] || pin == g_atp_swd_pins[1]) {
        return ATP_HAL_GPIO_ERR_PIN_RESERVED;
    }
    if (!fn_allowed_on_pin(cfg->fn, pin)) {
        return ATP_HAL_GPIO_ERR_FUNC_NOT_ON_PIN;
    }
    uint32_t v = ((uint32_t)cfg->fn << 0) | ((uint32_t)cfg->pull << 8) |
                 ((uint32_t)cfg->dir << 12) | ((cfg->wake_on_edge ? 1u : 0u) << 16);
    GPIO_PADCFG[pin] = v;
    return ATP_STATUS_SUCCESS;
}

void atp_hal_gpio_write(uint32_t pin, bool level)
{
    if (level) {
        GPIO_OUT[pin / 32] |= (1u << (pin % 32));
    } else {
        GPIO_OUT[pin / 32] &= ~(1u << (pin % 32));
    }
}

bool atp_hal_gpio_read(uint32_t pin)
{
    return (GPIO_IN[pin / 32] >> (pin % 32)) & 1u;
}

void atp_hal_gpio_toggle(uint32_t pin)
{
    GPIO_OUT[pin / 32] ^= (1u << (pin % 32));
}

uint32_t atp_hal_gpio_pins_for(atp_gpio_fn_e fn, uint8_t *pins, uint32_t max)
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < g_atp_pinmux_count; i++) {
        if (g_atp_pinmux[i].fn != fn) {
            continue;
        }
        for (uint32_t j = 0; j < ATP_PINMUX_MAX_PINS; j++) {
            if (j > 0 && g_atp_pinmux[i].pins[j] == 0) {
                break;
            }
            if (n < max) {
                pins[n] = g_atp_pinmux[i].pins[j];
            }
            n++;
        }
    }
    return n;
}
