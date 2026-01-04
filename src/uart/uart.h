/*
 * uart.h
 *
 *  Created on: 2025年12月18日
 *      Author: admin
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "hal_data.h"
#include "stdio.h"

#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>  // 添加这个头文件以获得struct stat定义

#include <stdint.h>

void UART0_Init(void);
void uart0_callback(uart_callback_args_t *p_args);

void UART1_Init(void);
void uart1_callback(uart_callback_args_t *p_args);

#endif /* UART_UART_H_ */
