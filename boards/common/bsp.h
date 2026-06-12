/*
 * bsp.h - board support shared by every evaluation kit.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef BSP_H
#define BSP_H

#include "atp_hal.h"
#include "bsp_pins.h"

/* Clocks to full speed, HAL tick, debug UART at 115200, LEDs off. */
atp_status_t bsp_init(void);

/* printf to the debug UART. */
int  bsp_printf(const char *fmt, ...);

void bsp_led_set(uint32_t led, bool on);
void bsp_led_toggle(uint32_t led);

/* Before deep sleep: close the debug UART and park unused pins.
 * After wake: reopen the debug UART. */
void bsp_prepare_sleep(void);
void bsp_resume_from_sleep(void);

#endif /* BSP_H */
