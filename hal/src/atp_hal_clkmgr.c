/*
 * atp_hal_clkmgr.c - clock manager (since 2.0.0).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include <stdbool.h>
#include "atp_hal.h"
#include "atp_regs.h"

static uint32_t g_users[ATP_CLKMGR_CLK_COUNT];   /* bit per user */
static uint32_t g_core_hz = 24000000u;
static atp_clkmgr_clk_e g_core_clk = ATP_CLKMGR_CLK_HFRC_24M;

static const uint32_t k_en_bit[ATP_CLKMGR_CLK_COUNT] = {
    CLKCTRL_EN_LFRC,
    CLKCTRL_EN_HFRC24,
    CLKCTRL_EN_HFRC96,
#if defined(ATP_CHIP_PRO100)
    CLKCTRL_EN_PLL,
#endif
};

#if defined(ATP_CHIP_PRO29) && ATP_CFG_CLKMGR_AUTOCAL
/*
 * Pro-29 only: the HFRC_96M drifts with temperature by up to 1.5%, enough to
 * break 921600-baud UART. While HFRC_96M is on, the clock manager trims it
 * against the LFRC once a second, using TIMER6 as the reference counter.
 * TIMER6 is therefore reserved (see atp_hal_timer.c). Build with
 * ATP_CFG_CLKMGR_AUTOCAL=0 to free TIMER6 and skip calibration.
 */
#define AUTOCAL_TIMER 6
static void autocal_start(void)
{
    atp_timer_regs_t *t = TIMERn(AUTOCAL_TIMER);
    t->CTRL  = 0;
    t->CMP0  = 32768u;          /* one LFRC second */
    t->INTEN = 1u;
    t->CTRL  = (1u << 1) | (1u << 0) | (1u << 8);  /* periodic, enabled, LFRC-clocked */
}

void atp_hal_clkmgr_autocal_isr(void)
{
    atp_timer_regs_t *t = TIMERn(AUTOCAL_TIMER);
    t->INTSTAT = 1u;
    uint32_t counted = CLKCTRL->CALIB;    /* HFRC_96M cycles in one LFRC second */
    int32_t error_ppm = (int32_t)((int64_t)(counted - 96000000u) * 1000000 / 96000000);
    CLKCTRL->CALIB = (uint32_t)(-error_ppm / 100);   /* trim step is 100 ppm */
}
#endif

atp_status_t atp_hal_clkmgr_request(atp_clkmgr_user_e user, atp_clkmgr_clk_e clk)
{
    if (user >= ATP_CLKMGR_USER_COUNT || clk >= ATP_CLKMGR_CLK_COUNT) {
        return ATP_STATUS_INVALID_ARG;
    }
    bool was_off = (g_users[clk] == 0);
    g_users[clk] |= (1u << user);
    if (was_off) {
        CLKCTRL->EN |= k_en_bit[clk];
        while ((CLKCTRL->READY & k_en_bit[clk]) == 0) { }
#if defined(ATP_CHIP_PRO29) && ATP_CFG_CLKMGR_AUTOCAL
        if (clk == ATP_CLKMGR_CLK_HFRC_96M) {
            autocal_start();
        }
#endif
    }
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_clkmgr_release(atp_clkmgr_user_e user, atp_clkmgr_clk_e clk)
{
    if (user >= ATP_CLKMGR_USER_COUNT || clk >= ATP_CLKMGR_CLK_COUNT) {
        return ATP_STATUS_INVALID_ARG;
    }
    if ((g_users[clk] & (1u << user)) == 0) {
        return ATP_HAL_CLKMGR_ERR_NOT_REQUESTED;
    }
    if (user == ATP_CLKMGR_USER_CORE && clk == g_core_clk) {
        return ATP_HAL_CLKMGR_ERR_CORE_IN_USE;
    }
    g_users[clk] &= ~(1u << user);
    if (g_users[clk] == 0 && clk != ATP_CLKMGR_CLK_LFRC_32K) {
        CLKCTRL->EN &= ~k_en_bit[clk];
    }
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_clkmgr_core_set(atp_clkmgr_clk_e clk)
{
    static const uint32_t hz[] = { 32768u, 24000000u, 96000000u,
#if defined(ATP_CHIP_PRO100)
        192000000u,
#endif
    };
    if (clk == ATP_CLKMGR_CLK_LFRC_32K || clk >= ATP_CLKMGR_CLK_COUNT) {
        return ATP_STATUS_INVALID_ARG;
    }
    atp_status_t st = atp_hal_clkmgr_request(ATP_CLKMGR_USER_CORE, clk);
    if (st != ATP_STATUS_SUCCESS) {
        return st;
    }
    CLKCTRL->CORESEL = (uint32_t)clk;
    atp_clkmgr_clk_e old = g_core_clk;
    g_core_clk = clk;
    g_core_hz = hz[clk];
    if (old != clk) {
        atp_hal_clkmgr_release(ATP_CLKMGR_USER_CORE, old);
    }
    return ATP_STATUS_SUCCESS;
}

uint32_t atp_hal_clkmgr_core_hz(void)
{
    return g_core_hz;
}
