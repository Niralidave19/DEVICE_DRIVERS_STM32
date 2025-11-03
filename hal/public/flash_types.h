/*
 * flash_types.h
 *
 *  Created on: Jul 30, 2025
 *      Author: niral
 */

#ifndef PUBLIC_FLASH_TYPES_H_
#define PUBLIC_FLASH_TYPES_H_

#include<stdint.h>

typedef struct
{
  volatile uint32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
  volatile uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
  volatile uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
  volatile uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
  volatile uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
  volatile uint32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
  volatile uint32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
} FLASH_TypeDef;

#define FLASH_CR_LOCK		(1 << 31)
#define FLASH_CR_PSIZE		8
#define FLASH_CR_PG			(1 << 0)
#define FLASH_CR_SER		(1 << 1)
#define FLASH_CR_SNB		3
#define FLASH_CR_START		(1 << 16)
#define FLASH_SR_BSY		(1 << 16)

#define FLASH_ACR_ICEN		(1 << 9)
#define FLASH_ACR_ICRST		(1 << 11)
#define FLASH_ACR_DCEN		(1 << 10)
#define FLASH_ACR_DCRST		(1 << 12)



#endif /* PUBLIC_FLASH_TYPES_H_ */
