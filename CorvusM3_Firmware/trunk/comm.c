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

#include "comm.h"
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu8 TxBuffer1[0xFF];
extern vu8 TxInCounter1;
extern vu8 TxOutCounter1;
extern vu8 RxBuffer1[0xFF];
extern vu8 RxOutCounter1;
extern vu8 RxInCounter1;

char line[80];
u8 l = 0;


/* getComm - get message from serial ----------------------------------------*/
void getComm()
{
	// read all incoming bytes
	while(RxInCounter1 != RxOutCounter1)
	{
		char byte = RxBuffer1[RxOutCounter1++];
		// if one line is complete
		if (byte == '\n')
		{
			doComm();
		}
		// don't use \r
		else if (byte != '\r')
		{
			line[l++] = byte;
		}
		
	}
}

/* doComm - if one line is complete do Command ------------------------------*/
void doComm()
{
	// do one byte commands
	switch (line[0])
	{
		case '0':
			setLEDStatus(LED_OFF);
			break;
		case '1':
			setLEDStatus(LED_FLASH);
			break;
		case '2':
			setLEDStatus(LED_BLINK);
			break;
		case '3':
			setLEDStatus(LED_ON);
			break;
	}
	
	// clear line
	l = 0;
	// send info to user
	char message [] = "OK\r\n";
	print_uart1(message);
}










