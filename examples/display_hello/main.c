/*
 * display_hello - draw on an AcmeTech Displays Pro-84 and fade the backlight.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "bsp.h"
#include "adsp84.h"

int main(void)
{
    bsp_init();

    atp_gpio_cfg_t sck  = { .fn = ATP_GPIO_FN_SPI0_SCK };
    atp_gpio_cfg_t mosi = { .fn = ATP_GPIO_FN_SPI0_MOSI };
    atp_hal_gpio_pinconfig(BSP_DISP_SCK, &sck);
    atp_hal_gpio_pinconfig(BSP_DISP_MOSI, &mosi);

    adsp84_config_t cfg = {
        .spi_instance  = BSP_DISP_SPI,
        .spi_clock     = ATP_SPI_CLK_24MHZ,
        .cs_pin        = BSP_DISP_CS,
        .dc_pin        = BSP_DISP_DC,
        .rst_pin       = BSP_DISP_RST,
        .backlight_pin = BSP_DISP_BACKLIGHT,
    };
    atp_status_t st = adsp84_open(&cfg);
    if (st != ATP_STATUS_SUCCESS) {
        bsp_printf("adsp84_open failed: 0x%08lx\n", (unsigned long)st);
        for (;;) { }
    }

    adsp84_fill(0x001F);   /* blue */
    adsp84_text(20, 110, "Hello from AcmeTech", 0xFFFF, 0x001F);

    for (;;) {
        for (uint8_t pct = 10; pct <= 100; pct += 10) {
            adsp84_backlight(pct);
            atp_hal_delay_ms(100);
        }
    }
}
