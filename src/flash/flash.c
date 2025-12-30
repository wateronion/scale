#include "flash.h"

uint8_t write_data[32] = {0};
uint8_t read_data[64] = {0};

// FLASH擦除完成标志
volatile bool flash_erase_complete_flag = false;
// FLASH写入完成标志
volatile bool flash_write_complete_flag = false;
// FLASH事件标志
volatile bool flash_event_not_blank = false;
// 
volatile bool flash_event_blank = false;
// FLASH事件错误标志
volatile bool flash_event_error = false;
volatile flash_event_t flash_event_error_type;

// 初始化Flash
void FLASH_Init(void)
{
    fsp_err_t err = R_FLASH_HP_Open(&g_flash0_ctrl, &g_flash0_cfg);
    assert(FSP_SUCCESS == err);
}

/**
 * @brief 擦除Flash
 * 
 * @param addr 要擦除的地址
 * @param block_num  要擦除块的数量
 */
void FLASH_Erase(uint32_t addr, uint32_t block_num)
{
    fsp_err_t err = R_FLASH_HP_Erase(&g_flash0_ctrl, addr, block_num);
    assert(FSP_SUCCESS == err);

    while(!flash_erase_complete_flag);
    flash_erase_complete_flag = false;
}

/**
 * @brief 写入Flash
 * 
 * @param data_addr  要写入的数据地址
 * @param flash_addr  要写入的Flash地址
 * @param size   要写入的数据字节数
 */
void FLASH_Write(uint32_t data_addr, uint32_t flash_addr, uint32_t size)
{
    fsp_err_t err = R_FLASH_HP_Write(&g_flash0_ctrl, data_addr, flash_addr, size);
    assert(FSP_SUCCESS == err);

    while(!flash_write_complete_flag);
    flash_write_complete_flag = false;
}

/**
 * @brief 读取Flash
 * 
 * @param data_addr 读取到的输入写入的地址
 * @param flash_addr 要读取的Flash地址
 * @param size 读取的字节数
 */
void FLASH_Read(void *data_addr, uint32_t flash_addr, uint32_t size)
{
    memcpy(data_addr, (uint8_t *)flash_addr, size);
}

void FLASH_Test(void)
{
    // 要写入的数据
    for(uint8_t i = 0; i < 32; i++)
    {
        write_data[i] = (uint8_t)('A' + (i % 26));
    }
    
    // 初始化Flash
    FLASH_Init();
    
    // 擦除Flash
    FLASH_Erase(FLASH_HP_DF_BLOCK_0, 1);

    /* write flash */
    FLASH_Write((uint32_t)write_data, FLASH_HP_DF_BLOCK_0, 32);

    /* read flash */
    // memcpy(read_data, (uint8_t *)FLASH_HP_DF_BLOCK_0, 32);
    FLASH_Read(read_data, FLASH_HP_DF_BLOCK_0, 32);
    for(uint8_t i = 0; i < 32; i++)
    {
        printf("%c ", read_data[i]);
    }
    printf("\r\n");
}

/* DATA FLASH 操作完成中断回调 */
void flash_callback(flash_callback_args_t *p_args)
{
    flash_event_t flash_event = p_args->event;

    if (FLASH_EVENT_ERASE_COMPLETE == flash_event)
    {
        flash_erase_complete_flag = true;
    }
    else if (FLASH_EVENT_WRITE_COMPLETE == flash_event)
    {
        flash_write_complete_flag = true;
    }
    else if (FLASH_EVENT_BLANK == flash_event)
    {
        flash_event_blank = true;
    }
    else if (FLASH_EVENT_NOT_BLANK == flash_event)
    {
        flash_event_not_blank = true;
    }
    else
    {
        flash_event_error = true;
        flash_event_error_type = flash_event;
    }
}
