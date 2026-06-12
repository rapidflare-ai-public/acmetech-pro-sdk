/*
 * i2c_scan - list the devices that answer on the camera connector's I2C bus.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "bsp.h"

int main(void)
{
    bsp_init();

    atp_gpio_cfg_t scl = { .fn = (BSP_CAM_I2C == 0) ? ATP_GPIO_FN_I2C0_SCL : ATP_GPIO_FN_I2C1_SCL };
    atp_gpio_cfg_t sda = { .fn = (BSP_CAM_I2C == 0) ? ATP_GPIO_FN_I2C0_SDA : ATP_GPIO_FN_I2C1_SDA };
    atp_hal_gpio_pinconfig(BSP_CAM_SCL, &scl);
    atp_hal_gpio_pinconfig(BSP_CAM_SDA, &sda);

    /* The camera stays in power-down unless PWDN is low. */
    atp_gpio_cfg_t pwdn = { .fn = ATP_GPIO_FN_GPIO, .dir = ATP_GPIO_DIR_OUT };
    atp_hal_gpio_pinconfig(BSP_CAM_PWDN, &pwdn);
    atp_hal_gpio_write(BSP_CAM_PWDN, false);
    atp_hal_delay_ms(5);

    atp_i2c_config_t cfg = { .speed = ATP_I2C_SPEED_100K };
    atp_i2c_handle_t i2c;
    atp_hal_i2c_init(BSP_CAM_I2C, &cfg, &i2c);

    bsp_printf("i2c_scan on I2C%d\n", BSP_CAM_I2C);
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        atp_status_t st = atp_hal_i2c_write(i2c, addr, 0, 0, true);
        if (st == ATP_STATUS_SUCCESS) {
            bsp_printf("  found 0x%02x\n", addr);
        } else if (st == ATP_HAL_I2C_ERR_BUS_STUCK) {
            bsp_printf("  bus stuck, recovering\n");
            atp_hal_i2c_recover(i2c);
        }
        /* ATP_HAL_I2C_ERR_ADDR_NAK: nothing at this address */
    }
    bsp_printf("done\n");
    for (;;) { }
}
