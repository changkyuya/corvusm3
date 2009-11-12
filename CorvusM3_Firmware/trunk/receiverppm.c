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

#include "receiverppm.h"

#include <stdio.h> //for debug
#include "serial.h" //for debug

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu16 	IC2Value;
vu16 	lastIC2Value;
vu8		channelCount;

vu16 receiverChannel[9];
vu16 receiverPPMChannel[9];
//volatile pulswidthStruct pulswidth;
//extern ChannelStruct receiverChannel;  //statemachine

char x [10];  // for Debug

/* read and trimm receiverChannels --------------------------------------------------*/
void getPPMChannels()
{
	u8 i;
	for (i = 0; i < 9; i++)
	{
		receiverChannel[i] = receiverPPMChannel[i];
	}
	
	if (receiverChannel[0] == PPM_OK)
	{
		// only for test
		sprintf(x,"R-Receiver:1:%d:",receiverChannel[1]);
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
		
	}
}

/* Interrupt Handler for TIM1 -----------------------------------------------*/
/* [0xAC] TIM1 Capture/Compare Interrupt */
void TIM1_CC_IRQHandler(void) 
{ 	
	if(TIM1_GetITStatus(TIM1_IT_CC1) == SET) 
	{ 
		/* Clear TIM1 Capture compare interrupt pending bit */ 
		TIM1_ClearITPendingBit(TIM1_IT_CC1); 
		
		//TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
		TIM1_ClearFlag(TIM1_FLAG_CC1); 
		GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
		
		// /* Get the Input Capture value */ 
		IC2Value = TIM1_GetCapture1(); 
		vu16 length = IC2Value - lastIC2Value; 

		if(length > 20000) 
		{
			channelCount = 0; 
			receiverChannel[0] = PPM_SYNC;	
		}
		switch(channelCount) 
		{ 
			case 1: 
				receiverPPMChannel[1] = length>>2; 
				break; 
			case 2: 
				receiverPPMChannel[2] = length>>2; 
				break; 
			case 3: 
				receiverPPMChannel[3] = length>>2; 
				break; 
			case 4: 
				receiverPPMChannel[4] = length>>2; 
				break; 
			case 5: 
				receiverPPMChannel[5] = length>>2; 
				break; 
			case 6: 
				receiverPPMChannel[6] = length>>2; 
				break; 
			case 7: 
				receiverPPMChannel[7] = length>>2; 
				break; 
			case 8: 
				receiverPPMChannel[8] = length>>2; 
				receiverPPMChannel[0] = PPM_OK;
				/*
				u8 i;
				for (i = 0; i < 9; i++)
				{
					receiverChannel[i] = receiverPPMChannel[i];
				}
				*/
				break; 
			default: 
				channelCount = 0; 
				receiverPPMChannel[0] = PPM_WRONG;
				break; 
		} 
		lastIC2Value = IC2Value; 
		if(channelCount == 6) GPIO_SetBits(GPIOA, GPIO_Pin_8); else GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
		channelCount++; 
	} 
}


