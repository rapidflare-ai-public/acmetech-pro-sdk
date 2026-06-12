/*
 * atp_regs.h - register blocks used by the HAL. Internal; include atp_hal.h.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. The addresses
 * and bit positions below are invented. See NOTICE.md.
 */
#ifndef ATP_REGS_H
#define ATP_REGS_H

#include <stdint.h>

typedef struct {
    volatile uint32_t EN;
    volatile uint32_t READY;
    volatile uint32_t CORESEL;
    volatile uint32_t CALIB;
} atp_clkctrl_t;

#define CLKCTRL ((atp_clkctrl_t *)0x40001000u)
#define CLKCTRL_EN_LFRC    (1u << 0)
#define CLKCTRL_EN_HFRC24  (1u << 1)
#define CLKCTRL_EN_HFRC96  (1u << 2)
#define CLKCTRL_EN_PLL     (1u << 3)

typedef struct {
    volatile uint32_t SLEEPCTRL;
    volatile uint32_t WKTCTRL;
    volatile uint32_t WKTCMP;
    volatile uint32_t WAKESRC;
    volatile uint32_t TRIM;
} atp_pwrctrl_t;

#define PWRCTRL ((atp_pwrctrl_t *)0x40002000u)

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t COUNT;
    volatile uint32_t CMP0;
    volatile uint32_t CMP1;
    volatile uint32_t INTEN;
    volatile uint32_t INTSTAT;
} atp_timer_regs_t;

#define TIMER_BASE    0x40010000u
#define TIMER_STRIDE  0x100u
#define TIMERn(n)     ((atp_timer_regs_t *)(TIMER_BASE + (n) * TIMER_STRIDE))

#endif /* ATP_REGS_H */
