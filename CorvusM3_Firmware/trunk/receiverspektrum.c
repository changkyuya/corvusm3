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

#include "serial.h" //test
#include <stdio.h> //test 

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 msCount;
vu32 oldSpektrumMsCount = 0;

//test
extern vu8 TxBuffer1[0xFF];
extern vu8 TxInCounter1;
extern vu8 TxOutCounter1;
extern vu8 RxBuffer1[0xFF];
extern vu8 RxOutCounter1;
extern vu8 RxInCounter1;

extern vu8 TxBuffer3[0xFF];
extern vu8 TxInCounter3;
extern vu8 TxOutCounter3;
extern vu8 RxBuffer3[0xFF];
extern vu8 RxOutCounter3;
extern vu8 RxInCounter3;



volatile union byteMap {
	u8 byte[2];
	u16 word;
} byteMapping;


/* read receiverChannels ----------------------------------------------------*/
void getSpektrumChannels()
{
	
}

/* read receiverChannels Interrupt from UART3 -------------------------------*/
void getSpektrumChannels_IT()
{
	if (oldSpektrumMsCount + 7 < msCount)
	{
			TxBuffer1[TxInCounter1++] = '*';
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
	
	oldSpektrumMsCount = msCount;
}




