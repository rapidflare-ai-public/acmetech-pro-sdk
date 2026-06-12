/*
 * acam37.h - driver for the AcmeTech Cameras Pro-37 (ACAM37) image sensor.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * The camera is configured over I2C and streams frames over the parallel
 * capture port. Its I2C address depends on the SA0 pin: 0x3C with SA0 low,
 * 0x3D with SA0 high. Use the board's BSP_CAM_I2C_ADDR.
 */
#ifndef ACAM37_H
#define ACAM37_H

#include <stdint.h>
#include "atp_hal.h"

#define ACAM37_I2C_ADDR_SA0_LOW   0x3C
#define ACAM37_I2C_ADDR_SA0_HIGH  0x3D

/* Driver status codes: ATP_STATUS_BASE(ATP_MODULE_ACAM37) is 0x00200000.
 * I2C failures are passed through unchanged (0x0003xxxx). */
#define ACAM37_ERR_WRONG_CHIP_ID   (ATP_STATUS_BASE(ATP_MODULE_ACAM37) | 0x0001u) /* 0x00200001 something answered, not an ACAM37 */
#define ACAM37_ERR_RES_UNSUPPORTED (ATP_STATUS_BASE(ATP_MODULE_ACAM37) | 0x0002u) /* 0x00200002 resolution too big for this chip */
#define ACAM37_ERR_FRAME_TIMEOUT   (ATP_STATUS_BASE(ATP_MODULE_ACAM37) | 0x0003u) /* 0x00200003 no VSYNC within 200 ms */

#define ACAM37_CHIP_ID             0x3725u

typedef enum {
    ACAM37_RES_QQVGA,   /* 160x120 */
    ACAM37_RES_QVGA,    /* 320x240 */
    ACAM37_RES_VGA,     /* 640x480 */
    ACAM37_RES_HD,      /* 1280x720, Pro-100 only */
} acam37_res_e;

/* The largest frame the chip has RAM for, in RGB565. */
#if defined(ATP_CHIP_PRO29)
#define ACAM37_MAX_RES ACAM37_RES_QVGA   /* 150 KB; VGA would need 600 KB of the Pro-29's 256 KB */
#else
#define ACAM37_MAX_RES ACAM37_RES_HD
#endif

typedef enum { ACAM37_FMT_RGB565, ACAM37_FMT_YUV422, ACAM37_FMT_GRAY8 } acam37_format_e;

typedef struct {
    uint32_t        i2c_instance;
    uint8_t         i2c_addr;
    uint32_t        pwdn_pin;
    acam37_res_e    res;
    acam37_format_e format;
} acam37_config_t;

/* Power up, reset, check the chip id and apply cfg. */
atp_status_t acam37_init(const acam37_config_t *cfg);
atp_status_t acam37_capture(uint8_t *frame, uint32_t frame_len);
atp_status_t acam37_power_down(void);

#endif /* ACAM37_H */
