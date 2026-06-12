/*
 * acam37.c - driver for the AcmeTech Cameras Pro-37 (ACAM37) image sensor.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "acam37.h"

#define REG_CHIP_ID_H   0x0A
#define REG_CHIP_ID_L   0x0B
#define REG_RESET       0x12
#define REG_RES         0x20
#define REG_FORMAT      0x21
#define REG_STREAM      0x30

static atp_i2c_handle_t g_i2c;
static uint8_t          g_addr;
static uint32_t         g_pwdn;

static atp_status_t reg_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };
    return atp_hal_i2c_write(g_i2c, g_addr, buf, 2, true);
}

static atp_status_t reg_read(uint8_t reg, uint8_t *val)
{
    return atp_hal_i2c_write_read(g_i2c, g_addr, &reg, 1, val, 1);
}

atp_status_t acam37_init(const acam37_config_t *cfg)
{
    if (cfg == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    if (cfg->res > ACAM37_MAX_RES) {
        return ACAM37_ERR_RES_UNSUPPORTED;
    }

    g_addr = cfg->i2c_addr ? cfg->i2c_addr : ACAM37_I2C_ADDR_SA0_LOW;
    g_pwdn = cfg->pwdn_pin;

    /* PWDN is active high: drive it low to power the sensor up. */
    atp_gpio_cfg_t pwdn = { .fn = ATP_GPIO_FN_GPIO, .dir = ATP_GPIO_DIR_OUT };
    atp_hal_gpio_pinconfig(g_pwdn, &pwdn);
    atp_hal_gpio_write(g_pwdn, false);
    atp_hal_delay_ms(5);

    atp_i2c_config_t i2c = { .speed = ATP_I2C_SPEED_400K };
    atp_status_t st = atp_hal_i2c_init(cfg->i2c_instance, &i2c, &g_i2c);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }

    st = reg_write(REG_RESET, 0x80);
    if (st != ATP_STATUS_SUCCESS) {
        return st;   /* 0x00030002 here means nothing answered at g_addr: check SA0 and PWDN */
    }
    atp_hal_delay_ms(20);   /* 1.4.2: the sensor ignores writes for 20 ms after reset */

    uint8_t hi = 0, lo = 0;
    st = reg_read(REG_CHIP_ID_H, &hi);
    if (st == ATP_STATUS_SUCCESS) {
        st = reg_read(REG_CHIP_ID_L, &lo);
    }
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }
    if ((((uint16_t)hi << 8) | lo) != ACAM37_CHIP_ID) {
        return ACAM37_ERR_WRONG_CHIP_ID;
    }

    st = reg_write(REG_RES, (uint8_t)cfg->res);
    if (st == ATP_STATUS_SUCCESS) {
        st = reg_write(REG_FORMAT, (uint8_t)cfg->format);
    }
    return st;
}

atp_status_t acam37_capture(uint8_t *frame, uint32_t frame_len)
{
    (void)frame;
    (void)frame_len;
    atp_status_t st = reg_write(REG_STREAM, 1);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }
    /* Capture port DMA omitted in this demo. */
    uint32_t start = atp_hal_tick_ms();
    while (atp_hal_tick_ms() - start < 200u) {
        /* wait for VSYNC */
    }
    reg_write(REG_STREAM, 0);
    return ACAM37_ERR_FRAME_TIMEOUT;
}

atp_status_t acam37_power_down(void)
{
    reg_write(REG_STREAM, 0);
    atp_hal_gpio_write(g_pwdn, true);
    return atp_hal_i2c_deinit(g_i2c);
}
