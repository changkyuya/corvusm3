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

#include "receiverspektrum.h"

#include "serial.h" //test
#include <stdio.h> //test 

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 msCount;  //statemachine
extern vu16 parameter[0xFF]; //parameter
extern vu8 spektrumBytes[33];  //serial, 0 ... status 1-32 ... bytes from receiver
vu16 receiverSpektrumChannel[17];
vu16 oldReceiverSpektrumChannel[17];
extern vu32 oldSpektrumMsCount; //serial, for failsave



/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels(vu16 * receiverChannel)
{

	u8 i;
	
	// OK Byte - test if uart3 interrupt is running
	// each 11ms one frame comes - 
	if ((oldSpektrumMsCount + 12) > msCount)
	{
		receiverChannel[0] = spektrumBytes[0];
	}
	// interrupt for uart3 is delay - no signal
	else
	{
		receiverChannel[0] = SPEKTRUM_NO;
	}
	// first 2 bytes are not used
	for (i = 2; i < 17; i++)
	{
		// first 2 byte from 2. frame also not used
		if (i != 9)
		{
			// shift first byte 8 steps to high and add low byte
			receiverSpektrumChannel[i] = (spektrumBytes[(i << 1) - 1] << 8) + spektrumBytes[i << 1]; 
			if (receiverSpektrumChannel[i] < 1024)
			{
				// resolution is 10bit ... 0 - 1024 ... add 988 for middle
				// Nick
				//receiverChannel[3] = smoothValue(constrain(receiverSpektrumChannel[i] + 988, 1000, 2000),oldReceiverSpektrumChannel[i],parameter[PARA_SMOOTH_RC]);
				//oldReceiverSpektrumChannel[i] = receiverChannel[3];
				receiverChannel[3] = constrain(receiverSpektrumChannel[i] + 988, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 2048)
			{
				// Pitch
				//receiverChannel[1] = smoothValue(constrain(receiverSpektrumChannel[i] - 36, 1000, 2000),oldReceiverSpektrumChannel[i],parameter[PARA_SMOOTH_RC]);
				//oldReceiverSpektrumChannel[i] = receiverChannel[1];
				receiverChannel[1] = constrain(receiverSpektrumChannel[i] - 36, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 3072)
			{
				// Roll
				//receiverChannel[2] = smoothValue(constrain(receiverSpektrumChannel[i] - 1060, 1000, 2000),oldReceiverSpektrumChannel[i],parameter[PARA_SMOOTH_RC]);
				//oldReceiverSpektrumChannel[i] = receiverChannel[2];
				receiverChannel[2] = constrain(receiverSpektrumChannel[i] - 1060, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 4096)
			{
				//Yaw
				//receiverChannel[4] = smoothValue(constrain(receiverSpektrumChannel[i] - 2084, 1000, 2000),oldReceiverSpektrumChannel[i],parameter[PARA_SMOOTH_RC]);
				//oldReceiverSpektrumChannel[i] = receiverChannel[4];
				receiverChannel[2] = constrain(receiverSpektrumChannel[i] - 1060, 1000, 2000);
			}
		}
	}
	
	//char x[100];
	//sprintf(x,"%d:%d-%d-%d-%d\r\n",spektrumBytes[0],spektrumBytes[1],spektrumBytes[2],spektrumBytes[3],spektrumBytes[4]);
	//print_uart1(x);

}





