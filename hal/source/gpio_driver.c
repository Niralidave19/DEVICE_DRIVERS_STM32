/*
 * gpio_driver.c
 *
 *  Created on: Jun 17, 2025
 *      Author: niral
 */

#include"stm32f446xx.h"
#include<stdint.h>
#include<stddef.h>
#include"gpio_types.h"
#include"gpio_macros.h"

#ifndef GPIO_PROTO_H_
#define GPIO_PROTO_H_
#endif

/****************GPIO_Clock(uint8_t enordis, gpio_port_def_t GPIO_ID)**********
 * Description: To enable/disable clock for a specific GPIO Port***************
 * Parameters : uint8_t enordis : denotes if the clock must be enabled/disabled
 *              gpio_port_def_t GPIO_ID : Specific Port ID for which clock needs
 *              to be enabled or disabled 
 * ***************************************************************************/
void GPIO_Clock(gpio_port_def_t GPIO_ID,gpio_enable_t enordis){
	if(enordis == 1){
		switch (GPIO_ID){
		case GPIO_ID_A:
			GPIOA_clock_enable();
			break;
		case GPIO_ID_B:
			GPIOB_clock_enable();
			break;
		case GPIO_ID_C:
			GPIOC_clock_enable();
			break;
		case GPIO_ID_D:
			GPIOD_clock_enable();
			break;
		case GPIO_ID_E:
			GPIOE_clock_enable();
			break;
		case GPIO_ID_F:
			GPIOF_clock_enable();
			break;
		case GPIO_ID_G:
			GPIOG_clock_enable();
			break;
		case GPIO_ID_H:
			GPIOH_clock_enable();
			break;
		}
	}
	else if(enordis == 0){
		switch (GPIO_ID){
		case GPIO_ID_A:
			GPIOA_clock_disable();
			break;
		case GPIO_ID_B:
			GPIOB_clock_disable();
			break;
		case GPIO_ID_C:
			GPIOC_clock_disable();
			break;
		case GPIO_ID_D:
			GPIOD_clock_disable();
			break;
		case GPIO_ID_E:
			GPIOE_clock_disable();
			break;
		case GPIO_ID_F:
			GPIOF_clock_disable();
			break;
		case GPIO_ID_G:
			GPIOG_clock_disable();
			break;
		case GPIO_ID_H:
			GPIOH_clock_disable();
			break;
		}
	}
}

/***********************GPIO_GetPort(gpio_port_def_t GPIO_ID)************************
 * Description: Returns a pointer, pointing to the base address of the GPIOX register
 * Parameters : gpio_port_def_t GPIO_ID : Specific Port ID for which the base address
 *              is returned 
 * *********************************************************************************/
volatile GPIO_Reg_def_t* GPIO_GetPort(gpio_port_def_t GPIO_ID){
	switch (GPIO_ID){
	case GPIO_ID_A:
		return (volatile GPIO_Reg_def_t*)GPIOA_BASE_ADDRESS;
	case GPIO_ID_B:
		return (volatile GPIO_Reg_def_t*)GPIOB_BASE_ADDRESS;
	case GPIO_ID_C:
		return (volatile GPIO_Reg_def_t*)GPIOC_BASE_ADDRESS;
	case GPIO_ID_D:
		return (volatile GPIO_Reg_def_t*)GPIOD_BASE_ADDRESS;
	case GPIO_ID_E:
		return (volatile GPIO_Reg_def_t*)GPIOE_BASE_ADDRESS;
	case GPIO_ID_F:
		return (volatile GPIO_Reg_def_t*)GPIOF_BASE_ADDRESS;
	case GPIO_ID_G:
		return (volatile GPIO_Reg_def_t*)GPIOG_BASE_ADDRESS;
	case GPIO_ID_H:
		return (volatile GPIO_Reg_def_t*)GPIOH_BASE_ADDRESS;
	}
	return NULL;
}

/***************GPIO_Init(gpio_config_t gpio)******************************************************
 * Description: Initialize GPIO Port
 * Prameters : gpio_config_t gpio : This is the GPIO pin configuration that has to be initialised 
 * ***********************************************************************************************/
