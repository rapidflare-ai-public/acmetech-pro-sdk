/*
 * atp_hal_clk.c - clock sources and the core clock.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "atp_hal.h"
#include "atp_regs.h"

static uint32_t g_core_hz = 24000000u;

static uint32_t src_bit(atp_clk_src_e src)
{
    switch (src) {
    case ATP_CLK_SRC_LFRC_32K: return CLKCTRL_EN_LFRC;
    case ATP_CLK_SRC_HFRC_24M: return CLKCTRL_EN_HFRC24;
    case ATP_CLK_SRC_HFRC_96M: return CLKCTRL_EN_HFRC96;
#if defined(ATP_CHIP_PRO100)
    case ATP_CLK_SRC_PLL_192M: return CLKCTRL_EN_PLL;
#endif
    default: return 0;
    }
}

atp_status_t atp_hal_clk_enable(atp_clk_src_e src)
{
    uint32_t bit = src_bit(src);
    if (bit == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    CLKCTRL->EN |= bit;
    while ((CLKCTRL->READY & bit) == 0) { }
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_clk_disable(atp_clk_src_e src)
{
    uint32_t bit = src_bit(src);
    if (bit == 0 || bit == CLKCTRL_EN_LFRC) {
        return ATP_STATUS_INVALID_ARG;   /* LFRC can't be turned off */
    }
    CLKCTRL->EN &= ~bit;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_clk_core_set(atp_clk_src_e src)
{
    uint32_t bit = src_bit(src);
    if ((CLKCTRL->READY & bit) == 0) {
        return ATP_STATUS_INVALID_OPERATION;
    }
    switch (src) {
    case ATP_CLK_SRC_HFRC_24M: CLKCTRL->CORESEL = 1; g_core_hz = 24000000u;  break;
    case ATP_CLK_SRC_HFRC_96M: CLKCTRL->CORESEL = 2; g_core_hz = 96000000u;  break;
#if defined(ATP_CHIP_PRO100)
    case ATP_CLK_SRC_PLL_192M: CLKCTRL->CORESEL = 3; g_core_hz = 192000000u; break;
#endif
    default: return ATP_STATUS_INVALID_ARG;
    }
    return ATP_STATUS_SUCCESS;
}

uint32_t atp_hal_clk_core_hz(void)
{
    return g_core_hz;
}
