/*
 * adsp84.c - driver for the AcmeTech Displays Pro-84 (ADSP84).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "adsp84.h"

#define CMD_SWRESET  0x01
#define CMD_SLPOUT   0x11
#define CMD_DISPON   0x29
#define CMD_CASET    0x2A
#define CMD_RASET    0x2B
#define CMD_RAMWR    0x2C
#define CMD_RDID     0x04

static atp_spi_handle_t g_spi;
static uint32_t         g_dc;

static void cmd(uint8_t c)
{
    atp_hal_gpio_write(g_dc, false);
    atp_hal_spi_transfer(g_spi, &c, 0, 1);
    atp_hal_gpio_write(g_dc, true);
}

static void data(const uint8_t *d, size_t n)
{
    atp_hal_spi_transfer(g_spi, d, 0, n);
}

static void window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint8_t c[4] = { (uint8_t)(x >> 8), (uint8_t)x, (uint8_t)((x + w - 1) >> 8), (uint8_t)(x + w - 1) };
    uint8_t r[4] = { (uint8_t)(y >> 8), (uint8_t)y, (uint8_t)((y + h - 1) >> 8), (uint8_t)(y + h - 1) };
    cmd(CMD_CASET); data(c, 4);
    cmd(CMD_RASET); data(r, 4);
    cmd(CMD_RAMWR);
}

atp_status_t adsp84_open(const adsp84_config_t *cfg)
{
    if (cfg == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    atp_spi_config_t spi = { .clock = cfg->spi_clock, .mode = ATP_SPI_MODE0, .cs_pin = cfg->cs_pin };
    atp_status_t st = atp_hal_spi_init(cfg->spi_instance, &spi, &g_spi);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }

    g_dc = cfg->dc_pin;
    atp_gpio_cfg_t out = { .fn = ATP_GPIO_FN_GPIO, .dir = ATP_GPIO_DIR_OUT };
    atp_hal_gpio_pinconfig(cfg->dc_pin, &out);
    atp_hal_gpio_pinconfig(cfg->rst_pin, &out);

    atp_hal_gpio_write(cfg->rst_pin, false);
    atp_hal_delay_ms(10);
    atp_hal_gpio_write(cfg->rst_pin, true);
    atp_hal_delay_ms(120);

    uint8_t id[3] = { 0 };
    cmd(CMD_RDID);
    atp_hal_spi_transfer(g_spi, 0, id, 3);
    if (id[0] == 0x00 || id[0] == 0xFF) {
        return ADSP84_ERR_NO_PANEL;
    }

    cmd(CMD_SWRESET);
    atp_hal_delay_ms(120);
    cmd(CMD_SLPOUT);
    atp_hal_delay_ms(120);
    cmd(CMD_DISPON);

    atp_timer_config_t bl = {
        .mode = ATP_TIMER_PWM,
        .period_us = 1000,      /* 1 kHz */
        .duty_pct = 80,
        .out_pin = cfg->backlight_pin,
    };
    st = atp_hal_timer_config(ADSP84_BACKLIGHT_TIMER, &bl);
    if (st == ATP_STATUS_SUCCESS) {
        st = atp_hal_timer_start(ADSP84_BACKLIGHT_TIMER);
    }
    return st;
}

atp_status_t adsp84_close(void)
{
    atp_hal_timer_stop(ADSP84_BACKLIGHT_TIMER);
    return atp_hal_spi_deinit(g_spi);
}

atp_status_t adsp84_fill(uint16_t rgb565)
{
    uint8_t px[2] = { (uint8_t)(rgb565 >> 8), (uint8_t)rgb565 };
    window(0, 0, ADSP84_WIDTH, ADSP84_HEIGHT);
    for (uint32_t i = 0; i < (uint32_t)ADSP84_WIDTH * ADSP84_HEIGHT; i++) {
        data(px, 2);
    }
    return ATP_STATUS_SUCCESS;
}

atp_status_t adsp84_draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pixels)
{
    if (x + w > ADSP84_WIDTH || y + h > ADSP84_HEIGHT) {
        return ATP_STATUS_OUT_OF_RANGE;
    }
    window(x, y, w, h);
    data((const uint8_t *)pixels, (size_t)w * h * 2u);
    return ATP_STATUS_SUCCESS;
}

atp_status_t adsp84_text(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg)
{
    /* 8x8 font rendering omitted in this demo. */
    (void)x; (void)y; (void)s; (void)fg; (void)bg;
    return ATP_STATUS_SUCCESS;
}

atp_status_t adsp84_backlight(uint8_t pct)
{
    return atp_hal_timer_set_duty(ADSP84_BACKLIGHT_TIMER, pct);
}
