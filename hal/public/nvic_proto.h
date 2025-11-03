/*
 * nvic_proto.h
 *
 *  Created on: Oct 30, 2025
 *      Author: niral
 */

#ifndef PUBLIC_NVIC_PROTO_H_
#define PUBLIC_NVIC_PROTO_H_

#include<stdint.h>
#include "nvic_types.h"

void NVIC_enable_IRQ(uint32_t IRQn);
void NVIC_set_priority_grouping(uint8_t preempt_bits);
void NVIC_set_priority(uint32_t IRQn,uint8_t Priority);
void NVIC_init(NVIC_config_t nvic_config);

#endif /* PUBLIC_NVIC_PROTO_H_ */