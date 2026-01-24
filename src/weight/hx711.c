#include "hx711.h"
#include "stdio.h"
#include "flash/flash.h"

// 重量0值
uint32_t HX711_Weight_Init;
Moving_Average_Filter_St HX711_Filter;
// 电子秤按键
SCALE_Key Scale_Key;

float GAP_VALUE = 111.08;

static uint32_t Moving_Average_Filter(Moving_Average_Filter_St *filter, uint32_t new_adc_value)
{
    if (new_adc_value == 0)
        return 0;
    // 1. 如果缓冲区还未填满，执行填充操作
    if (!filter->is_full)
    {
        filter->sum += new_adc_value;
        filter->buffer[filter->index] = new_adc_value;
        filter->index++;

        // 检查缓冲区是否刚刚被填满
        if (filter->index == FILTER_BUFFER_SIZE)
        {
            filter->is_full = 1;
            filter->index = 0;
        }

        // 在填满前，返回当前已采集数据的平均值
        return filter->sum / (filter->index);
    }

    // 2. 缓冲区满后
    else
    {
        filter->sum -= filter->buffer[filter->index];
        filter->sum += new_adc_value;
        filter->buffer[filter->index] = new_adc_value;
        filter->index = (filter->index + 1) % FILTER_BUFFER_SIZE;

        return filter->sum / FILTER_BUFFER_SIZE;
    }
}

void HX711_Init(void)
{
    SDA_OUT(); // 设置SDA引脚为输出模式

    SDA(1);
    SCL(1);

    delay_ms(100); // 等待传感器稳定
}

/**
 * @brief 获取ADC值
 *
 * @return uint32_t
 */
static uint32_t HX711_Read(void) // 增益128
{
    unsigned long count;
    unsigned char i;

    SDA_OUT();
    delay_us(5);

    SDA(1);
    delay_us(1);
    SCL(0);
    count = 0;

    SDA_IN();
    delay_us(5);

    while (GET_SDA())
        ;
    for (i = 0; i < 24; i++)
    {
        SCL(1);
        count = count << 1;
        delay_us(1);
        SCL(0);
        if (GET_SDA())
            count++;
        delay_us(1);
    }
    SCL(1);
    count = count ^ 0x800000; // 第25个脉冲下降沿来时，转换数据
    delay_us(1);
    SCL(0);

    return (count);
}

void HX711_Get_InitValue(void)
{
    HX711_Weight_Init = HX711_Read();
}

/**
 * @brief 获取重量
 *
 * @return float
 */
float HX711_Get_Weight(void)
{
    uint32_t HX711_Read_Buffer = 0;
    float Return_Buffer = 0;

    // HX711_Read_Buffer = HX711_Read();
    HX711_Read_Buffer = Moving_Average_Filter(&HX711_Filter, HX711_Read());
    // printf("""HX711 Read Buffer = %d\r\n", HX711_Read_Buffer);

    if (HX711_Read_Buffer > HX711_Weight_Init)
    {
        HX711_Read_Buffer -= HX711_Weight_Init; // 获取实物的采样数值。

        Return_Buffer = (float)HX711_Read_Buffer / GAP_VALUE;
    }

    return Return_Buffer;
}

void Get_Key(void)
{
    static uint8_t current_count = 0;

    Scale_Key.is_Tare.last_state = Scale_Key.is_Tare.current_state;
    Scale_Key.is_Calibration.last_state = Scale_Key.is_Calibration.current_state;
    current_count++;
    R_IOPORT_PinRead(&g_ioport_ctrl, KEY1, &Scale_Key.is_Tare.current_state);
    R_IOPORT_PinRead(&g_ioport_ctrl, KEY2, &Scale_Key.is_Calibration.current_state);

    if (Scale_Key.is_Tare.current_state == KEY_PRESSED && Scale_Key.is_Tare.last_state == KEY_RELEASED)
    {
        Scale_Key.is_Tare.press_time = current_count;
        Scale_Key.is_Tare.press_detected = 1; // 检测到按键按下
    }
    else if (Scale_Key.is_Tare.current_state == KEY_RELEASED && Scale_Key.is_Tare.last_state == KEY_PRESSED)
    {
        Scale_Key.is_Tare.release_time = current_count;
        if (Scale_Key.is_Tare.press_detected)
        {
            // key.release_detected = 1; //检测到按键释放
            Scale_Key.is_Tare.press_detected = 0;
            Scale_Key.is_Tare.is_pressed = 1;
        }
    }

    if (Scale_Key.is_Calibration.current_state == KEY_PRESSED && Scale_Key.is_Calibration.last_state == KEY_RELEASED)
    {
        Scale_Key.is_Calibration.press_time = current_count;
        Scale_Key.is_Calibration.press_detected = 1; // 检测到按键按下
    }
    else if (Scale_Key.is_Calibration.current_state == KEY_RELEASED && Scale_Key.is_Calibration.last_state == KEY_PRESSED)
    {
        Scale_Key.is_Calibration.release_time = current_count;
        if (Scale_Key.is_Calibration.press_detected)
        {
            // key.release_detected = 1; //检测到按键释放
            Scale_Key.is_Calibration.press_detected = 0;
            Scale_Key.is_Calibration.is_pressed = 1;
        }
    }
}

void HX711_Get_CalibrationValue(void)
{
    uint64_t calibration_Parameter = 0;

    // 零点
    if(Scale_Key.is_Calibration.is_pressed)
    {
        calibration_Parameter = 0;

        // 采集10次重量，抛弃
        for(int i = 0; i < 10; i++)
        {
            Moving_Average_Filter(&HX711_Filter, HX711_Read());
            delay_ms(10);
            RUN_LED_TOGGLE();
        }

        delay_ms(500);

        for(int i = 0; i < 80; i++)
        {
            calibration_Parameter += Moving_Average_Filter(&HX711_Filter, HX711_Read());
            delay_ms(10);
            RUN_LED_TOGGLE();
        }

        RUN_LED_ON();

        calibration_Parameter /= 80;
        HX711_Weight_Init = calibration_Parameter;
        // printf("HX711_Weight_Init = %d\r\n", HX711_Weight_Init);

        // 等待
        delay_ms(10000);

        calibration_Parameter = 0;
        // 校准GAP_VALUE
        for(int i = 0; i < 10; i++)
        {
            Moving_Average_Filter(&HX711_Filter, HX711_Read());
            delay_ms(10);
            RUN_LED_TOGGLE();
        }

        delay_ms(500);

        for(int i = 0; i < 80; i++)
        {
            calibration_Parameter += Moving_Average_Filter(&HX711_Filter, HX711_Read());
            delay_ms(10);
            RUN_LED_TOGGLE();
        }

        RUN_LED_ON();

        calibration_Parameter /= 80;
        // if(HX711_Weight_Init > calibration_Parameter)
        // {
        //     RUN_LED_OFF();
        //     return;
        // }

        // if(abs(calibration_Parameter - GAP_VALUE) > 100)
        // {
        //     RUN_LED_OFF();
        //     return;
        // }

        GAP_VALUE = (float)(calibration_Parameter - HX711_Weight_Init) / 1000.0f;
        FLASH_Write_Float(&GAP_VALUE,CALIBRATION_VALUE_ADDR);
        // printf("GAP_VALUE = %f\r\n", GAP_VALUE);
    }
}
