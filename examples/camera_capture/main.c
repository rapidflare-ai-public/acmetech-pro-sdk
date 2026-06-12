/*
 * camera_capture - initialize an AcmeTech Cameras Pro-37 and grab frames.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "bsp.h"
#include "acam37.h"

#if defined(ATP_CHIP_PRO29)
#define RES     ACAM37_RES_QVGA
#define FRAME_W 320
#define FRAME_H 240
#else
#define RES     ACAM37_RES_VGA
#define FRAME_W 640
#define FRAME_H 480
#endif

static uint8_t g_frame[FRAME_W * FRAME_H * 2];

int main(void)
{
    bsp_init();

    atp_gpio_cfg_t scl = { .fn = (BSP_CAM_I2C == 0) ? ATP_GPIO_FN_I2C0_SCL : ATP_GPIO_FN_I2C1_SCL };
    atp_gpio_cfg_t sda = { .fn = (BSP_CAM_I2C == 0) ? ATP_GPIO_FN_I2C0_SDA : ATP_GPIO_FN_I2C1_SDA };
    atp_hal_gpio_pinconfig(BSP_CAM_SCL, &scl);
    atp_hal_gpio_pinconfig(BSP_CAM_SDA, &sda);

    acam37_config_t cfg = {
        .i2c_instance = BSP_CAM_I2C,
        .i2c_addr     = BSP_CAM_I2C_ADDR,
        .pwdn_pin     = BSP_CAM_PWDN,
        .res          = RES,
        .format       = ACAM37_FMT_RGB565,
    };
    atp_status_t st = acam37_init(&cfg);
    if (st != ATP_STATUS_SUCCESS) {
        bsp_printf("acam37_init failed: 0x%08lx (module 0x%02lx, code 0x%04lx)\n",
                   (unsigned long)st, (unsigned long)ATP_STATUS_MODULE(st),
                   (unsigned long)ATP_STATUS_CODE(st));
        for (;;) { }
    }

    for (;;) {
        st = acam37_capture(g_frame, sizeof(g_frame));
        bsp_printf("frame: 0x%08lx\n", (unsigned long)st);
        atp_hal_delay_ms(1000);
    }
}
