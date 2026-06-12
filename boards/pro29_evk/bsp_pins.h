/*
 * bsp_pins.h - Pro-29 Evaluation Kit pin map.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef BSP_PINS_H
#define BSP_PINS_H

/* Debug UART: UART1 on pins 14/15, routed to the USB bridge. */
#define BSP_DEBUG_UART        0
#define BSP_DEBUG_UART_TX     6
#define BSP_DEBUG_UART_RX     7

/* User LEDs, active high. Both pins can also take ATP_GPIO_FN_TIMER_OUT. */
#define BSP_LED0              16
#define BSP_LED1              17

/* User button, active low, with an external pull-up. Can wake from deep sleep. */
#define BSP_BUTTON0           11

/* Camera connector (AcmeTech Cameras Pro-37) on I2C0. SA0 is tied to GND on
 * this board, so the camera answers at 0x3C. */
#define BSP_CAM_I2C           0
#define BSP_CAM_SCL           4
#define BSP_CAM_SDA           5
#define BSP_CAM_PWDN          22
#define BSP_CAM_I2C_ADDR      0x3C

/* Display connector (AcmeTech Displays Pro-84) on SPI0. */
#define BSP_DISP_SPI          0
#define BSP_DISP_SCK          8
#define BSP_DISP_MOSI         9
#define BSP_DISP_CS           23
#define BSP_DISP_DC           24
#define BSP_DISP_RST          25
#define BSP_DISP_BACKLIGHT    2     /* PWM; the display driver drives it with TIMER2 */

#endif /* BSP_PINS_H */
