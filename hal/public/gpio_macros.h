/*
 * gpio_macros.h
 *
 *  Created on: Jun 21, 2025
 *      Author: niral
 */

#ifndef PUBLIC_GPIO_MACROS_H_
#define PUBLIC_GPIO_MACROS_H_

/*GPIO A and GPIO B clock bits are in register: RCC_AHB1RSTR :
 *Bit 0 (GPIO A) Bit 1(GPIO B) Bit 2(GPIO C) and so on.. : Reset value is 0 */
#define GPIOA_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<0)
#define GPIOB_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<1)
#define GPIOC_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<2)
#define GPIOD_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<3)
#define GPIOE_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<4)
#define GPIOF_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<5)
#define GPIOG_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<6)
#define GPIOH_clock_enable() RCC->rcc_ahb1enr = RCC->rcc_ahb1enr | (1<<7)

/*Disable the clock bits for GPIO Ports */
#define GPIOA_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<0))
#define GPIOB_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<1))
#define GPIOC_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<2))
#define GPIOD_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<3))
#define GPIOE_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<4))
#define GPIOF_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<5))
#define GPIOG_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<6))
#define GPIOH_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<7))

/*Enable USART2 Clock */
#define USART2_clock_enable()  RCC->rcc_apb1rstr = RCC->rcc_ahb1rstr | (1<<17)
#define USART2_clock_disable() RCC->rcc_ahb1rstr = ~(RCC->rcc_ahb1rstr & (1<<17))
#endif /* PUBLIC_GPIO_MACROS_H_ */
