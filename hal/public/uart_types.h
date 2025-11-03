/*
 * uart_types.h
 *
 *  Created on: Jul 1, 2025
 *      Author: niral
 */

#ifndef PUBLIC_UART_TYPES_H_
#define PUBLIC_UART_TYPES_H_

#include <stdint.h>

typedef struct {
	volatile uint32_t usart_sr;
	volatile uint32_t usart_dr;
	volatile uint32_t usart_brr;
	volatile uint32_t usart_cr1;
	volatile uint32_t usart_cr2;
	volatile uint32_t usart_cr3;
	volatile uint32_t usart_gtpr;
}UART_Reg_def_t;

/*To choose which type of mode UART works in:
  1. Polling   2. Interrupt driven  3. DMA */
typedef enum {
	UART_POLLING_MODE,
	UART_INTERRUPT_MODE,
	UART_DMA_MODE
}UART_mode_t;

typedef struct {
	uint8_t     enable_clk_cntrl;
	uint8_t     enable_flow_cntrl;
	uint32_t    baud;
	uint32_t    pclk;
	UART_mode_t mode;
}UART_config_t;

#endif /* PUBLIC_UART_TYPES_H_ */
