/*
 * atp_hal_spi.c - SPI controller.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include <stdbool.h>
#include "atp_hal.h"

#if defined(ATP_CHIP_PRO29)
#define SPI_COUNT 1
#else
#define SPI_COUNT 2
#endif

#define SPI_BASE(n)  ((volatile uint32_t *)(0x40050000u + (n) * 0x1000u))
#define SPI_CTRL     0
#define SPI_STAT     1
#define SPI_DATA     2
#define SPI_CLKDIV   3

struct atp_spi_handle {
    volatile uint32_t *regs;
    uint32_t           cs_pin;
    bool               open;
};

static struct atp_spi_handle g_spi[SPI_COUNT];

static const uint32_t k_clk_hz[] = { 1000000u, 4000000u, 12000000u, 24000000u };

atp_status_t atp_hal_spi_init(uint32_t instance, const atp_spi_config_t *cfg, atp_spi_handle_t *handle)
{
    if (instance >= SPI_COUNT || cfg == 0 || handle == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    uint32_t core = atp_hal_clkmgr_core_hz();
    uint32_t want = k_clk_hz[cfg->clock];
#if ATP_HAL_VALIDATE
    if (want > core / 4u) {
        return ATP_STATUS_OUT_OF_RANGE;
    }
#else
    if (want > core / 4u) {
        want = core / 4u;   /* clamp silently; build with ATP_HAL_VALIDATE=1 to get an error */
    }
#endif
    volatile uint32_t *r = SPI_BASE(instance);
    r[SPI_CTRL]   = 0;
    r[SPI_CLKDIV] = core / want;
    r[SPI_CTRL]   = 1u | ((uint32_t)cfg->mode << 1);

    atp_gpio_cfg_t cs = { .fn = ATP_GPIO_FN_GPIO, .dir = ATP_GPIO_DIR_OUT };
    atp_hal_gpio_pinconfig(cfg->cs_pin, &cs);
    atp_hal_gpio_write(cfg->cs_pin, true);

    g_spi[instance].regs = r;
    g_spi[instance].cs_pin = cfg->cs_pin;
    g_spi[instance].open = true;
    *handle = &g_spi[instance];
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_spi_deinit(atp_spi_handle_t h)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    h->regs[SPI_CTRL] = 0;
    h->open = false;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_spi_transfer(atp_spi_handle_t h, const uint8_t *tx, uint8_t *rx, size_t len)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    atp_hal_gpio_write(h->cs_pin, false);
    for (size_t i = 0; i < len; i++) {
        h->regs[SPI_DATA] = tx ? tx[i] : 0xFFu;
        while ((h->regs[SPI_STAT] & 1u) == 0) { }
        uint8_t b = (uint8_t)h->regs[SPI_DATA];
        if (rx) {
            rx[i] = b;
        }
    }
    atp_hal_gpio_write(h->cs_pin, true);
    return ATP_STATUS_SUCCESS;
}
