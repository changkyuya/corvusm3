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
#ifndef __MAIN_H
#define __MAIN_H

/* Define for STM fwlib load ------------------------------------------------*/
#define _DMA
#define _DMA_Channel1
#define _TIM
#define _EXTI
#define _GPIO
#define _RCC
#define _USART
#define _NVIC
#define _USART
#define _USART1
#define _PWR
#define _ADC
#define _FLASH
#define _FLASH_PROG

/* Include STM fwlib --------------------------------------------------------*/
#include "stm32fwlib.h"

/* Define the hardware depending --------------------------------------------*/ 
/* Map Pins to Serial 1 */
#define  GPIO_SER                 GPIOA
#define  RCC_APB2Periph_GPIOx     RCC_APB2Periph_GPIOA
#define  GPIO_RTSPin              GPIO_Pin_12
#define  GPIO_CTSPin              GPIO_Pin_11
#define  GPIO_TxPin               GPIO_Pin_9
#define  GPIO_RxPin               GPIO_Pin_10
/* Map Pin for LED 1 --> PC13 */
#define LED PERIPHERAL_BIT(GPIOC->ODR, 13)					
#define RCC_APB2Periph_GPIO_LED RCC_APB2Periph_GPIOC
#define GPIO_LEDPIN GPIO_Pin_13
#define GPIO_LED GPIOC
/* Map Pins for ACC */
#define GPIO_SEN GPIOC  // for ACC and Gyro
#define GPIO_ACC_Z GPIO_Pin_3
#define GPIO_ACC_Y GPIO_Pin_4
#define GPIO_ACC_X GPIO_Pin_5
/* Map Pins for Gyro */
#define GPIO_GYRO_Z GPIO_Pin_0
#define GPIO_GYRO_X GPIO_Pin_1
#define GPIO_GYRO_Y GPIO_Pin_2

/* Define -------------------------------------------------------------------*/
/* define led statis */
#define LED_OFF 0
#define LED_FLASH 1
#define LED_BLINK 2
#define LED_ON 3
/* XYZ */
#define GYRO_X 	0
#define GYRO_Y 	1
#define GYRO_Z 	2
#define ACC_X	3
#define ACC_Y	4
#define ACC_Z	5




#endif /* __MAIN_H */