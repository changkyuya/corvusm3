/*	This file is part of the Corvus M3 project

	http://corvusm3.blogspot.com/
	http://code.google.com/p/corvusm3/	

	2009, Thorsten Raab - thorsten.raab@gmx.at

    Corvus M3 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Corvus M3 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corvus M3.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Define for STM fwlib load ------------------------------------------------*/
#define _DMA
#define _DMA_Channel1
#define _TIM
#define _TIM1
#define _TIM8
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
#define _SysTick

/* Include STM fwlib --------------------------------------------------------*/
#include "stm32fwlib.h"
/* tools */
#include "tools.h"
#include "led.h"

/* Define the hardware depending --------------------------------------------*/ 
/* Map Pins to Serial 1 */
#define  GPIO_TxPin1               GPIO_Pin_9
#define  GPIO_RxPin1   			   GPIO_Pin_10
/* Map Pin for LED 1 --> PC13 */
#define LED 						PERIPHERAL_BIT(GPIOC->ODR, 13)			
#define GPIO_LEDPIN 				GPIO_Pin_13

/* 1. BLMC ------------------------------------------------------------------*/
/* ADC */
#define GPIO_1_ADC_A				GPIO_Pin_0
#define GPIO_1_ADC_B				GPIO_Pin_1
#define GPIO_1_ADC_C				GPIO_Pin_2
#define GPIO_1_ADC_S				GPIO_Pin_3
/* PWM */
#define GPIO_1_PWM_A				GPIO_Pin_9
#define GPIO_1_PWM_B				GPIO_Pin_8
#define GPIO_1_PWM_C				GPIO_Pin_6
/* SWITCH */
#define GPIO_1_SWITCH_A				GPIO_Pin_2
#define GPIO_1_SWITCH_B				GPIO_Pin_12
#define GPIO_1_SWITCH_C				GPIO_Pin_11
#define 1_SWITCH_A					PERIPHERAL_BIT(GPIOD->ODR, 2)
#define 1_SWITCH_B					PERIPHERAL_BIT(GPIOC->ODR, 12)
#define 1_SWITCH_C					PERIPHERAL_BIT(GPIOC->ODR, 11)

/* 2. BLMC ------------------------------------------------------------------*/
/* ADC */
#define GPIO_2_ADC_A				GPIO_Pin_0
#define GPIO_2_ADC_B				GPIO_Pin_1
#define GPIO_2_ADC_C				GPIO_Pin_2
#define GPIO_2_ADC_S				GPIO_Pin_3
/* PWM */
#define GPIO_2_PWM_A				GPIO_Pin_5
#define GPIO_2_PWM_B				GPIO_Pin_4
#define GPIO_2_PWM_C				GPIO_Pin_3
/* SWITCH */
#define GPIO_2_SWITCH_A				GPIO_Pin_10
#define GPIO_2_SWITCH_B				GPIO_Pin_15
#define GPIO_2_SWITCH_C				GPIO_Pin_14
#define 2_SWITCH_A					PERIPHERAL_BIT(GPIOC->ODR, 10)
#define 2_SWITCH_B					PERIPHERAL_BIT(GPIOA->ODR, 15)
#define 2_SWITCH_C					PERIPHERAL_BIT(GPIOA->ODR, 14)

/* 3. BLMC ------------------------------------------------------------------*/
/* ADC */
#define GPIO_3_ADC_A				GPIO_Pin_4
#define GPIO_3_ADC_B				GPIO_Pin_5
#define GPIO_3_ADC_C				GPIO_Pin_6
#define GPIO_3_ADC_S				GPIO_Pin_7
/* PWM */
#define GPIO_3_PWM_A				GPIO_Pin_13
#define GPIO_3_PWM_B				GPIO_Pin_12
#define GPIO_3_PWM_C				GPIO_Pin_15
/* SWITCH */
#define GPIO_3_SWITCH_A				GPIO_Pin_11
#define GPIO_3_SWITCH_B				GPIO_Pin_8
#define GPIO_3_SWITCH_C				GPIO_Pin_9
#define 3_SWITCH_A						PERIPHERAL_BIT(GPIOA->ODR, 13)
#define 3_SWITCH_B						PERIPHERAL_BIT(GPIOA->ODR, 12)
#define 3_SWITCH_C						PERIPHERAL_BIT(GPIOB->ODR, 15)

/* 4. BLMC ------------------------------------------------------------------*/
/* ADC */
#define GPIO_4_ADC_A				GPIO_Pin_4
#define GPIO_4_ADC_B				GPIO_Pin_5
#define GPIO_4_ADC_C				GPIO_Pin_0
#define GPIO_4_ADC_S				GPIO_Pin_1
/* PWM */
#define GPIO_4_PWM_A				GPIO_Pin_8
#define GPIO_4_PWM_B				GPIO_Pin_7
#define GPIO_4_PWM_C				GPIO_Pin_6
/* SWITCH */
#define GPIO_4_SWITCH_A				GPIO_Pin_14
#define GPIO_4_SWITCH_B				GPIO_Pin_13
#define GPIO_4_SWITCH_C				GPIO_Pin_12
#define 4_SWITCH_A						PERIPHERAL_BIT(GPIOB->ODR, 14)
#define 4_SWITCH_B						PERIPHERAL_BIT(GPIOB->ODR, 13)
#define 4_SWITCH_C						PERIPHERAL_BIT(GPIOB->ODR, 12)


/* Struct --------------------------------------------------------------------*/


// General function - no external variable involved
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define map(x,inMin,inMax,outMin,outMax) ((((inMax)-(inMin))>((outMax)-(outMin)))?\
					((((x)-(inMin))/(((inMax)-(inMin))/((outMax)-(outMin))))+(outMin)):\
					((((x)-(inMin))*(((outMax)-(outMin))/((inMax)-(inMin))))+(outMin))\
					)
#define constrain(x,low,high) ((((x)>=(low))&&((x)<=(high)))?(x):((x)>(high)?(high):(low)))
#define sq(x) ((x)*(x))

#endif /* __MAIN_H */