/*
 * atp_hal_clk.h - clock sources and the core clock.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_HAL_CLK_H
#define ATP_HAL_CLK_H

#include <stdint.h>
#include "atp_status.h"

typedef enum {
    ATP_CLK_SRC_LFRC_32K,    /* low-frequency RC, always on, runs in deep sleep */
    ATP_CLK_SRC_HFRC_24M,
    ATP_CLK_SRC_HFRC_96M,
#if defined(ATP_CHIP_PRO100)
    ATP_CLK_SRC_PLL_192M,    /* Pro-100 only */
#endif
} atp_clk_src_e;

/* Turn a clock source on or off. Sources are not reference counted:
 * one atp_hal_clk_disable() turns the source off for every user. */
atp_status_t atp_hal_clk_enable(atp_clk_src_e src);
atp_status_t atp_hal_clk_disable(atp_clk_src_e src);

/* Run the core from src. The source must already be enabled. */
atp_status_t atp_hal_clk_core_set(atp_clk_src_e src);
uint32_t     atp_hal_clk_core_hz(void);

#endif /* ATP_HAL_CLK_H */
