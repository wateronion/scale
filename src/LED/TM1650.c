/*
 * tm1650.c
 *
 *  Created on: 2025年12月19日
 *      Author: admin
 */
#include "tm1650.h"
// 显示内容阴码：0-9
const uint8_t number[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

// A-F:0x77,0x7c,0x39,0x5e,0x79,0x71

//----------------------IIC设置部分------------------------
void MyI2C_W_SCL(uint8_t BitValue)
{
    // GPIO_WriteBit(IICGPIO, SCLPIN, (BitAction)BitValue);		//根据BitValue，设置SCL引脚的电平
    R_IOPORT_PinWrite(&g_ioport_ctrl, TM1650_SCL, BitValue);
    delay_us(10); // 延时10us，防止时序频率超过要求
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    // GPIO_WriteBit(IICGPIO, SDAPIN, (BitAction)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
    R_IOPORT_PinWrite(&g_ioport_ctrl, TM1650_SDA, BitValue);
    delay_us(10); // 延时10us，防止时序频率超过要求
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    R_IOPORT_PinRead(&g_ioport_ctrl, TM1650_SDA, &BitValue);
    // BitValue = GPIO_ReadInputDataBit(IICGPIO, SDAPIN); // 读取SDA电平
    delay_us(10);    // 延时10us，防止时序频率超过要求
    return BitValue; // 返回SDA电平
}

// void MyI2C_Init(void)
// {
//     /*开启时钟*/
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_IICGPIO, ENABLE); // 开启IICGPIO的时钟

//     /*GPIO初始化*/
//     GPIO_InitTypeDef GPIO_InitStructure;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//     GPIO_InitStructure.GPIO_Pin = SCLPIN | SDAPIN;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(IICGPIO, &GPIO_InitStructure); // 将PB10和PB11引脚初始化为开漏输出

//     /*设置默认电平*/
//     GPIO_SetBits(IICGPIO, SCLPIN | SDAPIN); // 设置PB10和PB11引脚初始化后默认为高电平（释放总线状态）
// }

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1); // 释放SDA，确保SDA为高电平
    MyI2C_W_SCL(1); // 释放SCL，确保SCL为高电平
    MyI2C_W_SDA(0); // 在SCL高电平期间，拉低SDA，产生起始信号
    MyI2C_W_SCL(0); // 起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0); // 拉低SDA，确保SDA为低电平
    MyI2C_W_SCL(1); // 释放SCL，使SCL呈现高电平
    MyI2C_W_SDA(1); // 在SCL高电平期间，释放SDA，产生终止信号
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++) // 循环8次，主机依次发送数据的每一位
    {
        MyI2C_W_SDA(Byte & (0x80 >> i)); // 使用掩码的方式取出Byte的指定一位数据并写入到SDA线
        MyI2C_W_SCL(1);                  // 释放SCL，从机在SCL高电平期间读取SDA
        MyI2C_W_SCL(0);                  // 拉低SCL，主机开始发送下一位数据
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00; // 定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
    MyI2C_W_SDA(1);         // 接收前，主机先确保释放SDA，避免干扰从机的数据发送
    for (i = 0; i < 8; i++) // 循环8次，主机依次接收数据的每一位
    {
        MyI2C_W_SCL(1); // 释放SCL，主机机在SCL高电平期间读取SDA
        if (MyI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        } // 读取SDA数据，并存储到Byte变量
          // 当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
        MyI2C_W_SCL(0); // 拉低SCL，从机在SCL低电平期间写入SDA
    }
    return Byte; // 返回接收到的一个字节数据
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit); // 主机把应答位数据放到SDA线
    MyI2C_W_SCL(1);      // 释放SCL，从机在SCL高电平期间，读取应答位
    MyI2C_W_SCL(0);      // 拉低SCL，开始下一个时序模块
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;         // 定义应答位变量
    MyI2C_W_SDA(1);         // 接收前，主机先确保释放SDA，避免干扰从机的数据发送
    MyI2C_W_SCL(1);         // 释放SCL，主机机在SCL高电平期间读取SDA
    AckBit = MyI2C_R_SDA(); // 将应答位存储到变量里
    MyI2C_W_SCL(0);         // 拉低SCL，开始下一个时序模块
    return AckBit;          // 返回定义应答位变量
}

//--------------------------------------------------

/*
功能：初始化数码管
参数：IIC： SCL-B10,SDA-B11,有需要跳转到MyI2C.c中修改
解释：IIC与设置亮度，清空显示
*/
void TM1650_init(void)
{
    // MyI2C_Init();
    TM1650_bright(TM1650_LEVEL5); // 初始化为5级亮度，打开显示
    TM1650_clear();               // 将显存内容清0
}

