/*
 * uart_macros.h
 *
 *  Created on: Jul 4, 2025
 *      Author: niral
 */

#ifndef PUBLIC_UART_MACROS_H_
#define PUBLIC_UART_MACROS_H_

#define UART2_clock_enable() RCC->rcc_apb1enr = RCC->rcc_apb1enr | (1<<17)

#endif /* PUBLIC_UART_MACROS_H_ */
