/*
 * flash.h
 *
 *  Created on: 2025年12月18日
 *      Author: admin
 */
#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_

#include "hal_data.h"
#include "stdio.h"
#include "string.h"


#define FLASH_HP_EXAMPLE_WRITE_SIZE    32

/* Data Flash */
#define FLASH_HP_DF_BLOCK_SIZE      (64)        /* Block Size 64B */

#define FLASH_HP_DF_BLOCK_0         0x08000000U /* 大小为 64 B:  0x08000000U - 0x0800003F */
#define FLASH_HP_DF_BLOCK_1         0x08000040U /* 大小为 64 B:  0x08000040U - 0x0800007F */
#define FLASH_HP_DF_BLOCK_2         0x08000080U /* 大小为 64 B:  0x08000080U - 0x080000BF */
#define FLASH_HP_DF_BLOCK_3         0x080000C0U /* 大小为 64 B:  0x080000C0U - 0x080000FF */
#define FLASH_HP_DF_BLOCK_4         0x08000100U /* 大小为 64 B:  0x08000100U - 0x0800013F */
#define FLASH_HP_DF_BLOCK_5         0x08000140U /* 大小为 64 B:  0x08000140U - 0x0800017F */

void FLASH_Init(void);
void FLASH_Erase(uint32_t addr, uint32_t block_num);
void FLASH_Write(uint32_t data_addr, uint32_t flash_addr, uint32_t size);
void FLASH_Read(void *data_addr, uint32_t flash_addr, uint32_t size);
void FLASH_Test(void);
void FLASH_HP_DataFlash_Operation(void);

void FLASH_Write_Float(float *data_addr, uint32_t flash_addr);
void FLASH_Read_Float(float *data_addr, uint32_t flash_addr);

#endif