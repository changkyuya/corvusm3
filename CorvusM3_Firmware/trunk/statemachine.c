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
#include "tools.h"
#include "receiverppm.h"
#include "hal.h"

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[6];  //initsystem
char x [10];  // for Sensor Tests
vu16 stateLoopCount = 0;
vu16 receiverChannel[9]; 


	
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
	
	/* Debug Output 10Hz ---------------------------------------------------*/
	if (stateLoopCount % 100 == 0)
	{
		/* Graphoutput must start with 'G' and first value is timer (not used) */
		/*
		sprintf(x,"G-Gyro-ACC:1:%d:",ADCSensorValue[GYRO_X]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[GYRO_Y]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[GYRO_Z]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[ACC_X]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[ACC_Y]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",ADCSensorValue[ACC_Z]);
		print_uart1(x);
		*/
		
		/* test receiver */
		getChannels();
		//here it does not work :-(

		
		sprintf(x,"R-state:1:%d:",receiverChannel[0]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[1]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[2]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[3]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[4]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[5]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[6]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[7]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",receiverChannel[8]);
		print_uart1(x);	
		
		
		/* toggle LED */
		*LED ^= 1;
		//Pause(1000);
	}
	
	/* Count Loops till 1 second */
	stateLoopCount++;
	if (stateLoopCount > 1000) 
	{ 
		stateLoopCount = 0; 
	}
}
