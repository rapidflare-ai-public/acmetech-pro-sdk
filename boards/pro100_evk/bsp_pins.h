/*
 * bsp_pins.h - Pro-100 Evaluation Kit pin map.
 *
 * FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
 */
#ifndef BSP_PINS_H
#define BSP_PINS_H

/* Debug UART: UART0 on pins 14/15, routed to the USB bridge. */
#define BSP_DEBUG_UART        0
#define BSP_DEBUG_UART_TX     14
#define BSP_DEBUG_UART_RX     15

/* User LEDs, active high. LED0 moved from pin 54 to 52 in 1.4.1. */
#define BSP_LED0              52
#define BSP_LED1              53

/* User button, active low. Can wake from deep sleep. */
#define BSP_BUTTON0           34

/* Camera connector (AcmeTech Cameras Pro-37) on I2C1. SA0 is pulled HIGH on
 * this board (R41), so the camera answers at 0x3D, not the driver's
 * default 0x3C. */
#define BSP_CAM_I2C           1
#define BSP_CAM_SCL           20
#define BSP_CAM_SDA           21
#define BSP_CAM_PWDN          30
#define BSP_CAM_I2C_ADDR      0x3D

/* Display connector (AcmeTech Displays Pro-84) on SPI0. */
#define BSP_DISP_SPI          0
#define BSP_DISP_SCK          8
#define BSP_DISP_MOSI         9
#define BSP_DISP_CS           11
#define BSP_DISP_DC           12
#define BSP_DISP_RST          13
#define BSP_DISP_BACKLIGHT    16    /* PWM; the display driver drives it with TIMER2 */

#endif /* BSP_PINS_H */
