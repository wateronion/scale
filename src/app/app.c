/*
 * app.c
 *
 *  Created on: 2025年12月18日
 *      Author: admin
 */

#include "app.h"
#include "timer/gpt_timer.h"
#include "uart/uart.h"
#include "oled/oled.h"
#include "oled/bmp.h"
#include "flash/flash.h"
#include "LED/TM1650.h"
#include "weight/hx711.h"

void app_main(void)
{
    UART0_Init();
    UART1_Init();
    // GPT_Timer_Init();
    printf("Hello World!\r\n");

    FLASH_Init();

    {
        uint8_t temp[4] = {0};
        FLASH_Read(&temp,CALIBRATION_VALUE_ADDR,4);
        if(temp[0] == 0xff && temp[1] == 0xff && temp[2] == 0xff && temp[3] == 0xff)
        {
            printf("Calibration Value is not set!\r\n");
        }
        else
        {
            FLASH_Read_Float(&GAP_VALUE,CALIBRATION_VALUE_ADDR);
        }
    }

    TM1650_init();
    HX711_Init();

    HX711_Get_InitValue();

    OLED_Init();
    OLED_ColorTurn(0);   // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示
    OLED_ShowString(0, 24, "WEIGHT:", 16, 1);
    OLED_ShowString(96, 24, "g", 16, 1);
    OLED_Refresh();
    while (1)
    {
        // OLED_ShowPicture(0,0,128,64,BMP2,1);
        if (Scale_Key.is_Calibration.is_pressed)
        {
            HX711_Get_CalibrationValue();
            Scale_Key.is_Calibration.is_pressed = 0;
        }
        else
        {
            Get_Key();

            float weight = HX711_Get_Weight();
            printf("\r\nHX711 Get Weight = [%.2fg]\r\n", weight);
            // R_SCI_UART_Write(&g_uart1_cfg, (uint8_t *)&weight, 4);
            TM1650_showNum((int)weight);
            OLED_ShowNum(56, 24, (uint32_t)weight, 5, 16, 1);
            OLED_Refresh();

            if (Scale_Key.is_Tare.is_pressed)
            {
                HX711_Get_InitValue();
                Scale_Key.is_Tare.is_pressed = 0;
            }

            // delay_1ms(20);
        }
    }
}
