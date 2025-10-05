/*
 * uart2_driver.c
 *
 *  Created on: Jul 1, 2025
 *      Author: niral
 */

#include"uart_macros.h"
#include"stm32f446xx.h"
#include <stdint.h>
#include "gpio_types.h"
#include "gpio_proto.h"
#include "uart_types.h"
#include "uart_proto.h"

void UART_clock(){
	UART2_clock_enable();
}

/****************************Function Name: UART_GetPort() *************************
 ****Description: Returns a pointer, pointing to the address of the USART2 peripheral
 ***********************************************************************************/
volatile UART_Reg_def_t* UART_GetPort(){
	return (volatile UART_Reg_def_t*)USART2_BASE_ADDRESS;
}

/****************************Function Name: USART2_Init() *************************
 ****Description: 1. Initialises two GPIO pins to alternate functionality to act as 
                     UART RX and TX pins : PA2 for TX and PA3 for RX
				  2. Initialises the UART peripheral for receiving and sending data 
				     via USART2
 ***********************************************************************************/
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
	gpio_pin_tx.pin_number = 2; 

	gpio_pin_rx.port_id    = GPIO_ID_A;
	gpio_pin_rx.mode       = GPIO_MODE_ALTFUN;
	gpio_pin_rx.op_type    = GPIO_OP_PUSH_PULL;
	gpio_pin_rx.op_speed   = GPIO_LOW_SPEED;
	gpio_pin_rx.push_pull  = GPIO_PULL_UP;
	gpio_pin_rx.alt_func   = GPIO_AF7;
	gpio_pin_rx.enable     = GPIO_ENABLE;
	gpio_pin_rx.pin_number = 3;

	UART_config.enable_clk_cntrl  = 0;
	UART_config.enable_flow_cntrl = 0;
	UART_config.baud = 9600;
	UART_config.pclk = 45000000;

	/*Activate the GPIO and UART clock */
	GPIO_Clock(gpio_pin_tx.port_id,gpio_pin_tx.enable);
	GPIO_Clock(gpio_pin_rx.port_id,gpio_pin_rx.enable);
	UART_clock();

	/* GPIO pin initialisation */
	GPIO_Init(gpio_pin_tx);
	GPIO_Init(gpio_pin_rx);

	/* Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();

	USART->usart_brr = 0x683;

	/* General init configuration required for any UART peripheral
	 * Enable UART peripheral, Transmitter enable, receiver enable,
	 * set word length, parity */
	USART->usart_cr1 =  (1 << 13) |  // UE: USART Enable
	            	    (1 << 3)  |  // TE: Transmitter enable
						(1 << 2)  ;  // RE: Receiver enable

	/* If USART Needs to be configured as synchronous
	 * Send the CLKEN bit to 1 in CR2
	 * If USART needs to be asynchronous, don't configure clock/set the bit to be 0 
	 * USART->usart_cr2 = 0; */
	 if(UART_config.enable_clk_cntrl){
		 USART->usart_cr2 |= (1 << 11);  // CLKEN - Enable SCLK Pin
		 USART->usart_cr2 &= ~(1 << 10); // CPOL = 0
		 USART->usart_cr2 &= ~(1 << 9);  // CPHA = 0
	 }

	 /*If USART needs flow control, we enable else disable
	  *If communicating with a strict device, with timings and buffers
	  * flow control is required. 
	  * USART->usart_cr3 = 0;*/
	 if(UART_config.enable_flow_cntrl){
		 USART->usart_cr3  |= (1 << 8);  // CTSE
		 USART->usart_cr3  |= (1 << 9);  // RTSE
	 }
}

/**************************Function   : USART2_Receive()********************************
 *********Description: Read what is being received in the UART port*********************
 *8 bits of data is read/9 bits *depending on the word length **************************
 **************************Return type : uint16_t *************************************/

uint16_t USART2_Receive(){

	/* Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();

	/* Polling is used for reading the data
	 * In this method, we tell the processor to keep checking the RXNE bit (5th bit of SR reg)
	   If the bit is set we read the data from the DR register */
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

/************************Function: uart_receive()***********************************
 ********* Description: Write 1 Byte of data into the UART Port, when TXE = 1*******
 **********Parameters : An unsigned char value is of input ************************/

void USART2_Write(uint8_t ch) {
	/*Get the peripheral address of USART */
	volatile UART_Reg_def_t* USART = UART_GetPort();
    /* Wait until TXE == 1*/
	while (!(USART->usart_sr & (1 << 7)));
	USART->usart_dr = ch;
}



