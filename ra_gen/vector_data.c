/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [1] = sci_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [2] = sci_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [3] = sci_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [4] = sci_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [5] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [6] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [7] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [8] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [9] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [10] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP0), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP1), /* SCI0 RXI (Receive data full) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP2), /* SCI0 TXI (Transmit data empty) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP3), /* SCI0 TEI (Transmit end) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP4), /* SCI0 ERI (Receive error) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP5), /* SCI9 RXI (Receive data full) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP6), /* SCI9 TXI (Transmit data empty) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP7), /* SCI9 TEI (Transmit end) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP0), /* SCI9 ERI (Receive error) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP1), /* FCU FRDYI (Flash ready interrupt) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,GROUP2), /* FCU FIFERR (Flash access error interrupt) */
        };
        #endif
        #endif
