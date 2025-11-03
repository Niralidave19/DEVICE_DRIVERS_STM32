/*
 * nvic_driver.c
 *
 *  Created on: Oct 30, 2025
 *      Author: niral
 */


#include<stdint.h>
#include "stm32f446xx.h"
#include "nvic_types.h"

static inline void enable_irq(void)
{
    __asm volatile("CPSIE i");
}


/*Enable IRQ line to NVIC*/
void NVIC_enable_IRQ(uint32_t IRQn){
    uint32_t iser_index = IRQn/32;
    uint32_t iser_bit = IRQn%32;
    volatile uint32_t *iser_reg = (uint32_t*)(NVIC_ISER_BASE + (iser_index*4));
    *iser_reg = (1U<<iser_bit);
    //__enable_irq();
}

/* NVIC_set_priority_grouping() function is used to set the priority grouping for an application */
void NVIC_set_priority_grouping(uint8_t preempt_bits){

    /* Decide the grouping based on the preempt bits set by used */
    uint8_t prigroup = (NVIC_PRIO_BITS - preempt_bits) & 0x7U;
    volatile uint32_t reg = *((volatile uint32_t*)SCB_AIRCR);

    /* Clear the VECT_KEY and write VECT_KEY */
    reg &= ~((0x7U << 8) | (0xFFFFUL << 16));

    /* Write the priority grouping into the register */
    reg |= AIRCR_VECTKEY | (prigroup << 8);
    *((uint32_t*)SCB_AIRCR) = reg;
}

/* NVIC_set_priority() function is used to set priority for a given interrupt*/
void NVIC_set_priority(uint8_t IRQn,uint8_t Priority){
    /* Priority needs to be set for IRQn - byte addressable*/
    volatile uint8_t *IRQ_IPR = (volatile uint8_t*)(NVIC_IPR_BASE + IRQn);
    *(IRQ_IPR) = (Priority << 4);
}

/*NVIC Init routine
  1. Sets the priority grouping
  2. Sets the priority of the given interrupt
  3. Enables IRQ */
void NVIC_init(NVIC_config_t nvic_config){
	//enable_irq();
    NVIC_set_priority_grouping(nvic_config.preempt_bits);
    NVIC_set_priority(nvic_config.IRQn,nvic_config.priority);
    NVIC_enable_IRQ(nvic_config.IRQn);
}
