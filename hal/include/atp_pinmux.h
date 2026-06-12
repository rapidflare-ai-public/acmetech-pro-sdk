/*
 * atp_pinmux.h - pin mux tables. Internal; one table per chip.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_PINMUX_H
#define ATP_PINMUX_H

#include <stdint.h>
#include "atp_hal_gpio.h"

#define ATP_PINMUX_MAX_PINS 12

typedef struct {
    atp_gpio_fn_e fn;
    uint8_t       pins[ATP_PINMUX_MAX_PINS];   /* unused slots are 0 after the first entry */
} atp_pinmux_entry_t;

extern const atp_pinmux_entry_t g_atp_pinmux[];
extern const uint32_t           g_atp_pinmux_count;
extern const uint32_t           g_atp_pin_count;
extern const uint8_t            g_atp_swd_pins[2];

#endif /* ATP_PINMUX_H */
