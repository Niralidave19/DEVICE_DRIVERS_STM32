/*
* flash.c
*
*  Created on: Jul 23, 2025
*      Author: niral
*/
#include"flash_types.h"
#include"stm32f446xx.h"
void Flash_Write(void)
{
    /* Unlock flash */
	/* Write KEY1 */
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;

	/* wait till FLash CR is unlocked */
	while(FLASH->CR & FLASH_CR_LOCK);
	/* Code reaching here indicates FLASH CR is unlocked */

	/* Wait till operation over */
	while(FLASH->SR & FLASH_SR_BSY);
	/* If interrupts are enabled check for EOP */

	/* Write a byte to flash */
	/* Set PSIZE (00 for byte writing) */
	/* CLear 8th and 9th bit */
	FLASH->CR &= ~(0x3 << FLASH_CR_PSIZE);
	/* Set bits to correct value (0 in case of byte writing) */
	FLASH->CR |= 0 << FLASH_CR_PSIZE;

	/* sector erase */
	/* wait for flash operation to end */
	while(FLASH->SR & FLASH_SR_BSY);
	/* SER bit set */
	FLASH->CR |= FLASH_CR_SER;
	/* Clear SNB */
	FLASH->CR &= ~(0xF << FLASH_CR_SNB);
	/* Select sector 7 in SNB */
	FLASH->CR |= (0x7 << FLASH_CR_SNB);
	/* Start Sector clear */
	FLASH->CR |= FLASH_CR_START;
	/* Wait for end of operation */
	while(FLASH->SR & FLASH_SR_BSY);
	/* Clear SER */
	FLASH->CR &= ~(FLASH_CR_SER);
	/* Clear SNB */
	FLASH->CR &= ~(0xF << FLASH_CR_SNB);
	/* START gets cleared automatically */

	/* Set programming bit */
	FLASH->CR |= FLASH_CR_PG;
	/* write a byte */
	*((uint8_t *)START_ADDR) = (uint8_t)0x22;
	/* Wait for operation completion */
	while(FLASH->SR & FLASH_SR_BSY);
	/* Clear programming bit */
	FLASH->CR &= ~(FLASH_CR_PG);
	/* Wait for completion */
	while(FLASH->SR & FLASH_SR_BSY);

	/* Flush caches */
	/* Flush instruction cache  */
	if ((FLASH->ACR & FLASH_ACR_ICEN) != 0)
	{
		/* Disable instruction cache  */
		FLASH->ACR &= ~(FLASH_ACR_ICEN);
		/* Reset instruction cache */
		FLASH->ACR |= FLASH_ACR_ICRST;
		/* Enable instruction cache */
		FLASH->ACR |= FLASH_ACR_ICEN;
	}
	/* Flush data cache */
	if ((FLASH->ACR & FLASH_ACR_DCEN) != 0)
	{
		/* Disable data cache  */
		FLASH->ACR &= ~(FLASH_ACR_DCEN);
		/* Reset data cache */
		FLASH->ACR |= FLASH_ACR_DCRST;
		/* Enable data cache */
		FLASH->ACR |= FLASH_ACR_DCEN;
	}

	/* Wait for completion */
	while(FLASH->SR & FLASH_SR_BSY);
	/* Lock flash */
	FLASH->CR |= FLASH_CR_LOCK;
	/* Wait for completion */
	while(FLASH->SR & FLASH_SR_BSY);
}
