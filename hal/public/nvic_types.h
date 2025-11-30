/*
 * nvic_types.h
 *
 *  Created on: Oct 30, 2025
 *      Author: niral
 */

#ifndef PUBLIC_NVIC_TYPES_H_
#define PUBLIC_NVIC_TYPES_H_

#include<stdint.h>
#include "stm32f446xx.h"

/*NVIC_config_t : IRQn, preempt_bits, priority */
typedef struct{
    uint32_t IRQn;
    uint8_t preempt_bits;
    uint8_t priority;
}NVIC_config_t;

#endif /* PUBLIC_NVIC_TYPES_H_ */