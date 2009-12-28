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
extern vu16 ADCSensorValue[56]; //initsystem
extern vu32 overADCSensorValue[7]; // sensor
extern vs32 gyroAngle[3];  //statemachine
extern vs32 accAngle[2];  //statemachine
extern vs32 copterAngle[3];  //statemachine
extern vs32 targetAngle[3]; //statemachine
extern volatile char motor[5]; //statemachine

/* doDebug - send Debug infos over Serial -----------------------------------*/
void doDebug()
{
	char x [200];			
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_ADC)
	{
		sprintf(x,"Gyro-ACC:1:%d:",overADCSensorValue[GYRO_X]);
		print_uart1(x);
		sprintf(x,"%d:",overADCSensorValue[GYRO_Y]);
		print_uart1(x);
		sprintf(x,"%d:",overADCSensorValue[GYRO_Z]);
		print_uart1(x);
		sprintf(x,"%d:",overADCSensorValue[ACC_X]);
		print_uart1(x);
		sprintf(x,"%d:",overADCSensorValue[ACC_Y]);
		print_uart1(x);
		sprintf(x,"%d:",overADCSensorValue[ACC_Z]);
		print_uart1(x);
		sprintf(x,"%d:\r\n",ADCSensorValue[VOLT]);
		print_uart1(x);
		//sprintf(x,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",ADCSensorValue[0],ADCSensorValue[7],ADCSensorValue[14],ADCSensorValue[21],ADCSensorValue[28],ADCSensorValue[35],ADCSensorValue[42],ADCSensorValue[49]);
		//print_uart1(x);
	}
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_SENSOR)
	{
		sprintf(x,"G-A-C:1:%d:",gyroAngle[X]/1000);
		print_uart1(x);
		sprintf(x,"%d:",gyroAngle[Y]/1000);
		print_uart1(x);
		sprintf(x,"%d:",gyroAngle[Z]/1000);
		print_uart1(x);
		sprintf(x,"%d:",accAngle[X]/1000);
		print_uart1(x);
		sprintf(x,"%d:",accAngle[Y]/1000);
		print_uart1(x);
		sprintf(x,"%d:",copterAngle[X]/1000);
		print_uart1(x);
		sprintf(x,"%d:",copterAngle[Y]/1000);
		print_uart1(x);
		sprintf(x,"%d:\r\n",copterAngle[Z]/1000);
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
		sprintf(x,"G-TarAngle-Mot-1:%d:",targetAngle[0]/1000);
		print_uart1(x);
		sprintf(x,"%d:",targetAngle[1]/1000);
		print_uart1(x);
		sprintf(x,"%d:",targetAngle[2]/1000);
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