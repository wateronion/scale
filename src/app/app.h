/*
 * app.h
 *
 *  Created on: 2025年12月18日
 *      Author: admin
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "hal_data.h"

#define RUN_LED_ON() R_IOPORT_PinWrite(&g_ioport_ctrl, RUN_LED, BSP_IO_LEVEL_HIGH)
#define RUN_LED_OFF() R_IOPORT_PinWrite(&g_ioport_ctrl, RUN_LED, BSP_IO_LEVEL_LOW)
#define RUN_LED_TOGGLE() R_PORT4->PODR ^= 1<<(RUN_LED & 0xFF);

void app_main(void);

#endif /* APP_APP_H_ */
