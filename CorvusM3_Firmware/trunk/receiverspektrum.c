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
vu16 receiverSpektrumChannel[9];
extern vu16 receiverChannel[9];
extern vu8 RxBuffer3[0xFF];
extern vu8 RxInCounter3;
extern vu8 RxOutCounter3; 
extern vu32 uptimeMs; //to finde the sync gap all 11ms update from statemachine all 1ms

vu32 oldUptimeMs = 0;
vu8 byteCount = 0;
u8 channelCount = 0;

volatile union byteMap {
	u8 byte[2];
	u16 word;
} byteMapping;

/* map spektrum channels to channels if spektrum is used ----------------------------*/
void getSpektrumChannels()
{
	receiverChannel[0] = receiverSpektrumChannel[0];
	u8 i;
	for (i = 1; i < 9; i++)
	{
		// div values and set min max from 1000 to 2000
		receiverChannel[i] = receiverSpektrumChannel[i];  //calc the right values open!
	}
}


/* read receiverSpektrumChannels ----------------------------------------------------*/
/* used by uart3 RXNE interrupt -----------------------------------------------------*/
void getSpektrumChannels_IT()
{
	while (RxInCounter3 != RxOutCounter3)
	{
		// sync not OK - search sync gap		
		if (oldUptimeMs + SYNC_GAP < uptimeMs)  // found gap
		{
			receiverSpektrumChannel[0] = SPEKTRUM_OK;
			byteCount = 0;
		}
		
		// something wrong - no GAP found
		if (oldUptimeMs + SYNC_GAP > uptimeMs && byteCount == 0)
		{
			receiverSpektrumChannel[0] = SPEKTRUM_NO;
			byteCount = 0;
		}
		
		// if all is OK map channels
		if (receiverSpektrumChannel[0] == SPEKTRUM_OK && byteCount > 1)
		{
			if (byteCount%2 == 0)
			{
				byteMapping.byte[0] = RxBuffer3[RxOutCounter3];
			} 
			else
			{
				byteMapping.byte[1] = RxBuffer3[RxOutCounter3];
				receiverSpektrumChannel[channelCount++] = byteMapping.word;
			}
			
			byteCount++;
		}
		
		
		if (byteCount > 15)
		{
			byteCount = 0;
		}
		
		RxOutCounter3++;
		oldUptimeMs = uptimeMs;
		
	}
}




