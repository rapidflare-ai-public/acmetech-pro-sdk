/*
 * atp_status.h - status codes shared by every AcmeTech Pro SDK module.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 *
 * A status is a 32-bit value. The upper 16 bits name the module that
 * returned it and the lower 16 bits are the code within that module:
 *
 *     0x00MM_CCCC    MM = module id, CCCC = code
 *
 * Codes below ATP_STATUS_MODULE_CODE_START (0x0100) are the generic codes
 * listed here and mean the same thing in every module. Module-specific codes
 * start at 0x0001 inside their module's range and are defined in that
 * module's header, so 0x00030002 and 0x00200002 are unrelated errors.
 */
#ifndef ATP_STATUS_H
#define ATP_STATUS_H

#include <stdint.h>

typedef uint32_t atp_status_t;

/* Module ids (bits 23:16). */
#define ATP_MODULE_GENERIC   0x00u
#define ATP_MODULE_GPIO      0x01u
#define ATP_MODULE_UART      0x02u
#define ATP_MODULE_I2C       0x03u
#define ATP_MODULE_SPI       0x04u
#define ATP_MODULE_TIMER     0x05u
#define ATP_MODULE_CLK       0x06u
#define ATP_MODULE_PWR       0x07u
#define ATP_MODULE_ACAM37    0x20u   /* drivers/camera_pro37 */
#define ATP_MODULE_ADSP84    0x21u   /* drivers/display_pro84 */

/* Base of a module's range: ATP_STATUS_BASE(ATP_MODULE_I2C) == 0x00030000. */
#define ATP_STATUS_BASE(module)      ((atp_status_t)(module) << 16)
#define ATP_STATUS_MODULE(status)    (((status) >> 16) & 0xFFu)
#define ATP_STATUS_CODE(status)      ((status) & 0xFFFFu)

/* Generic codes. A module returns these unshifted (module id 0). */
#define ATP_STATUS_SUCCESS            0x0000u
#define ATP_STATUS_FAIL               0x0001u
#define ATP_STATUS_INVALID_HANDLE     0x0002u
#define ATP_STATUS_IN_USE             0x0003u
#define ATP_STATUS_TIMEOUT            0x0004u
#define ATP_STATUS_OUT_OF_RANGE       0x0005u
#define ATP_STATUS_INVALID_ARG        0x0006u
#define ATP_STATUS_INVALID_OPERATION  0x0007u
#define ATP_STATUS_HW_ERR             0x0008u

#define ATP_STATUS_MODULE_CODE_START  0x0100u

#endif /* ATP_STATUS_H */
