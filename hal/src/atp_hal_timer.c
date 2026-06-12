/*
 * atp_hal_timer.c - general-purpose timers, PWM and the HAL tick.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include <stdbool.h>
#include "atp_hal.h"
#include "atp_regs.h"

/* The tick runs on the last timer of the chip. */
#define TICK_TIMER (ATP_TIMER_COUNT - 1)

#define TCTRL_EN        (1u << 0)
#define TCTRL_PERIODIC  (1u << 1)
#define TCTRL_PWM       (1u << 2)

static volatile uint32_t g_tick_ms;

static bool timer_reserved(uint32_t timer)
{
#if ATP_CFG_HAL_TICK_TIMER
    if (timer == TICK_TIMER) {
        return true;
    }
#endif
    return false;
}

atp_status_t atp_hal_timer_config(uint32_t timer, const atp_timer_config_t *cfg)
{
    if (timer >= ATP_TIMER_COUNT || cfg == 0 || cfg->period_us == 0) {
        return ATP_STATUS_INVALID_ARG;
    }
    if (timer_reserved(timer)) {
        return ATP_HAL_TIMER_ERR_RESERVED;
    }
    /* Timers count at 1 MHz from the HFRC_24M prescaler. */
    atp_timer_regs_t *t = TIMERn(timer);
    t->CTRL = 0;
    t->CMP0 = cfg->period_us;
    uint32_t ctrl = 0;
    if (cfg->mode == ATP_TIMER_PERIODIC) {
        ctrl |= TCTRL_PERIODIC;
    } else if (cfg->mode == ATP_TIMER_PWM) {
        ctrl |= TCTRL_PERIODIC | TCTRL_PWM;
        t->CMP1 = (cfg->period_us * cfg->duty_pct) / 100u;
        atp_gpio_cfg_t pin = { .fn = ATP_GPIO_FN_TIMER_OUT };
        atp_status_t st = atp_hal_gpio_pinconfig(cfg->out_pin, &pin);
        if (st != ATP_STATUS_SUCCESS) {
            return st;
        }
    }
    t->CTRL = ctrl;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_timer_start(uint32_t timer)
{
    if (timer >= ATP_TIMER_COUNT) {
        return ATP_STATUS_INVALID_ARG;
    }
    TIMERn(timer)->CTRL |= TCTRL_EN;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_timer_stop(uint32_t timer)
{
    if (timer >= ATP_TIMER_COUNT) {
        return ATP_STATUS_INVALID_ARG;
    }
    TIMERn(timer)->CTRL &= ~TCTRL_EN;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_timer_set_duty(uint32_t timer, uint8_t duty_pct)
{
    if (timer >= ATP_TIMER_COUNT || duty_pct > 100u) {
        return ATP_STATUS_INVALID_ARG;
    }
    atp_timer_regs_t *t = TIMERn(timer);
    t->CMP1 = (t->CMP0 * duty_pct) / 100u;
    return ATP_STATUS_SUCCESS;
}

atp_status_t atp_hal_timer_tick_init(void)
{
#if ATP_CFG_HAL_TICK_TIMER
    atp_timer_regs_t *t = TIMERn(TICK_TIMER);
    t->CTRL  = 0;
    t->CMP0  = 1000u;            /* 1 ms at 1 MHz */
    t->INTEN = 1u;
    t->CTRL  = TCTRL_PERIODIC | TCTRL_EN;
#endif
    return ATP_STATUS_SUCCESS;
}

void atp_hal_tick_isr(void)
{
    TIMERn(TICK_TIMER)->INTSTAT = 1u;
    g_tick_ms++;
}

uint32_t atp_hal_tick_ms(void)
{
    return g_tick_ms;
}

void atp_hal_delay_ms(uint32_t ms)
{
    uint32_t start = g_tick_ms;
    while (g_tick_ms - start < ms) { }
}

void atp_hal_delay_us(uint32_t us)
{
    /* About 4 cycles per loop. */
    uint32_t loops = (atp_hal_clk_core_hz() / 4000000u) * us;
    while (loops--) { __asm volatile ("nop"); }
}
