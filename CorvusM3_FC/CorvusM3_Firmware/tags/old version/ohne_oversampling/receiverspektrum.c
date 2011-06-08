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
extern vu16 parameter[0x190]; //parameter
extern vu8 spektrumBytes[33];  //serial, 0 ... status 1-32 ... bytes from receiver
vu16 receiverSpektrumChannel[17];
vu16 oldReceiverSpektrumChannel[17];
extern vu32 oldSpektrumMsCount; //serial, for failsave


/* return if spektrum is valid ----------------------------------------------*/
u8 isSpektrumOnline()
{
	// OK Byte - test if uart3 interrupt is running
	// each 11ms one frame comes - 
	if ((oldSpektrumMsCount + 12) > msCount)
	{
		return 1;
	}
	return 0;
}


/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels(vu16 * receiverChannel)
{

	u8 i;
	
	// OK Byte - test if uart3 interrupt is running
	// each 11ms one frame comes - 
	if ((oldSpektrumMsCount + 12) > msCount)
	{
		receiverChannel[RC_OK] = spektrumBytes[0];
	}
	// interrupt for uart3 is delay - no signal
	else
	{
		receiverChannel[RC_OK] = SPEKTRUM_NO;
	}
	
	// first 2 bytes are not used
	
	for (i = 2; i < 17; i++)
	{
		// first 2 byte from 2. frame also not used
		if (i != 9)
		{
			// shift first byte 8 steps to high and add low byte
			receiverSpektrumChannel[i] = (spektrumBytes[(i << 1) - 1] << 8) + spektrumBytes[i << 1]; 
			
			u8 channel = (receiverSpektrumChannel[i] & 0x3C00) >> 10; //bitmask 00111100 00000000 = channel number
			u16 value = (receiverSpektrumChannel[i] & 0x3FF); //bitmask 00000011 11111111 = channel value
			value = constrain(value + 989, 1000, 2000); // limit  + 1011 to center to ms
			value = smoothValue(value, oldReceiverSpektrumChannel[i], parameter[PARA_SMOOTH_RC]);
			oldReceiverSpektrumChannel[i] = value;
			
			// map channels
			switch (channel)
			{
				// Nick
				case 0:
					receiverChannel[3] = value;
					break;
				// Pitch
				case 1:
					receiverChannel[1] = value;
					break;
				// Roll
				case 2:
					receiverChannel[2] = value;
					break;
				// YAW
				case 3:
					receiverChannel[4] = value;
					break;
				// ch 5-8
				case 4:
				case 5:
				case 6:
				case 7:				
					receiverChannel[channel+1] = value;
					break;
				default:
					break;
			}
		}
	}
	
	
	//char x[100];
	//sprintf(x,"%d:%d-%d-%d-%d\r\n",receiverSpektrumChannel[12],receiverSpektrumChannel[13],receiverSpektrumChannel[14],receiverSpektrumChannel[15],receiverSpektrumChannel[16]);
	//print_uart1(x);

}





