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

#include "led.h" //test
/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 receiverChannel[9];
extern vu8 RxBuffer3[0xFF];
extern vu8 RxInCounter3;
extern vu8 RxOutCounter3; 

u8 sync = 0;
u8 channelCount = 0;

/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels()
{
	while (RxInCounter3 != RxOutCounter3)
	{
		//find sync
		if (RxBuffer3[RxOutCounter3] == 0x00)
		{
			//first sync byte
			sync = 1;
			channelCount = 0;
			receiverChannel[0] = SPEKTRUM_NO;
		}
		// second sync byte
		if (RxBuffer3[RxOutCounter3] == 0x03 && sync == 1)
		{
			sync = 0;
			receiverChannel[channelCount++] = SPEKTRUM_OK;
			*LED ^= 1;
		}
		
		// for next loop
		RxOutCounter3++;
		
	}
}




