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

#include "comm.h"
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu8 TxBuffer1[0xFF]; //serial
extern vu8 TxInCounter1; //serial
extern vu8 TxOutCounter1; //serial
extern vu8 RxBuffer1[0xFF]; //serial
extern vu8 RxOutCounter1; //serial
extern vu8 RxInCounter1; //serial
extern vu16 parameter[0xFF]; //statemachine


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
			l = 0;
		}
		// don't use \r
		else// if (byte != '\r') // use \r as limitter
		{
			line[l++] = byte;
		}
		
	}
}

/* doComm - if one line is complete do Command ------------------------------*/
void doComm()
{
	// do byte commands first char
	switch (line[0])
	{
		// switch LED
		case 'l':
			dolComm();
			break;
		// debug level
		case 'd':
			dodComm();
			break;
		// set parameter
		case 'p':
			dopComm();
			break;
		default:
			send(ERROR);
	}
	
	// clear line
	l = 0;
	// send info to user
	//char message [] = "OK\r\n";
}


/* do LED Command -----------------------------------------------------------*/
void dolComm()
{
	switch (line[1])
	{
		case '0':
			setLEDStatus(LED_OFF);
			send(OK);
			break;
		case '1':
			setLEDStatus(LED_FLASH);
			send(OK);
			break;
		case '2':
			setLEDStatus(LED_BLINK);
			send(OK);
			break;
		case '3':
			setLEDStatus(LED_ON);
			send(OK);
			break;
		default:
			send(ERROR);
	}
}


/* do LED Command -----------------------------------------------------------*/
void dodComm()
{
	switch (line[1])
	{
		// all debug OFF
		case '0':
			parameter[PARA_DEBUG] = 0x00;
			send(OK);
			break;
		case 'r':
			// switch on
			//parameter[PARA_DEBUG] =| 0x01;
			// toggle
			parameter[PARA_DEBUG] = parameter[PARA_DEBUG] ^ 0x01;
			send(OK);
			break;
		case 's':
			// switch on
			//parameter[PARA_DEBUG] =| 0x02;
			// toggle
			parameter[PARA_DEBUG] = parameter[PARA_DEBUG] ^ 0x02;
			send(OK);
			break;
		default:
			send(ERROR);
	}
}


/* do Parameter -------------------------------------------------------------*/
void dopComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	switch (set)
	{
		// set start parameter set 1/2
		case 00:
			// starte with position 4
			// s00:12345:
			parameter[PARA_SET] = readInt(4);
			send(OK);
			break;
		case 01:
			// starte with position 4
			// s01:12345:
			parameter[PARA_DEBUG] = readInt(4);
			send(OK);
			break;
		default:
			send(ERROR);
	}
}



/* read ineger from byte array ----------------------------------------------*/
u16 readInt(u8 start) {
	char value[5];
	u8 count = 0;  // arraycount
	// read int to : or end \r
	while (line[start] != ':' && line[start] != '\r' && count <5 ) 
	{	
		value[count++] = line[start++];
	} 
	// Array to Int, atof Array to float
	return atoi(value);  
}


/* send Error/OK to User ----------------------------------------------------*/
void send(u8 infoText)
{
	if (infoText == 0)
	{
		char message[] = "ERROR\r\n";
		print_uart1(message);
	}
	else if (infoText == 1)
	{
		char message[] = "OK\r\n";
		print_uart1(message);
	}
}




