/*
 * atp_hal_spi.h - SPI controller.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_HAL_SPI_H
#define ATP_HAL_SPI_H

#include <stdint.h>
#include <stddef.h>
#include "atp_status.h"

typedef enum {
    ATP_SPI_CLK_1MHZ,
    ATP_SPI_CLK_4MHZ,
    ATP_SPI_CLK_12MHZ,
    ATP_SPI_CLK_24MHZ,       /* since 1.4.0; core must run at 96 MHz or faster */
} atp_spi_clk_e;

typedef enum { ATP_SPI_MODE0, ATP_SPI_MODE1, ATP_SPI_MODE2, ATP_SPI_MODE3 } atp_spi_mode_e;

typedef struct {
    atp_spi_clk_e  clock;
    atp_spi_mode_e mode;
    uint32_t       cs_pin;   /* driven by the HAL as a GPIO */
} atp_spi_config_t;

typedef struct atp_spi_handle *atp_spi_handle_t;

atp_status_t atp_hal_spi_init(uint32_t instance, const atp_spi_config_t *cfg, atp_spi_handle_t *handle);
atp_status_t atp_hal_spi_deinit(atp_spi_handle_t handle);
atp_status_t atp_hal_spi_transfer(atp_spi_handle_t h, const uint8_t *tx, uint8_t *rx, size_t len);

#endif /* ATP_HAL_SPI_H */
