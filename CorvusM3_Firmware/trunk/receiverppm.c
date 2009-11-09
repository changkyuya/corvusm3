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

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu16 	IC2Value;
vu16 	lastIC2Value;
vu8		channel;

typedef struct
{
  u16 ch1;
  u16 ch2;
  u16 ch3;
  u16 ch4;
  u16 ch5;
  u16 ch6;
  u16 ch7;
  u16 ch8;
}pulswidthStruct;

volatile pulswidthStruct pulswidth;

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
		//vu16 length = IC2Value - lastIC2Value; 
		vu16 length = IC2Value - lastIC2Value; 

		if(length > 20000) 
		{
			channel = 0; 
		}
		switch(channel) 
		{ 
			case 1: 
				pulswidth.ch1 = length; 
				break; 
			case 2: 
				pulswidth.ch2 = length; 
				break; 
			case 3: 
				pulswidth.ch3 = length; 
				break; 
			case 4: 
				pulswidth.ch4 = length; 
				break; 
			case 5: 
				pulswidth.ch5 = length; 
				break; 
			case 6: 
				pulswidth.ch6 = length; 
				break; 
			case 7: 
				pulswidth.ch7 = length; 
				break; 
			case 8: 
				pulswidth.ch8 = length; 
				break; 
			default: 
				channel = 0; 
				break; 
		} 
		lastIC2Value = IC2Value; 
		if(channel == 6) GPIO_SetBits(GPIOA, GPIO_Pin_8); else GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
		channel++; 
	} 
}


