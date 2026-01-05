#ifndef WEIGHT_HX711_H_
#define WEIGHT_HX711_H_

#include "hal_data.h"
#include <stdio.h>

#ifndef delay_ms
#define delay_ms(x) R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MILLISECONDS)
#endif
#ifndef delay_1ms
#define delay_1ms(x) R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MILLISECONDS)
#endif
#ifndef delay_us
#define delay_us(x) R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MICROSECONDS)
#endif
#ifndef delay_1us
#define delay_1us(x) R_BSP_SoftwareDelay(x, BSP_DELAY_UNITS_MICROSECONDS)
#endif

#define Module_SCL_PIN HX_SCL // SCL
#define Module_SDA_PIN HX_SDA // SDA

extern float GAP_VALUE;

// 滤波器
#define FILTER_BUFFER_SIZE 10

// 按键检测
#define KEY_PRESSED 0
#define KEY_RELEASED 1

// 校准值
#define CALIBRATION_VALUE_ADDR FLASH_HP_DF_BLOCK_0
#define CALIBRATION_VALUE_SIZE 4

// SDA输入模式
#define SDA_IN()                                                                                                         \
    {                                                                                                                    \
        fsp_err_t err = R_IOPORT_PinCfg(&g_ioport_ctrl, Module_SDA_PIN,                                                  \
                                        (uint32_t)IOPORT_CFG_PORT_DIRECTION_INPUT | (uint32_t)IOPORT_CFG_PULLUP_ENABLE); \
        if (err != FSP_SUCCESS)                                                                                          \
        {                                                                                                                \
            printf("R_IOPORT_PinCfg Failed!!\r\n");                                                                      \
        }                                                                                                                \
    }

// SDA输出模式
#define SDA_OUT()                                                                                                                                                                                   \
    {                                                                                                                                                                                               \
        fsp_err_t err = R_IOPORT_PinCfg(&g_ioport_ctrl, Module_SDA_PIN,                                                                                                                             \
                                        ((uint32_t)IOPORT_CFG_DRIVE_HIGH | (uint32_t)IOPORT_CFG_NMOS_ENABLE | (uint32_t)IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t)IOPORT_CFG_PORT_OUTPUT_HIGH)); \
        if (err != FSP_SUCCESS)                                                                                                                                                                     \
        {                                                                                                                                                                                           \
            printf("R_IOPORT_PinCfg Failed!!\r\n");                                                                                                                                                 \
        }                                                                                                                                                                                           \
    }

// SCL引脚和SDA引脚的输出
#define SCL(BIT) R_IOPORT_PinWrite(&g_ioport_ctrl, Module_SCL_PIN, BIT)
#define SDA(BIT) R_IOPORT_PinWrite(&g_ioport_ctrl, Module_SDA_PIN, BIT)

#define RUN_LED_TOGGLE()    R_PORT4->PODR ^= 1 << (BSP_IO_PORT_04_PIN_02 & 0xFF)
#define RUN_LED_ON()        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_02, 1)
#define RUN_LED_OFF()       R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_02, 0)

// 获取SDA引脚的电平状态
static inline bsp_io_level_t GET_SDA(void)
{
    bsp_io_level_t p_pin_value;
    fsp_err_t err = R_IOPORT_PinRead(&g_ioport_ctrl, Module_SDA_PIN, &p_pin_value);
    if (err != FSP_SUCCESS)
    {
        printf("GPIO Input Read Failed!!\r\n");
    }
    return p_pin_value;
}

typedef struct
{
    uint32_t buffer[FILTER_BUFFER_SIZE];
    uint32_t index;
    uint32_t sum;
    uint8_t is_full;
} Moving_Average_Filter_St;

extern Moving_Average_Filter_St HX711_Filter;

typedef struct
{
    bsp_io_level_t current_state; // 当前状态
    bsp_io_level_t last_state;    // 上一次状态
    uint8_t press_time;           // 按键按下时间
    uint8_t release_time;         // 按键释放时间
    uint8_t press_detected;       // 按键按下标志
    uint8_t is_pressed;           // 按键是否按下
} Key_State;

typedef struct
{
    Key_State is_Tare;        // 清零
    Key_State is_Calibration; // 校准
} SCALE_Key;

extern SCALE_Key Scale_Key;

void HX711_Init(void);
void HX711_Get_InitValue(void);
float HX711_Get_Weight(void);
void Get_Key(void);
void HX711_Get_CalibrationValue(void);

#endif /* BSP_CODE_BSP_HX711_H_ */
