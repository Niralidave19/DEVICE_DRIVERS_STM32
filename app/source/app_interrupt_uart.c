
/*******************************************************************************************
 * File name: app_uart_get_data.c
 * Description: The client requests STM32, to send 4 bytes of data stored at a given address
 * Protocol used is described in the readme.text document
 *******************************************************************************************/

#include <stdint.h>
#include "uart_proto.h"
#include "app_uart_proto.h"
extern rx_ready;
extern rx_data;

void app_uart_init(){
	USART_Init();
}

void app_uart_interrupt_recv_update(){
	if(rx_ready){
		uint8_t ob = rx_data;
	}
}