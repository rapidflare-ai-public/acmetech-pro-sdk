/*
 * atp_hal_pwr.c - sleep modes and the wake-up timer (WKT).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "atp_hal.h"
#include "atp_regs.h"

#define WKTCTRL_EN        (1u << 0)
#define WKTCTRL_PERIODIC  (1u << 1)
#define WKTCTRL_DIV1024   (1u << 4)
#define SLEEPCTRL_DEEP    (1u << 0)
#define WAKESRC_WKT       (1u << 0)
#define WAKESRC_GPIO      (1u << 1)

atp_status_t atp_hal_pwr_wkt_config(const atp_pwr_wkt_config_t *cfg)
{
    if (cfg == 0 || cfg->ticks == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    uint32_t ctrl = WKTCTRL_EN;
    if (cfg->div == ATP_PWR_WKT_DIV_1024) {
        ctrl |= WKTCTRL_DIV1024;
    }
    if (cfg->periodic) {
        ctrl |= WKTCTRL_PERIODIC;
    }
    PWRCTRL->WKTCTRL = 0;
    PWRCTRL->WKTCMP  = cfg->ticks;
    PWRCTRL->WKTCTRL = ctrl;
    PWRCTRL->WAKESRC |= WAKESRC_WKT;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_pwr_wkt_stop(void)
{
    PWRCTRL->WKTCTRL = 0;
    PWRCTRL->WAKESRC &= ~WAKESRC_WKT;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_pwr_sleep(atp_pwr_sleep_e mode)
{
    if (mode == ATP_PWR_SLEEP_DEEP) {
        if ((PWRCTRL->WAKESRC & (WAKESRC_WKT | WAKESRC_GPIO)) == 0) {
            return ATP_HAL_PWR_ERR_WAKE_SOURCE;
        }
        PWRCTRL->SLEEPCTRL = SLEEPCTRL_DEEP;
    } else {
        PWRCTRL->SLEEPCTRL = 0;
    }
    __asm volatile ("wfi");
    return ATP_STATUS_SUCCESS;
}

#define INFO_CHIPREV  (*(volatile uint32_t *)0x50000010u)
#define INFO_LDOTRIM  (*(volatile uint32_t *)0x50000040u)

uint32_t atp_hal_pwr_chip_rev(void)
{
    return INFO_CHIPREV & 0xFu;
}

atp_status_t atp_hal_pwr_trim_apply(void)
{
#if defined(ATP_CHIP_PRO100)
    if (atp_hal_pwr_chip_rev() == 0xBu) {
        /* PRO100-E7: TRIM resets to 0x10 on deep sleep exit instead of
         * keeping the factory value. */
        PWRCTRL->TRIM = INFO_LDOTRIM & 0x3Fu;
    }
#endif
    return ATP_STATUS_SUCCESS;
}
