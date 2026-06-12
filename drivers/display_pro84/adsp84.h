/*
 * adsp84.h - driver for the AcmeTech Displays Pro-84 (ADSP84), a 240x240
 * RGB565 SPI panel with a PWM backlight.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * The driver takes TIMER2 for the backlight PWM while the display is open,
 * so don't use TIMER2 elsewhere at the same time.
 */
#ifndef ADSP84_H
#define ADSP84_H

#include <stdint.h>
#include "atp_hal.h"

#define ADSP84_WIDTH   240
#define ADSP84_HEIGHT  240
#define ADSP84_BACKLIGHT_TIMER 2

#define ADSP84_ERR_NO_PANEL (ATP_STATUS_BASE(ATP_MODULE_ADSP84) | 0x0001u) /* 0x00210001 */

typedef struct {
    uint32_t      spi_instance;
    atp_spi_clk_e spi_clock;     /* the panel accepts up to 24 MHz */
    uint32_t      cs_pin;
    uint32_t      dc_pin;
    uint32_t      rst_pin;
    uint32_t      backlight_pin;
} adsp84_config_t;

atp_status_t adsp84_open(const adsp84_config_t *cfg);
atp_status_t adsp84_close(void);
atp_status_t adsp84_fill(uint16_t rgb565);
atp_status_t adsp84_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pixels);
atp_status_t adsp84_text(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg);
atp_status_t adsp84_backlight(uint8_t pct);

#endif /* ADSP84_H */
