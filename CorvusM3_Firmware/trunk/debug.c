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

#include "debug.h"
#include "serial.h"
#include <stdio.h>


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0xFF];  //statemachine
extern vu16 receiverChannel[9];  //statemachine

/* doDebug - send Debug infos over Serial -----------------------------------*/
void doDebug()
{
	char x [80];			
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	/*
	sprintf(x,"G-Gyro-ACC:1:%d:",ADCSensorValue[GYRO_X]);
	print_uart1(x);
	sprintf(x,"%d:",ADCSensorValue[GYRO_Y]);
	print_uart1(x);
	sprintf(x,"%d:",ADCSensorValue[GYRO_Z]);
	print_uart1(x);
	sprintf(x,"%d:",ADCSensorValue[ACC_X]);
	print_uart1(x);
	sprintf(x,"%d:",ADCSensorValue[ACC_Y]);
	print_uart1(x);
	sprintf(x,"%d:\r\n",ADCSensorValue[ACC_Z]);
	print_uart1(x);
	*/
	

	/* Debug output for receiver channels */
	// if parameter value is 1 - 1. bit is set
	if (parameter[PARA_DEBUG] & 0x01)
	{
		sprintf(x,"R-state:%d:",receiverChannel[0]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[1]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[2]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[3]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[4]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[5]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[6]);
		print_uart1(x);
		sprintf(x,"%d:",receiverChannel[7]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",receiverChannel[8]);
		print_uart1(x);	
	}
	
	/* toggle LED */
	//*LED ^= 1;
	//Pause(1000);
}