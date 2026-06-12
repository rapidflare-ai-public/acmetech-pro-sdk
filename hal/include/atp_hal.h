/*
 * atp_hal.h - umbrella header for the AcmeTech Pro HAL.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_HAL_H
#define ATP_HAL_H

#if !defined(ATP_CHIP_PRO29) && !defined(ATP_CHIP_PRO100)
#error "Define ATP_CHIP_PRO29 or ATP_CHIP_PRO100"
#endif

#include "atp_status.h"
#include "atp_hal_cfg.h"
#include "atp_hal_clk.h"
#include "atp_hal_pwr.h"
#include "atp_hal_gpio.h"
#include "atp_hal_uart.h"
#include "atp_hal_i2c.h"
#include "atp_hal_spi.h"
#include "atp_hal_timer.h"

#endif /* ATP_HAL_H */
