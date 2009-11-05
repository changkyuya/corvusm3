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

/* Include ------------------------------------------------------------------*/
#include "main.h"
#include "statemachine.h"
#include "serial.h"
#include <stdio.h>
#include "led.h"

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[6];
char x [10];  // for Sensor Tests
vu16 stateLoopCount = 0;
	
/* [0xB4] TIM3 Interrupt ----------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	statemachine();
}

/* Main Control Loop --------------------------------------------------------*/
void statemachine(void)
{
	/* Clear TIM3 update interrupt */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	

	// test LED
	//setLEDStatus(LED_FLASH);
	
	/* Debug Output 100Hz ---------------------------------------------------*/
	if (stateLoopCount % 100 == 0)
	{
		sprintf(x,"-%d-",ADCSensorValue[GYRO_X]);
		print_uart1(x);
		sprintf(x,"-%d-",ADCSensorValue[GYRO_Y]);
		print_uart1(x);
		sprintf(x,"-%d-",ADCSensorValue[GYRO_Z]);
		print_uart1(x);
		sprintf(x,"-%d-",ADCSensorValue[ACC_X]);
		print_uart1(x);
		sprintf(x,"-%d-",ADCSensorValue[ACC_Y]);
		print_uart1(x);
		sprintf(x,"-%d-\r\n",ADCSensorValue[ACC_Z]);
		print_uart1(x);
		/* toggle LED */
		*LED ^= 1;
	}
	
	/* Count Loops till 1 second */
	stateLoopCount++;
	if (stateLoopCount > 1000) 
	{ 
		stateLoopCount = 0; 
	}
}
