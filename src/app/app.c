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
//    float get_weight = 0;
    UART0_Init();
    GPT_Timer_Init();
    
    printf("Hello World!\r\n");

    FLASH_Init();
    // FLASH_Test();

    TM1650_init();
    HX711_Init();

    HX711_Get_InitValue();

    OLED_Init();
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    TM1650_showNum(1234);
    while(1)
    {
        OLED_ShowPicture(0,0,128,64,BMP2,1);
        OLED_Refresh();
        delay_ms(200);

        printf("\r\nHX711 Get Weight = [%.2fg]\r\n", HX711_Get_Weight());
        delay_1ms(300);
    }
}
