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
#define _I2C

/* Include STM fwlib --------------------------------------------------------*/
#include "stm32fwlib.h"
/* tools */
#include "tools.h"
#include "led.h"

/* Define the hardware depending --------------------------------------------*/ 
/* Map Pins to Serial 1 */
#define  GPIO_TxPin1               GPIO_Pin_9
#define  GPIO_RxPin1   			   GPIO_Pin_10       
/* Map Pins to Serial 2 */
#define  GPIO_TxPin2               GPIO_Pin_2
#define  GPIO_RxPin2        	   GPIO_Pin_3
/* Map Pins to Serial 3 */
#define  GPIO_TxPin3               GPIO_Pin_10
#define  GPIO_RxPin3               GPIO_Pin_11
/* Map Pins to Serial 4 */
#define  GPIO_TxPin4               GPIO_Pin_10
#define  GPIO_RxPin4               GPIO_Pin_11
/* Map Pin for LED 1 --> PC13 */
#define LED PERIPHERAL_BIT(GPIOC->ODR, 13)			
#define GPIO_LEDPIN GPIO_Pin_13
/* Map Pin for Debug --> PA00 */
#define DBG PERIPHERAL_BIT(GPIOA->ODR, 0)			
#define GPIO_DBG GPIO_Pin_0
/* Map Pin for Beeper --> PA01 */
#define BEEPER PERIPHERAL_BIT(GPIOA->ODR, 1)			
#define GPIO_BEEPER GPIO_Pin_1
/* Map Pin for Servo0 --> PC06 */
#define SERVO0 PERIPHERAL_BIT(GPIOC->ODR, 6)			
#define GPIO_SERVO0 GPIO_Pin_6
/* Map Pin for Servo1 --> PC07 */
#define SERVO1 PERIPHERAL_BIT(GPIOC->ODR, 7)			
#define GPIO_SERVO1 GPIO_Pin_7
/* Map Pin for Servo2 --> PC08 */
#define SERVO2 PERIPHERAL_BIT(GPIOC->ODR, 8)			
#define GPIO_SERVO2 GPIO_Pin_8
/* Map Pin for Servo3 --> PC09 */
#define SERVO3 PERIPHERAL_BIT(GPIOC->ODR, 9)			
#define GPIO_SERVO3 GPIO_Pin_9
/* Map Pins for ACC */
#define GPIO_ACC_Z GPIO_Pin_3
#define GPIO_ACC_Y GPIO_Pin_4
#define GPIO_ACC_X GPIO_Pin_5
/* Map Pins for Gyro */
#define GPIO_GYRO_Z GPIO_Pin_0
#define GPIO_GYRO_X GPIO_Pin_1
#define GPIO_GYRO_Y GPIO_Pin_2
/* Map Pin for V */
#define GPIO_V GPIO_Pin_0
/* define adress from Compass */
#define HMC5843_ADDRESS         0x3C
#define MAG_I2C_Speed           100000

/* Define -------------------------------------------------------------------*/
#define FALSE	0
#define OK		1
#define TRUE	1
#define X		0
#define Y		1
#define Z		2
/* define led statis */
#define LED_OFF 0
#define LED_FLASH 1
#define LED_BLINK 2
#define LED_ON 3
/* places for ADC*/
#define GYRO_X 	0
#define GYRO_Y 	1
#define GYRO_Z 	2
#define ACC_X	3
#define ACC_Y	4
#define ACC_Z	5
#define VOLT	6
/* Receiver state */
#define REC_NO 	0
#define REC_OK		1
#define PPM_NO		0
#define PPM_OK 		1
#define SPEKTRUM_NO		0
#define SPEKTRUM_OK 	1
/* Receiver Channels */
#define RC_OK		0
#define PITCH		1
#define ROLL		2
#define NICK		3
#define YAW			4
/* define error codes -------------------------------------------------------*/
#define ERROR_AKKU			1
#define ERROR_RC			2
#define ERROR_SENSOR		4
/* flight states ------------------------------------------------------------*/
#define FLIGHT_START		0
#define FLIGHT_RC			1
#define FLIGHT_MOTOR 		2
#define FLIGHT_FLYING		3
/* define remote commands ---------------------------------------------------*/
#define RC_NO				0
#define RC_MOTORS			1
#define RC_CALIBRATE		2
#define RC_NEUTRAL			99
/* define Parameter Array ---------------------------------------------------*/
#define PARA_SET			0
#define PARA_DEBUG 			1
#define PARA_HW				2
#define PARA_VOLT   		3	
#define PARA_SW				4
#define PARA_ACC_X_ZERO		5
#define PARA_ACC_Y_ZERO		6
#define PARA_ACC_Z_ZERO		7
#define PARA_SMOOTH_ACC		8
#define PARA_SMOOTH_GYRO	9
#define PARA_SMOOTH_RC		10
#define PARA_GYRO_X_90		11
#define PARA_GYRO_Y_90		12
#define PARA_GYRO_Z_90		13
#define PARA_ACC_FORCE		14
#define PARA_GYRO_CORR		15
#define PARA_SERVO0_CH		16
#define PARA_SERVO1_CH		17
#define PARA_SERVO2_CH		18
#define PARA_SERVO3_CH		19
#define PARA_CAM_X			20
#define PARA_CAM_Y			21
#define PARA_MIN_GAS		22
#define PARA_P_X			23
#define PARA_I_X			24
#define PARA_D_X			25
#define PARA_P_Y			26
#define PARA_I_Y			27
#define PARA_D_Y			28
#define PARA_P_Z			29
#define PARA_I_Z			30
#define PARA_D_Z			31
#define PARA_COMP_CORR_X	32
#define PARA_COMP_CORR_Y	33
#define PARA_COMP_CORR_Z	34
#define PARA_COMP_DECL		35
#define PARA_COMP_FORCE		36
#define USED_PARAMETER 		36
/* define Para debug values */
#define PARA_DEBUG_REC  	0x01
#define PARA_DEBUG_ADC		0x02
#define PARA_DEBUG_SENSOR 	0x04
#define PARA_DEBUG_MOT 		0x08
#define PARA_DEBUG_COMP		0x10
/* define Para HW values */
#define PARA_HW_RC			0x00
#define PARA_HW_PC			0x01
#define PARA_HW_4PLUS		0x00
#define PARA_HW_4X			0x02

/* define Para SW values */
#define PARA_SW_REGLER		0xF  //bitmask 1111
#define PARA_SW_HH			0x00 // to bitmask 1111
#define PARA_SW_ACC			0x01 // to bitmask 1111
#define PARA_SW_COMP2		0x02 // to bitmask 1111


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