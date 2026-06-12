/*
 * atp_hal_i2c.c - I2C controller.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "atp_hal.h"

#if defined(ATP_CHIP_PRO29)
#define I2C_COUNT 2
#else
#define I2C_COUNT 3
#endif

#define I2C_BASE(n)  ((volatile uint32_t *)(0x40040000u + (n) * 0x1000u))
#define I2C_CTRL     0
#define I2C_STAT     1
#define I2C_DATA     2
#define I2C_CLKDIV   3
#define I2C_ADDR     4

#define STAT_DONE     (1u << 0)
#define STAT_ADDR_NAK (1u << 1)
#define STAT_DATA_NAK (1u << 2)
#define STAT_ARBLOST  (1u << 3)
#define STAT_SDA_LOW  (1u << 4)
#define STAT_SCL_LOW  (1u << 5)

struct atp_i2c_handle {
    volatile uint32_t *regs;
    uint32_t           stretch_timeout_ms;
    bool               open;
};

static struct atp_i2c_handle g_i2c[I2C_COUNT];

static const uint32_t k_speed_hz[] = { 100000u, 400000u, 1000000u };

atp_status_t atp_hal_i2c_init(uint32_t instance, const atp_i2c_config_t *cfg, atp_i2c_handle_t *handle)
{
    if (instance >= I2C_COUNT || cfg == 0 || handle == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    uint32_t core = atp_hal_clk_core_hz();
    if (cfg->speed == ATP_I2C_SPEED_1M && core < 96000000u) {
        return ATP_STATUS_OUT_OF_RANGE;
    }
    volatile uint32_t *r = I2C_BASE(instance);
    r[I2C_CTRL]   = 0;
    r[I2C_CLKDIV] = core / (2u * k_speed_hz[cfg->speed]);
    r[I2C_CTRL]   = 1u;

    g_i2c[instance].regs = r;
    g_i2c[instance].stretch_timeout_ms =
        cfg->stretch_timeout_ms ? cfg->stretch_timeout_ms : ATP_CFG_BLOCKING_TIMEOUT_MS;
    g_i2c[instance].open = true;
    *handle = &g_i2c[instance];
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_i2c_deinit(atp_i2c_handle_t h)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    h->regs[I2C_CTRL] = 0;
    h->open = false;
    return ATP_STATUS_SUCCESS;
}

/* Wait for the current byte, mapping the status bits to a status code. */
static atp_status_t wait_byte(atp_i2c_handle_t h)
{
    uint32_t start = atp_hal_tick_ms();
    for (;;) {
        uint32_t s = h->regs[I2C_STAT];
        if (s & STAT_ARBLOST)  return ATP_HAL_I2C_ERR_ARB_LOST;
        if (s & STAT_ADDR_NAK) return ATP_HAL_I2C_ERR_ADDR_NAK;
        if (s & STAT_DATA_NAK) return ATP_HAL_I2C_ERR_DATA_NAK;
        if (s & STAT_DONE)     return ATP_STATUS_SUCCESS;
        if (atp_hal_tick_ms() - start >= h->stretch_timeout_ms) {
            if (s & STAT_SDA_LOW) return ATP_HAL_I2C_ERR_BUS_STUCK;
            if (s & STAT_SCL_LOW) return ATP_HAL_I2C_ERR_STRETCH_TIMEOUT;
            return ATP_STATUS_TIMEOUT;
        }
    }
}

atp_status_t atp_hal_i2c_write(atp_i2c_handle_t h, uint8_t addr, const uint8_t *buf, size_t len, bool stop)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    h->regs[I2C_ADDR] = (uint32_t)addr << 1;
    atp_status_t st = wait_byte(h);
    for (size_t i = 0; st == ATP_STATUS_SUCCESS && i < len; i++) {
        h->regs[I2C_DATA] = buf[i];
        st = wait_byte(h);
    }
    if (stop || st != ATP_STATUS_SUCCESS) {
        h->regs[I2C_CTRL] |= (1u << 2);   /* STOP */
    }
    return st;
}

atp_status_t atp_hal_i2c_read(atp_i2c_handle_t h, uint8_t addr, uint8_t *buf, size_t len)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    h->regs[I2C_ADDR] = ((uint32_t)addr << 1) | 1u;
    atp_status_t st = wait_byte(h);
    for (size_t i = 0; st == ATP_STATUS_SUCCESS && i < len; i++) {
        st = wait_byte(h);
        buf[i] = (uint8_t)h->regs[I2C_DATA];
    }
    h->regs[I2C_CTRL] |= (1u << 2);
    return st;
}

atp_status_t atp_hal_i2c_write_read(atp_i2c_handle_t h, uint8_t addr,
                                    const uint8_t *wbuf, size_t wlen, uint8_t *rbuf, size_t rlen)
{
    atp_status_t st = atp_hal_i2c_write(h, addr, wbuf, wlen, false);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }
    return atp_hal_i2c_read(h, addr, rbuf, rlen);
}

atp_status_t atp_hal_i2c_recover(atp_i2c_handle_t h)
{
    if (h == 0 || !h->open) {
        return ATP_STATUS_INVALID_HANDLE;
    }
    for (int i = 0; i < 9 && (h->regs[I2C_STAT] & STAT_SDA_LOW); i++) {
        h->regs[I2C_CTRL] |= (1u << 5);   /* one manual SCL pulse */
        atp_hal_delay_us(10);
    }
    return (h->regs[I2C_STAT] & STAT_SDA_LOW) ? ATP_HAL_I2C_ERR_BUS_STUCK : ATP_STATUS_SUCCESS;
}
