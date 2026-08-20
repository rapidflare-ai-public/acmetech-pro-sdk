/*
 * atp_hal_clkmgr.h - clock manager (since 2.0.0).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Replaces atp_hal_clk_* from 1.x. Clocks are now reference counted per
 * user: a clock stays on while any user has it requested, so a driver
 * releasing a clock no longer turns it off under another driver.
 *
 *     1.x                                    2.x
 *     atp_hal_clk_enable(src)                atp_hal_clkmgr_request(user, clk)
 *     atp_hal_clk_disable(src)               atp_hal_clkmgr_release(user, clk)
 *     atp_hal_clk_core_set(src)              atp_hal_clkmgr_core_set(clk)
 *     atp_hal_clk_core_hz()                  atp_hal_clkmgr_core_hz()
 */
#ifndef ATP_HAL_CLKMGR_H
#define ATP_HAL_CLKMGR_H

#include <stdint.h>
#include "atp_status.h"

#define ATP_HAL_CLKMGR_ERR_NOT_REQUESTED (ATP_STATUS_BASE(ATP_MODULE_CLK) | 0x0001u) /* 0x00060001 release without request */
#define ATP_HAL_CLKMGR_ERR_CORE_IN_USE   (ATP_STATUS_BASE(ATP_MODULE_CLK) | 0x0002u) /* 0x00060002 can't release the core's clock */

typedef enum {
    ATP_CLKMGR_CLK_LFRC_32K,
    ATP_CLKMGR_CLK_HFRC_24M,
    ATP_CLKMGR_CLK_HFRC_96M,
#if defined(ATP_CHIP_PRO100)
    ATP_CLKMGR_CLK_PLL_192M,
#endif
    ATP_CLKMGR_CLK_COUNT
} atp_clkmgr_clk_e;

typedef enum {
    ATP_CLKMGR_USER_CORE,
    ATP_CLKMGR_USER_APP,
    ATP_CLKMGR_USER_UART,
    ATP_CLKMGR_USER_I2C,
    ATP_CLKMGR_USER_SPI,
    ATP_CLKMGR_USER_CAMERA,
    ATP_CLKMGR_USER_DISPLAY,
    ATP_CLKMGR_USER_COUNT
} atp_clkmgr_user_e;

atp_status_t atp_hal_clkmgr_request(atp_clkmgr_user_e user, atp_clkmgr_clk_e clk);
atp_status_t atp_hal_clkmgr_release(atp_clkmgr_user_e user, atp_clkmgr_clk_e clk);

/* Run the core from clk. Requests clk for ATP_CLKMGR_USER_CORE. */
atp_status_t atp_hal_clkmgr_core_set(atp_clkmgr_clk_e clk);
uint32_t     atp_hal_clkmgr_core_hz(void);

#endif /* ATP_HAL_CLKMGR_H */
