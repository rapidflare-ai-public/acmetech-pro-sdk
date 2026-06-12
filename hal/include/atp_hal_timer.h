/*
 * atp_hal_timer.h - general-purpose timers, PWM and the HAL tick.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * Pro-29 has TIMER0-TIMER7, Pro-100 has TIMER0-TIMER11. The timers stop in
 * deep sleep; to wake from deep sleep use the WKT in atp_hal_pwr.h.
 */
#ifndef ATP_HAL_TIMER_H
#define ATP_HAL_TIMER_H

#include <stdint.h>
#include "atp_status.h"

#if defined(ATP_CHIP_PRO29)
#define ATP_TIMER_COUNT 8
#else
#define ATP_TIMER_COUNT 12
#endif

#define ATP_HAL_TIMER_ERR_RESERVED (ATP_STATUS_BASE(ATP_MODULE_TIMER) | 0x0001u) /* used by the HAL itself */

typedef enum {
    ATP_TIMER_ONESHOT,
    ATP_TIMER_PERIODIC,
    ATP_TIMER_PWM,           /* output on a pin set to ATP_GPIO_FN_TIMER_OUT */
} atp_timer_mode_e;

typedef struct {
    atp_timer_mode_e mode;
    uint32_t         period_us;
    uint8_t          duty_pct;    /* PWM only, 0-100 */
    uint32_t         out_pin;     /* PWM only */
} atp_timer_config_t;

atp_status_t atp_hal_timer_config(uint32_t timer, const atp_timer_config_t *cfg);
atp_status_t atp_hal_timer_start(uint32_t timer);
atp_status_t atp_hal_timer_stop(uint32_t timer);
atp_status_t atp_hal_timer_set_duty(uint32_t timer, uint8_t duty_pct);

/* HAL tick. Needs ATP_CFG_HAL_TICK_TIMER (atp_hal_cfg.h). */
uint32_t atp_hal_tick_ms(void);
void     atp_hal_delay_ms(uint32_t ms);
void     atp_hal_delay_us(uint32_t us);

/* Called by bsp_init(). Starts the tick timer. */
atp_status_t atp_hal_timer_tick_init(void);

#endif /* ATP_HAL_TIMER_H */
