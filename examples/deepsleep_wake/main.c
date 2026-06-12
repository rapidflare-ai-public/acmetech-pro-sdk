/*
 * deepsleep_wake - deep sleep, wake every 10 seconds on the WKT or when
 * BUTTON0 is pressed, blink, and sleep again.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "bsp.h"

#define WAKE_SECONDS 10u

int main(void)
{
    bsp_init();
    bsp_printf("deepsleep_wake: every %u s or on BUTTON0\n", WAKE_SECONDS);

    /* 1024 Hz WKT: 10 s is 10240 ticks, under the 16-bit limit. For periods
     * over 64 s use ATP_PWR_WKT_DIV_1024 (32 Hz). */
    atp_pwr_wkt_config_t wkt = {
        .div      = ATP_PWR_WKT_DIV_32,
        .ticks    = (uint16_t)(WAKE_SECONDS * 1024u),
        .periodic = true,
    };
    atp_hal_pwr_wkt_config(&wkt);

    atp_gpio_cfg_t button = {
        .fn = ATP_GPIO_FN_GPIO,
        .dir = ATP_GPIO_DIR_IN,
        .wake_on_edge = true,
    };
    atp_hal_gpio_pinconfig(BSP_BUTTON0, &button);

    for (;;) {
        bsp_prepare_sleep();
        atp_hal_pwr_sleep(ATP_PWR_SLEEP_DEEP);
        bsp_resume_from_sleep();

        bsp_led_set(BSP_LED0, true);
        atp_hal_delay_ms(50);
        bsp_led_set(BSP_LED0, false);
        bsp_printf("woke at %lu ms\n", (unsigned long)atp_hal_tick_ms());
    }
}
