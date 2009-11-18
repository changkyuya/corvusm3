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

#include "receiverspektrum.h"

//#include "serial.h" //test
//#include <stdio.h> //test 

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 msCount;
extern vu8 spektrumBytes[33];  // 0 ... status 1-32 ... bytes from receiver
extern vu16 receiverChannel[9];
vu16 receiverSpektrumChannel[17];
extern vu32 oldSpektrumMsCount; // for failsave



/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels()
{
	//char x[100];
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
			receiverSpektrumChannel[i] = (spektrumBytes[i*2-1] << 8) + spektrumBytes[i*2]; 
			if (receiverSpektrumChannel[i] < 1024)
			{
				// Nick
				receiverChannel[3] = constrain(receiverSpektrumChannel[i] + 1000, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 2048)
			{
				// Pitch
				receiverChannel[1] = constrain(receiverSpektrumChannel[i] -24, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 3072)
			{
				// Roll
				receiverChannel[2] = constrain(receiverSpektrumChannel[i] - 1024, 1000, 2000);
			}
			else if (receiverSpektrumChannel[i] < 4096)
			{
				//Yaw
				receiverChannel[4] = constrain(receiverSpektrumChannel[i] - 2048, 1000, 2000);
			}
		}
	}
		//sprintf(x,"%d:%d-%d-%d-%d\r\n",receiverChannel[0],receiverChannel[1],receiverChannel[2],receiverChannel[3],receiverChannel[4]);
		//print_uart1(x);

}





