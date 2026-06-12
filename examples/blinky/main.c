/*
 * blinky - toggle LED0 twice a second.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#include "bsp.h"

int main(void)
{
    bsp_init();
    bsp_printf("blinky\n");
    for (;;) {
        bsp_led_toggle(BSP_LED0);
        atp_hal_delay_ms(250);
    }
}
