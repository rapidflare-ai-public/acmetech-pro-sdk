/*
 * atp_hal_i2c.h - I2C controller.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_HAL_I2C_H
#define ATP_HAL_I2C_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "atp_status.h"

/* Module-specific status codes: ATP_STATUS_BASE(ATP_MODULE_I2C) is 0x00030000. */
#define ATP_HAL_I2C_ERR_ARB_LOST        (ATP_STATUS_BASE(ATP_MODULE_I2C) | 0x0001u) /* 0x00030001 another controller won the bus */
#define ATP_HAL_I2C_ERR_ADDR_NAK        (ATP_STATUS_BASE(ATP_MODULE_I2C) | 0x0002u) /* 0x00030002 no device acknowledged the address */
#define ATP_HAL_I2C_ERR_DATA_NAK        (ATP_STATUS_BASE(ATP_MODULE_I2C) | 0x0003u) /* 0x00030003 device NAKed a data byte */
#define ATP_HAL_I2C_ERR_BUS_STUCK       (ATP_STATUS_BASE(ATP_MODULE_I2C) | 0x0004u) /* 0x00030004 SDA held low; see atp_hal_i2c_recover() */
#define ATP_HAL_I2C_ERR_STRETCH_TIMEOUT (ATP_STATUS_BASE(ATP_MODULE_I2C) | 0x0005u) /* 0x00030005 device held SCL low too long */

typedef enum {
    ATP_I2C_SPEED_100K,
    ATP_I2C_SPEED_400K,
    ATP_I2C_SPEED_1M,       /* needs the core at 96 MHz or faster */
} atp_i2c_speed_e;

typedef struct {
    atp_i2c_speed_e speed;
    uint32_t        stretch_timeout_ms;   /* 0 = ATP_CFG_BLOCKING_TIMEOUT_MS */
} atp_i2c_config_t;

typedef struct atp_i2c_handle *atp_i2c_handle_t;

atp_status_t atp_hal_i2c_init(uint32_t instance, const atp_i2c_config_t *cfg, atp_i2c_handle_t *handle);
atp_status_t atp_hal_i2c_deinit(atp_i2c_handle_t handle);

/* addr is the 7-bit address, not shifted. */
atp_status_t atp_hal_i2c_write(atp_i2c_handle_t h, uint8_t addr, const uint8_t *buf, size_t len, bool stop);
atp_status_t atp_hal_i2c_read(atp_i2c_handle_t h, uint8_t addr, uint8_t *buf, size_t len);
atp_status_t atp_hal_i2c_write_read(atp_i2c_handle_t h, uint8_t addr,
                                    const uint8_t *wbuf, size_t wlen, uint8_t *rbuf, size_t rlen);

/* Clock SCL up to 9 times to release a device holding SDA low. */
atp_status_t atp_hal_i2c_recover(atp_i2c_handle_t h);

#endif /* ATP_HAL_I2C_H */
