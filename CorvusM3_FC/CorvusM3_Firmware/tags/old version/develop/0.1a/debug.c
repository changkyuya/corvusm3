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

#include "debug.h"
#include "serial.h"
#include <stdio.h>
#include "parameter.h"


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0x190];  //statemachine
extern vu16 receiverChannel[9];  //statemachine
extern vu16 ADCSensorValue[7]; //initsystem
extern volatile float gyroAngle[3];  //statemachine
extern volatile float accAngle[2];  //statemachine
extern volatile float copterAngle[3];  //statemachine
extern volatile float targetAngle[3]; //statemachine
extern volatile char motor[5]; //statemachine

/* doDebug - send Debug infos over Serial -----------------------------------*/
void doDebug()
{
	char x [80];			
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_ADC)
	{
		sprintf(x,"Gyro-ACC:1:%d:",ADCSensorValue[GYRO_X]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[GYRO_Y]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[GYRO_Z]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[ACC_X]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[ACC_Y]);
		print_uart1(x);
		sprintf(x,"%d:",ADCSensorValue[ACC_Z]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",ADCSensorValue[VOLT]);
		print_uart1(x);
	}
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_SENSOR)
	{
		sprintf(x,"G-A-C:1:%-.2f:",gyroAngle[X]);
		print_uart1(x);
		sprintf(x,"%-.2f:",gyroAngle[Y]);
		print_uart1(x);
		sprintf(x,"%-.2f:",gyroAngle[Z]);
		print_uart1(x);
		sprintf(x,"%-.2f:",accAngle[X]);
		print_uart1(x);
		sprintf(x,"%-.2f:",accAngle[Y]);
		print_uart1(x);
		sprintf(x,"%-.2f:",copterAngle[X]);
		print_uart1(x);
		sprintf(x,"%-.2f:",copterAngle[Y]);
		print_uart1(x);
		sprintf(x,"%-.2f:\r\n",copterAngle[Z]);
		print_uart1(x);
	}
	

	/* Debug output for receiver channels */
	// if parameter value is 1 - 1. bit is set
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_REC)
	{
		sprintf(x,"RC-%d:%d:",getParameter(getParameter(PARA_HW) & 0x01),receiverChannel[0]);
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
	

	/* Debug output for receiver channels */
	// if parameter value is 1 - 1. bit is set
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_MOT)
	{
		sprintf(x,"G-TarAngle-Mot-1:%-.2f:",targetAngle[0]);
		print_uart1(x);
		sprintf(x,"%-.2f:",targetAngle[1]);
		print_uart1(x);
		sprintf(x,"%-.2f:",targetAngle[2]);
		print_uart1(x);
		sprintf(x,"%d:",motor[1]);
		print_uart1(x);
		sprintf(x,"%d:",motor[2]);
		print_uart1(x);
		sprintf(x,"%d:",motor[3]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",motor[4]);
		print_uart1(x);
	}
	
}