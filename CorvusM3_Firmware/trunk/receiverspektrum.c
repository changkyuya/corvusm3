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
vu8 syncByte[2];
vu8 syncCount = 0;
vu8 byteCount = 0;
vu8 channelByteCount = 0;

u8 channelCount = 0;

volatile union byteMap {
	u8 byte[2];
	u16 word;
} byteMapping;


/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels()
{
	while (RxInCounter3 != RxOutCounter3)
	{
		// sync not OK - search sync gap
		if (receiverChannel[0] == SPEKTRUM_NO)
		{
			if (RxBuffer3[RxOutCounter3] == 0x00 && syncCount == 0)
			{
				syncCount++;
				syncByte[0] = [RxOutCounter3];
			}
			if (RxBuffer3[RxOutCounter3] != 0x00 && syncCount == 1)
			{
				syncCount++;
				syncByte[1] = [RxOutCounter3];
			}
			if (RxBuffer3[RxOutCounter3] == syncByte[0] && syncCount == 2)
			{
				syncCount++;
			}
			if (RxBuffer3[RxOutCounter3] == syncByte[1] && syncCount == 3)
			{
				syncCount++;
				byteCount = 1;
				receiverChannel[0] == SPEKTRUM_OK;
			}
		}
		
		// test sync all 16byte
		if (byteCount == 0 && RxBuffer3[RxOutCounter3] != syncByte[0] && receiverChannel[0] == SPEKTRUM_OK ||
			byteCount == 1 && RxBuffer3[RxOutCounter3] != syncByte[1] && receiverChannel[0] == SPEKTRUM_OK)
		{
			receiverChannel[0] == SPEKTRUM_NO;
			syncCount = 0;
		}
		
		// if all is OK map channels
		if (receiverChannel[0] == SPEKTRUM_OK && byteCount > 1)
		{
			if (channelByteCount == 0)
			{
				channelByteCount++;
				byteMapping.byte[0] = RxBuffer3[RxOutCounter3];
			}
			if (channelByteCount == 1)
			{
				channelByteCount = 0;
				byteMapping.byte[1] = RxBuffer3[RxOutCounter3];
				receiverChannel[channelCount++] = byteMapping.word;
			}
		}
		
		
		byteCount++;
		if (byteCount > 15)
		{
			byteCount = 0;
		}
		
		RxOutCounter3++;
		
	}
}




