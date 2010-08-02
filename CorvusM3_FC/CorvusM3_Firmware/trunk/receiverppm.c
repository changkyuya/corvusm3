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

#include "receiverppm.h"

#include <stdio.h> //for debug
#include "serial.h" //for debug

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu16 	IC2Value;
vu16 	lastIC2Value;
vu8		channelCount;
extern vu16 parameter[0x190]; //parameter
vu16 receiverPPMChannel[9];
vu16 oldReceiverPPMChannel[9];


/* return if ppm is valid ---------------------------------------------------*/
u8 isPPMonline()
{
	return (u8)receiverPPMChannel[0];
}


/* read and trimm receiverChannels --------------------------------------------------*/
void getPPMChannels(vu16 * receiverChannel)
{
	receiverChannel[0] = receiverPPMChannel[0];
	u8 i;
	for (i = 1; i < 9; i++)
	{
		// div values and set min max from 1000 to 2000
		receiverChannel[i] = smoothValue(constrain(receiverPPMChannel[i]>>2, 1000, 2000),oldReceiverPPMChannel[i],parameter[PARA_SMOOTH_RC]);
		oldReceiverPPMChannel[i] = receiverChannel[i];
	}
}

/* Interrupt Handler for TIM1 -----------------------------------------------*/
/* [0xAC] TIM1 Capture/Compare Interrupt */
void TIM1_CC_IRQHandler(void) 
{ 	
#ifndef PPM_PA11
	if(TIM1_GetITStatus(TIM1_IT_CC1) == SET) 
	{ 
		/* Clear TIM1 Capture compare interrupt pending bit */ 
		TIM1_ClearITPendingBit(TIM1_IT_CC1); 
		
		//TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
		TIM1_ClearFlag(TIM1_FLAG_CC1); 
		GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
		// /* Get the Input Capture value */ 
		IC2Value = TIM1_GetCapture1(); 
#else		
	if(TIM1_GetITStatus(TIM1_IT_CC4) == SET) 
	{ 
		/* Clear TIM1 Capture compare interrupt pending bit */ 
		TIM1_ClearITPendingBit(TIM1_IT_CC4); 
		
		//TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
		TIM1_ClearFlag(TIM1_FLAG_CC4); 
		GPIO_ResetBits(GPIOA, GPIO_Pin_11); 
		// /* Get the Input Capture value */ 
		IC2Value = TIM1_GetCapture4(); 
#endif	

		vu16 length = IC2Value - lastIC2Value; 

		// find the start sync gap
		// 4000 points are 1ms
		if(length > 20000)   //5ms
		{
			channelCount = 0; 
		}
		else
		{
			if(channelCount > 0 && channelCount < 9)
			{
				// if signal is OK 
				if (length > 3000 && length < 9000)    //>0.75ms && <2.25ms 
				{
					receiverPPMChannel[channelCount] = length;
					// one set complete
					if(channelCount > 4)
					{
						receiverPPMChannel[0] = PPM_OK;
					} 
				}
				else
				{
					receiverPPMChannel[0] = PPM_NO;
				}
			}
			else
			{
				receiverPPMChannel[0] = PPM_NO;
				channelCount = 0;
			}
		}
		// store last time
		lastIC2Value = IC2Value; 
#ifndef PPM_PA11
		if(channelCount == 6) GPIO_SetBits(GPIOA, GPIO_Pin_8); else GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
#else
		if(channelCount == 6) GPIO_SetBits(GPIOA, GPIO_Pin_11); else GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#endif
		// increase channelCount
		channelCount++;
	} 
}


