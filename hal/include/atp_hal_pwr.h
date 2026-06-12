/*
 * atp_hal_pwr.h - sleep modes and the wake-up timer (WKT).
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * In deep sleep only the LFRC and the WKT keep running, so the WKT and GPIO
 * edges are the only wake sources. The general-purpose timers (atp_hal_timer)
 * stop in deep sleep and can't wake the chip.
 */
#ifndef ATP_HAL_PWR_H
#define ATP_HAL_PWR_H

#include <stdint.h>
#include <stdbool.h>
#include "atp_status.h"

/* Module-specific status codes. */
#define ATP_HAL_PWR_ERR_WAKE_SOURCE  (ATP_STATUS_BASE(ATP_MODULE_PWR) | 0x0001u) /* no wake source armed */

typedef enum {
    ATP_PWR_SLEEP_NORMAL,    /* core clock gated, peripherals run */
    ATP_PWR_SLEEP_DEEP,      /* everything off except LFRC, WKT, GPIO wake */
} atp_pwr_sleep_e;

/* The WKT counts the LFRC divided down. Its compare register is 16 bits. */
typedef enum {
    ATP_PWR_WKT_DIV_32,      /* 1024 Hz: up to 65535 ticks, about 64 s */
    ATP_PWR_WKT_DIV_1024,    /* 32 Hz:   up to 65535 ticks, about 34 min */
} atp_pwr_wkt_div_e;

typedef struct {
    atp_pwr_wkt_div_e div;
    uint16_t          ticks;     /* wake after this many WKT ticks */
    bool              periodic;  /* reload and wake again every period */
} atp_pwr_wkt_config_t;

atp_status_t atp_hal_pwr_wkt_config(const atp_pwr_wkt_config_t *cfg);
atp_status_t atp_hal_pwr_wkt_stop(void);

/* Enter a sleep mode. Returns after the next wake-up. Deep sleep with no
 * wake source armed returns ATP_HAL_PWR_ERR_WAKE_SOURCE without sleeping. */
atp_status_t atp_hal_pwr_sleep(atp_pwr_sleep_e mode);

#endif /* ATP_HAL_PWR_H */
