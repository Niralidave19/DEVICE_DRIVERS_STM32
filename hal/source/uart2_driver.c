/*
 * uart2_driver.c
 *
 *  Created on: Jul 1, 2025
 *      Author: niral
 */

#include "uart_macros.h"
#include "stm32f446xx.h"
#include <stdint.h>
#include "gpio_types.h"
#include "gpio_proto.h"
#include "uart_types.h"
#include "uart_proto.h"
#include "nvic_proto.h"
#include "nvic_types.h"

uint8_t rx_data;
uint8_t rx_ready;
void UART_clock(){
	UART2_clock_enable();
}

/* UART_GetPort() -
 * Returns a pointer, pointing to the address of the USART2 peripheral
 */
volatile UART_Reg_def_t* UART_GetPort(){
	return (volatile UART_Reg_def_t*)USART2_BASE_ADDRESS;
}

/* USART_Init() -
 * Initializes USART2 Port, enabling PA2 for TX and PA3 for RX */
void USART_Init()
{
	gpio_config_t         gpio_pin_tx;
	gpio_config_t         gpio_pin_rx;
	UART_config_t         UART_config;

	gpio_pin_tx.port_id    = GPIO_ID_A;
	gpio_pin_tx.mode       = GPIO_MODE_ALTFUN;
	gpio_pin_tx.op_type    = GPIO_OP_PUSH_PULL;
	gpio_pin_tx.op_speed   = GPIO_LOW_SPEED;
	gpio_pin_tx.push_pull  = GPIO_PULL_UP;
	gpio_pin_tx.alt_func   = GPIO_AF7;
	gpio_pin_tx.enable     = GPIO_ENABLE;
	gpio_pin_tx.pin_number = 2;                     //PA2 for TX

	gpio_pin_rx.port_id    = GPIO_ID_A;
	gpio_pin_rx.mode       = GPIO_MODE_ALTFUN;
	gpio_pin_rx.op_type    = GPIO_OP_PUSH_PULL;
	gpio_pin_rx.op_speed   = GPIO_LOW_SPEED;
	gpio_pin_rx.push_pull  = GPIO_PULL_UP;
	gpio_pin_rx.alt_func   = GPIO_AF7;
	gpio_pin_rx.enable     = GPIO_ENABLE;
	gpio_pin_rx.pin_number = 3;                    //PA3 for RX

	UART_config.enable_clk_cntrl  = 0;
	UART_config.enable_flow_cntrl = 0;
	UART_config.baud = 9600;
	UART_config.pclk = 45000000;
	UART_config.mode = 1;

	GPIO_Clock(gpio_pin_tx.port_id,gpio_pin_tx.enable);
	GPIO_Clock(gpio_pin_rx.port_id,gpio_pin_rx.enable);
	UART_clock();

	GPIO_Init(gpio_pin_tx);
	GPIO_Init(gpio_pin_rx);

	/*Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();

	/* NVIC Configuration */
	NVIC_config_t nvic_config;
	nvic_config.IRQn = 38;
	nvic_config.preempt_bits = 4;
	nvic_config.priority = 5;

	/* Set the baud rate: No. of bits/sec
	 * PCLK : Clock driving the UART peripheral : basically the APB1 Bus clock
	 * Each bus clock is derived from system clock
	 * USART->usart_brr = (UART_config.pclk + (UART_config.baud / 2)) / UART_config.baud;
	 * Hardcoded value 0x682 - for baud rate 9600 and Default APB1 Bus clock */

	USART->usart_brr = 0x683;

	/* General init configuration required for any UART peripheral
	 * Enable UART peripheral, Transmitter enable, receiver enable,
	 * set word length, parity
	 */
	USART->usart_cr1 =  (1 << 13) |  // UE: USART Enable
	            	    (1 << 3)  |  // TE: Transmitter enable
						(1 << 2)  ;  // RE: Receiver enable

	/*If USART Needs to be configured as synchronous based on application
	 * Set the CLKEN bit to 1 in CR2
	 * If USART needs to be asynchronous, don't configure clock/set the bit to be 0 */
	 if(UART_config.enable_clk_cntrl){
		 USART->usart_cr2 |= (1 << 11);  // CLKEN - Enable SCLK Pin
		 USART->usart_cr2 &= ~(1 << 10); // CPOL = 0
		 USART->usart_cr2 &= ~(1 << 9);  // CPHA = 0
	 }

	 /*If USART needs flow control, we enable else disable
	  *If communicating with a strict device, with timings and buffers
	  * flow control is required. */
	 if(UART_config.enable_flow_cntrl){
		 USART->usart_cr3  |= (1 << 8);  // CTSE
		 USART->usart_cr3  |= (1 << 9);   // RTSE
	 }

	 /* UART in which mode of communication? UART using polling / interrupt/ DMA*/
	 if(UART_config.mode == 1){
	 		USART->usart_cr1 |= (1 << 7); //TXEIE
	 		USART->usart_cr1 |= (1 << 5); //RXNEIE
	 		NVIC_init(nvic_config);
	 }
}

/* Function   : uart_receive()
 * Description: Read what is being received in the UART port - 8 bits of data is read/9 bits
 * depending on the word length
 * Parameters : */
uint16_t uart_receive(){

	/* Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();

	/* Polling is used for reading the data
	 * In this method, we tell the processor to keep checking the RXNE bit (5th bit of SR reg)
	 * if the bit is set we read the data from the DR register
	 */

	while(!((USART->usart_sr) & (1<<5)));

	/* After receiving the data, check the word length and type-cast to return
	 * the data value */
	if(USART->usart_cr1 & (1<<12)){
		return (uint16_t)(USART->usart_dr & 0x1FF);
	}
	else{
		return (uint8_t)(USART->usart_dr & 0xFF);
	}
}

/* Function   : uart_receive()
 * Description: Write 1 Byte of data into the UART Port, when TXE = 1
 * Parameters : */
void uart_write(uint8_t ch) {
	/*Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();
  // Wait until TXE == 1
	while (!(USART->usart_sr & (1 << 7)));
	USART->usart_dr = ch;
}

/*Transmission data*/
uint8_t get_data(){
	return (uint8_t)0x8;
}

/************************Function: USART2_IRQHandler()***********************************
 ********* Description: ISR handler when an interrupt occurs on the USART peripheral*****
 **********Note: The name of the function must be same as that of the vector table*******/
void USART2_IRQHandler(){
	/*Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();
	/*Transmit data*/
	if(USART->usart_sr & (1 << 7)){
		USART->usart_dr = get_data();
	}

	/*Receive data and store into ch*/
	if(USART->usart_sr & (1 << 5)){
		/* After receiving the data, check the word length and type-cast to return
	 	 * the data value */
		if(USART->usart_cr1 & (1<<12)){
			rx_data = (uint16_t)(USART->usart_dr & 0x1FF);
		}
		else{
			rx_data = (uint8_t)(USART->usart_dr & 0xFF);
		}
		rx_ready = 1;
	}
}



