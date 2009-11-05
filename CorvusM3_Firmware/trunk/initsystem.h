/*	Part of the Corvus M3 project

	http://corvusm3.blogspot.com/
	http://code.google.com/p/corvusm3/	

	Copyright (c) 2009 Thorsten Raab - thorsten.raab@gmx.at

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INITSYSTEM_H
#define __INITSYSTEM_H

/* 	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC1_DR_Address 
	(u32)ADC1_DR_Address should be set as follows. 
	Take the base address of ADC1, i.e. (u32)ADC1_BASE. 
	Then need to add an offset of 19 32-bit registers, i.e. 19 * 4 bytes = 76d= 0x4C. 
	19 because there are 19 registers associated with ADC1 and the result is the last one. 
	By definition, ADC1_BASE = (APB2PERIPH_BASE + 0x2400); 
	APB2PERIPH_BASE = (PERIPH_BASE + 0x10000); 
	PERIPH_BASE = ((u32)0x40000000). 
	Adding these numbers together, the base address should be 4001244C.  */
#define ADC1_DR_Address    ((u32)0x4001244C)

/* Function prototypes -------------------------------------------------------*/
void initSystem(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void initUART1 (void);
void initDMA (void);
void initADC (void) ;

#endif /* __INITSYSTEM_H */