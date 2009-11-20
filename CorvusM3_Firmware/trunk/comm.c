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
#include "eeprom.h"
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
extern u16 VirtAddVarTab[NumbOfVar]; //main


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
		case 's':
			dosComm();
			break;
		// set and store in FLASH
		case 'f':
			dofComm();
			break;
		// print parameter
		case 'p':
			dopComm();
			break;
		// read from flash parameter
		case 'r':
			dorComm();
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


/* do print Parameter -------------------------------------------------------------*/
void dopComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	if (set == 0)
	{
		print_para(set, parameter[PARA_SET]);
	}
	else
	{
		print_para(set, parameter[set]+parameter[PARA_SET]);
	}
	send(OK);
}


/* do read from flash and print Parameter -----------------------------------------*/
void dorComm()
{
	u16 val;
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	if (set == 0)
	{
		EE_ReadVariable(VirtAddVarTab[PARA_SET], &val);
		parameter[PARA_SET] = val;
		print_para(set, parameter[PARA_SET]);
	}
	else
	{
		EE_ReadVariable(VirtAddVarTab[set+parameter[PARA_SET]], &val);
		parameter[set+parameter[PARA_SET]] = val;
		print_para(set, parameter[set+parameter[PARA_SET]]);
	}
	send(OK);
}


/* do set Parameter -------------------------------------------------------------*/
void dosComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	if (set == 0)
	{
		parameter[PARA_SET] = readInt(4);
	}
	else
	{
		parameter[set+parameter[PARA_SET]] = readInt(4);
	}
	send(OK);
}


/* do store Parameter in virtual EEPROM -------------------------------------*/
void dofComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	if (set == 0)
	{
		// starte with position 4
		// s00:12345:
		parameter[PARA_SET] = readInt(4);
		EE_WriteVariable(VirtAddVarTab[PARA_SET], parameter[PARA_SET]);
	} 
	else 
	{
		// starte with position 4
		// s01:12345:
		parameter[set+parameter[PARA_SET]] = readInt(4);
		EE_WriteVariable(VirtAddVarTab[set+parameter[PARA_SET]], parameter[set+parameter[PARA_SET]]);
	}
	send(OK);
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


/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_para (u8 para, u16 value)
{
	char x [80];
	sprintf(x,"Para[%d]:%d\r\n",para, value);
	print_uart1(x);	
}




