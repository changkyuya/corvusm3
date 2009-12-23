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
#include "parameter.h"
#include "sensor.h"
#include "filteracc.h"
#include <stdlib.h>
#include <stdio.h>

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0x190]; //parameter
extern u16 VirtAddVarTab[NumbOfVar]; //main
extern vs32 gyroAngle[3]; //statemachine
extern vs32 copterAngle[3]; //statemachine
vu16 receiverPCChannel[9];
extern vu32 msCount; //statemachine
vu32 msLastPCRC;


char line[80];
u8 l = 0;


/* getComm - get message from serial ----------------------------------------*/
void getComm()
{
	// read all incoming bytes
	while(is_read_uart1())
	{
		char byte = read_uart1();
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
		// command
		case 'c':
			docComm();
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
		// laod default parameter
		case '#':
			loadDefault();
			break;
		// flash all parameter
		case '*':
			flashAllParameter();
			break;
		// RC signal from Tool
		case '~':
			doRCComm();
			break;
		default:
			send(HELP);
	}
	
	// clear line
	l = 0;
	// send info to user
	//char message [] = "OK\r\n";
}

/* do Command Command -------------------------------------------------------*/
void docComm()
{
	switch (line[1])
	{
		case 'g':
			zeroGyro();
			setAngleFilterACC(gyroAngle, copterAngle);
			break;
		case 'a':
			zeroACC();
			break;
	}
}


/* do LED Command -----------------------------------------------------------*/
void dodComm()
{
	switch (line[1])
	{
		// all debug OFF
		case '0':
			setParameter(PARA_DEBUG, 0x00);
			send(OK);
			break;
		case 'r':
			// switch on
			//parameter[PARA_DEBUG] =| 0x01;
			// toggle
			setParameter(PARA_DEBUG, getParameter(PARA_DEBUG) ^ PARA_DEBUG_REC);
			send(OK);
			break;
		case 'a':
			// switch on
			//parameter[PARA_DEBUG] =| 0x02;
			// toggle
			setParameter(PARA_DEBUG, getParameter(PARA_DEBUG) ^ PARA_DEBUG_ADC);
			send(OK);
			break;
		case 's':
			// switch on
			//parameter[PARA_DEBUG] =| 0x02;
			// toggle
			setParameter(PARA_DEBUG, getParameter(PARA_DEBUG) ^ PARA_DEBUG_SENSOR);
			send(OK);
			break;
		case 'm':
			//toggle
			setParameter(PARA_DEBUG, getParameter(PARA_DEBUG) ^ PARA_DEBUG_MOT);
			send(OK);
			break;
		default:
			send(HELP);
	}
}


/* do print Parameter -------------------------------------------------------------*/
void dopComm()
{
	if (line[1] == 'a')
	{
		u8 i;
		for (i = 0;i <= USED_PARAMETER; i++)
		{
			print_para(i);
			Delay(5); // prevent txbuffer overvlow
		}
	}
	else
		{
		// add 2 chars to int from 0..99
		u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
		print_para(set);
		send(OK);
	}
}


/* do read from flash and print Parameter -----------------------------------------*/
void dorComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	print_para(set);
	send(OK);
}


/* do set Parameter -------------------------------------------------------------*/
void dosComm()
{
	if (line[1] == 'a')
	{
		zeroACC();
	}
	else
	{
		// add 2 chars to int from 0..99
		u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
		setParameter(set, readInt(4));
		print_para(set);
	}
}


/* do store Parameter in virtual EEPROM -------------------------------------*/
void dofComm()
{
	// add 2 chars to int from 0..99
	u8 set = (line[1] - 0x30) * 10 + (line[2] - 0x30);
	writeFlashParameter(set, readInt(4));
	print_para(set);
}


/* we get channels from PC tool ---------------------------------------------*/
void doRCComm()
{

	receiverPCChannel[0] = 1;
	receiverPCChannel[1] = readInt(1);
	receiverPCChannel[2] = readInt(6);
	receiverPCChannel[3] = readInt(11);
	receiverPCChannel[4] = readInt(16);
	
	msLastPCRC = msCount;
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
	if (infoText == ERROR)
	{
		print_uart1("ERROR\r\n");
	}
	else if (infoText == OK)
	{
		print_uart1("OK\r\n");
	}
	else if (infoText == DEFAULT)
	{
		print_uart1("Load default settings to flash\r\n");
	}
	else if (infoText == HELP)
	{
		print_uart1("Serial Commands:\r\n");
		Delay(20);
		print_uart1("ca ... Zero ACC\r\n");
		print_uart1("cg ... Zero Gyro\r\n");
		Delay(20);
		print_uart1("d0 ... Debug off\r\n");
		print_uart1("dr ... Toggle Receiver on/off\r\n");
		print_uart1("da ... Toggle Sensor RAW on/off\r\n");
		print_uart1("ds ... Toggle Sensor Data on/off\r\n");
		Delay(20);
		print_uart1("dm ... Toggle TargetAngle/Motors Data on/off\r\n");
		print_uart1("# ... Load default parameter to flash\r\n");
		print_uart1("* ... save all Parameter to flash\r\n");
		
		Delay(20);
		print_uart1("sa ... save ACC Zero\r\n");
		print_uart1("s00:0 ... Set parameterset 0 or 100(Set2)\r\n");
		print_uart1("s01:65535 ... Set parameter 01, 0 - 65535\r\n");
		print_uart1("f01:65535 ... Flash parameter 01, 0 - 65535\r\n");
		Delay(20);
		print_uart1("pa ... Print all Parameter\r\n");
		print_uart1("p01 ... Print parameter 01, 0 - 65535\r\n");
		print_uart1("r01 ... Read parameter from flash 01, 0 - 65535\r\n");
	}
}


/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_para (u8 para)
{
	char x [80];
	if (para == PARA_SET)
	{
		sprintf(x,"Para[%02d]:%d\r\n",para, parameter[PARA_SET]);
	}
	else
	{
		sprintf(x,"Para[%02d]:%d\r\n",para, parameter[para + parameter[PARA_SET]]);
	}
	print_uart1(x);	
}




