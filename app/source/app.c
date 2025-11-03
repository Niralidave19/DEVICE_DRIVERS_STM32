/************************************************************************************
 * File name: app_init.c
 * Initializes all the applications with the respective drivers / lower layer support
 *************************************************************************************/

#include "app_uart_proto.h"
#include "app_gpio_proto.h"
#include "gpio_types.h"

extern gpio_config_t gpio_pin;

void app_init(){
	/*GPIO Initialisation to blink an LED*/
	app_led_blink_init();
	/* UART configuration */
	app_uart_init();
}

void app_update(){
	/* GPIO LED Blink update routine */
	app_led_blink_update(1,gpio_pin);
	/* UART recieve - Interrupt enabled UART routine*/
	app_uart_interrupt_recv_update();
}