void GPIO_Init(gpio_config_t gpio){

	uint32_t temp = 0;
	GPIO_Clock(gpio.port_id,gpio.enable);
	/* Get the GPIO register for the GPIO_ID */
	volatile GPIO_Reg_def_t *pGPIOx = GPIO_GetPort(gpio.port_id);

	/*Set the mode of the GPIO Pin - MODER REGISTER*/
	pGPIOx->gpio_moder &= ~(3 << (gpio.pin_number * 2));
	pGPIOx->gpio_moder |= (gpio.mode << (gpio.pin_number * 2));

	/*set the output speed of the pin*/
	temp = 0;
	temp = temp | (gpio.op_speed << (gpio.pin_number *2));
	pGPIOx->gpio_ospeedr &= ~(3<<(gpio.pin_number * 2));
	pGPIOx->gpio_ospeedr = pGPIOx->gpio_ospeedr | temp;

	/* Set if the pin needs to be treated as a pull up/pull down pin -
	 * Usually used when pin is in input mode */
	temp = 0;
	temp = temp | (gpio.push_pull << (gpio.pin_number *2));
	pGPIOx->gpio_pupdr &= ~(3<<(gpio.pin_number * 2));
	pGPIOx->gpio_pupdr = pGPIOx->gpio_pupdr | temp;

	/*set output type register*/
	temp = 0;
	temp = temp | (gpio.op_type << (gpio.pin_number *2));
	pGPIOx->gpio_otyper &= ~(1<<(gpio.pin_number));
	pGPIOx->gpio_otyper = pGPIOx->gpio_otyper | temp;

	/*set alternate functionality mode register*/
	if(gpio.pin_number <= 7){
		//configure alternate functionality low register
		temp = 0;
		temp = temp | (gpio.alt_func << (gpio.pin_number *4));
		pGPIOx->gpio_aflr &= ~(15<<(gpio.pin_number));
		pGPIOx->gpio_aflr = pGPIOx->gpio_aflr | temp;
	}
	else if(gpio.pin_number > 7){
		//configure alternate functionality High register
		temp = 0;
		temp = temp | (gpio.alt_func << (gpio.pin_number *4));
		pGPIOx->gpio_afrh &= ~(15<<(gpio.pin_number));
		pGPIOx->gpio_afrh = pGPIOx->gpio_afrh | temp;
	}
}

/***************delay(uint16_t time)*************************************************************
 * Description: Delay function
 * Prameters : uint16_t time : The duration of delay 
 * ***********************************************************************************************/
void delay(uint16_t time){
	while(time!=0)
	{
		time--;
	}
}

/************GPIO_ReadfromInput_pin(GPIO_Reg_def_t *pGPIOx,uint8_t GPIO_PinNumber)**
 * Description: Returns 1 byte of data read from the Input Data Register
 * Parameters : GPIO_Reg_def_t pGPIOx : The GPIO port 
 * 				uint8_t GPIO_PinNumber
 * *********************************************************************************/
uint8_t GPIO_ReadfromInput_pin(GPIO_Reg_def_t *pGPIOx,uint8_t GPIO_PinNumber){
	uint8_t temp=0;
	temp = ((pGPIOx->gpio_idr)>>(GPIO_PinNumber))&1;
	return temp;
}

/************GPIO_WritetoPin(gpio_config_t gpio,uint8_t value)**********************
 * Description: Returns 1 byte of data read from the Input Data Register
 * Parameters : GPIO_Reg_def_t pGPIOx : The GPIO port 
 * 				uint8_t GPIO_PinNumber
 * *********************************************************************************/
uint8_t GPIO_WritetoPin(gpio_config_t gpio,uint8_t value){
	/* Get the GPIO register for the GPIO_ID */
	volatile GPIO_Reg_def_t *pGPIOx = GPIO_GetPort(gpio.port_id);
	if(value == 1){
		pGPIOx->gpio_odr &= ~(1 << gpio.pin_number);
		pGPIOx->gpio_odr = pGPIOx->gpio_odr | (value << gpio.pin_number);
	}
	if(value == 0){
		pGPIOx->gpio_odr &= ~(1 << gpio.pin_number);
		pGPIOx->gpio_odr = pGPIOx->gpio_odr | (value << gpio.pin_number);
	}
}
