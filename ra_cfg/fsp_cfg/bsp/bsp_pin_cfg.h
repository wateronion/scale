/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define TM1650_SCL (BSP_IO_PORT_02_PIN_06)
#define TM1650_SDA (BSP_IO_PORT_02_PIN_07)
#define HX_SCL (BSP_IO_PORT_02_PIN_12)
#define HX_SDA (BSP_IO_PORT_02_PIN_13)
#define KEY1 (BSP_IO_PORT_03_PIN_01)
#define KEY2 (BSP_IO_PORT_03_PIN_02)
#define RUN_LED (BSP_IO_PORT_04_PIN_02)
#define OLED_SCL (BSP_IO_PORT_04_PIN_07)
#define OLED_SDA (BSP_IO_PORT_04_PIN_08)

extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA6E2BB3CNE.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