/*
功能：设置亮度并打开显示
参数：param : TM1650_LEVELx,x为1-8表亮度，TM1650_DSP_OFF为关闭led显示
解释：这个操作不影响显存中的数据
*/
void TM1650_bright(uint8_t param)
{
    MyI2C_Start();
    MyI2C_SendByte(0x48); // 通信格式：起始信号，模式命令（1字节），显示命令（1字节），结束信号
    MyI2C_ReceiveAck();

    MyI2C_SendByte(param);
    MyI2C_ReceiveAck(); // 显示命令
    MyI2C_Stop();
}

/*
功能：清空数据，屏幕熄灭
解释：正常只能显示0-9，10作特殊判断，0000 0000 全部熄灭。
*/
void TM1650_clear(void)
{
    uint8_t dig;
    for (dig = TM1650_DIG1; dig <= TM1650_DIG4; dig++)
    {
        TM1650_print(dig, 10); // 特殊数字，将显存数据刷为0
    }
}

/*
功能：往一个指定的数码管位写入指定的显示数字
参数1：DIG:  TM1650_DIGX，（X：1234）,对应1234位置
参数2：数字0~9
参数3：是否开启当前位小数点：ENABLE，DIAABLE
解释：通信格式：起始信号，地址码（1字节），数码管段数据（1字节），结束信号
*/
void TM1650_print(uint8_t dig, uint8_t seg_data)
{
    MyI2C_Start();
    MyI2C_SendByte(dig * 2 + 0x68); // 显存起始地址为0x68，dig直接用0x68，0x6A，0x6e,0x6c也行
    MyI2C_ReceiveAck();

    if (seg_data == 10) // clear 清空显示
        MyI2C_SendByte(0);
    else
        MyI2C_SendByte(number[seg_data]); // 数码管段数据
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

/*
功能：往一个指定的数码管位写入指定的显示数字，可设置小鼠点
参数1：DIG:  TM1650_DIGX，（X：1234）,对应1234位置
参数2：数字0~9
参数3：是否开启当前位小数点：ENABLE，DIAABLE
解释：通信格式：起始信号，地址码（1字节），数码管段数据（1字节），结束信号
*/
void TM1650_pointPrint(uint8_t dig, uint8_t seg_data, uint8_t point)
{
    MyI2C_Start();
    MyI2C_SendByte(dig * 2 + 0x68); // 显存起始地址为0x68，dig直接用0x68，0x6A，0x6e,0x6c也行
    MyI2C_ReceiveAck();

    if (seg_data == 10) // clear 清空显示
        MyI2C_SendByte(0);
    else if (seg_data == 11)
    { // 仅显示小数点
        MyI2C_SendByte(0x80);
    }
    else
    {                                 // 显示数字+小数点
        uint8_t t = number[seg_data]; // 附加小数点
        if (point)
            t = t | 0x80;
        MyI2C_SendByte(t); // 数码管段数据
    }
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

/*
功能：（保留地址写入功能）指定的数码管位写入指定的地址
参数1：DIG:  TM1650_DIGX，（X：1234）,对应1234位置
参数2：阴码地址
解释：通信格式：起始信号，地址码（1字节），数码管段数据（1字节），结束信号
*/
void TM1650_addPrint(uint8_t dig, uint8_t seg_data)
{
    MyI2C_Start();
    MyI2C_SendByte(dig * 2 + 0x68); // 显存起始地址为0x68，dig直接用0x68，0x6A，0x6e,0x6c也行
    MyI2C_ReceiveAck();

    MyI2C_SendByte(seg_data); // 直接发地址
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

/*
功能：数字位灯段测试
参数：DIG:TM1650_DIGX，（X：1234）
解释：灯段测试，观察二进制1~64分别对应哪一位灯光变化
*/
void TM1650_lambLineTest(uint8_t DIG)
{
    for (int i = 1; i <= 64; i *= 2)
    {
        TM1650_addPrint(DIG, i);
        Delay_ms(400);
    }
    TM1650_pointPrint(DIG, 11, POINT_ENABLE);
    Delay_ms(400);
    TM1650_clear();
}

/*
功能：显示一个数字
参数：num<=9999
解释：空位补0，右对其
*/
void TM1650_showNum(int num)
{
    int ge = num % 10;
    int sw = num > 10 ? num / 10 % 10 : 0;
    int bw = num > 100 ? num / 100 % 10 : 0;
    int qw = num > 1000 ? num / 1000 : 0;
    TM1650_print(TM1650_DIG4, ge);
    TM1650_print(TM1650_DIG3, sw);
    TM1650_print(TM1650_DIG2, bw);
    TM1650_print(TM1650_DIG1, qw);
}
