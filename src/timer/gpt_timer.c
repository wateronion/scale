/*
 * gpt_timer.c
 *
 *  Created on: 2025年12月18日
 *      Author: admin
 */
#include "gpt_timer.h"

void GPT_Timer_Init(void)
{
    R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);

    R_GPT_Start(&g_timer0_ctrl);
}

void gpt0_callback(timer_callback_args_t *p_args)
{
    if (p_args->event == TIMER_EVENT_CYCLE_END)
    {
        R_PORT4->PODR ^= 1<<(BSP_IO_PORT_04_PIN_02 & 0xFF);
    }
}

