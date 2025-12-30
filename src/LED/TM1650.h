/*
 * tm1650.h
 *
 *  Created on: 2025年12月19日
 *      Author: admin
 */

#ifndef LED_TM1650_H_
#define LED_TM1650_H_

#include "hal_data.h"

#ifndef delay_ms
#define delay_ms(x)   R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MILLISECONDS)
#endif
#ifndef delay_1ms
#define delay_1ms(x)  R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MILLISECONDS)
#endif
#ifndef delay_us
#define delay_us(x)   R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MICROSECONDS)
#endif
#ifndef delay_1us
#define delay_1us(x)  R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MICROSECONDS)
#endif

#ifndef u8
#define u8  uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif


//设置亮度参数
#define TM1650_LEVEL1       0x11   /*一级亮度，打开LED显示*/
#define TM1650_LEVEL2       0x21   /*二级亮度，打开LED显示*/
#define TM1650_LEVEL3       0x31   /*三级亮度，打开LED显示*/
#define TM1650_LEVEL4       0x41   /*四级亮度，打开LED显示*/
#define TM1650_LEVEL5       0x51   /*五级亮度，打开LED显示*/
#define TM1650_LEVEL6       0x61   /*六级亮度，打开LED显示*/
#define TM1650_LEVEL7       0x71   /*七级亮度，打开LED显示*/
#define TM1650_LEVEL8       0x01   /*八级亮度，打开LED显示*/
#define TM1650_DSP_OFF       0x00   /*关闭LED显示*/

//数码管位选
#define TM1650_DIG1     0
#define TM1650_DIG2     1
#define TM1650_DIG3     2
#define TM1650_DIG4     3

//是否开启小数点
#define POINT_ENABLE      1
#define POINT_DISABLE     0

//Tm1650的 SCL，SDA 分别接 B10，B11，有需求自己修改
 
void TM1650_init(void);
void TM1650_bright(uint8_t param); //设置亮度 TM1650_LEVEL1-8
void TM1650_showNum(int num);      //显示一个4位以内的数字，右对齐，前补0
void TM1650_clear(void);
void TM1650_print(uint8_t dig,uint8_t seg_data); //第几位，显示数字几
void TM1650_pointPrint(uint8_t dig,uint8_t seg_data,uint8_t point);//第几位，显示数字几，带小数点

void TM1650_addPrint(uint8_t dig,uint8_t seg_data);//大佬请看这里：直接写入地址
void TM1650_lambLineTest(uint8_t DIG);//测试灯段损坏

#endif /* LED_TM1650_H_ */
