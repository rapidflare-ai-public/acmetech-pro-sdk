/*
 * atp_hal_cfg.h - build-time HAL options. Override any of these on the
 * compiler command line (-DATP_HAL_VALIDATE=1).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef ATP_HAL_CFG_H
#define ATP_HAL_CFG_H

/* Check arguments in every HAL call and return ATP_STATUS_INVALID_ARG or
 * ATP_STATUS_OUT_OF_RANGE. Off by default to save code space: with it off,
 * out-of-range values are clamped or ignored, not reported. */
#ifndef ATP_HAL_VALIDATE
#define ATP_HAL_VALIDATE 0
#endif

/* The HAL millisecond tick (atp_hal_delay_ms, timeouts) runs on the LAST
 * timer of the chip: TIMER7 on Pro-29, TIMER11 on Pro-100. Set to 0 to free
 * that timer; blocking HAL calls then busy-wait on cycle counts instead. */
#ifndef ATP_CFG_HAL_TICK_TIMER
#define ATP_CFG_HAL_TICK_TIMER 1
#endif

/* Default timeout for blocking transfers, in milliseconds. */
#ifndef ATP_CFG_BLOCKING_TIMEOUT_MS
#define ATP_CFG_BLOCKING_TIMEOUT_MS 100
#endif

#endif /* ATP_HAL_CFG_H */
