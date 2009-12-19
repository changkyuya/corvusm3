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

#include "sensor.h"
#include "parameter.h"
//for debug
#include "serial.h"
#include <stdio.h>


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
vu16 oldADCSensorValue[7]; // for smooth
vu16 gyroZero[3];
vu16 accZero[3];
extern vu16 parameter[0x190]; //parameter


/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	gyroZero[X] = ADCSensorValue[GYRO_X];
	gyroZero[Y] = ADCSensorValue[GYRO_Y];
	gyroZero[Z] = ADCSensorValue[GYRO_Z];
	
	u8 i;
	for (i = 0; i < 7; i++)
	{
		gyroZero[X] += ADCSensorValue[GYRO_X];
		gyroZero[Y] += ADCSensorValue[GYRO_Y];
		gyroZero[Z] += ADCSensorValue[GYRO_Z];
		Pause(100);
	}
	
	gyroZero[X] = gyroZero[X] >> 3; 
	gyroZero[Y] = gyroZero[Y] >> 3;
	gyroZero[Z] = gyroZero[Z] >> 3;
	
	for (i = 0; i < 7; i++)
	{
		gyroZero[X] += ADCSensorValue[GYRO_X];
		gyroZero[Y] += ADCSensorValue[GYRO_Y];
		gyroZero[Z] += ADCSensorValue[GYRO_Z];
		Pause(100);
	}
	
	gyroZero[X] = gyroZero[X] >> 3; 
	gyroZero[Y] = gyroZero[Y] >> 3;
	gyroZero[Z] = gyroZero[Z] >> 3;
	
	setOldADC();

	char x [80];
	sprintf(x,"Gyro-Zero:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}


/* set old ADV Values to actual Values --------------------------------------*/
void setOldADC()
{
	u8 i;
	for (i = 0; i < 7; i++)
	{
		oldADCSensorValue[i] = ADCSensorValue[i];
	}
}


/* Set ACC Values to Zero ---------------------------------------------------*/
void zeroACC()
{
	accZero[X] = ADCSensorValue[ACC_X];
	accZero[Y] = ADCSensorValue[ACC_Y];
	accZero[Z] = (ADCSensorValue[ACC_X] + ADCSensorValue[ACC_Y]) >> 1;
	
	setParameter(PARA_ACC_X_ZERO, accZero[X]);
	setParameter(PARA_ACC_Y_ZERO, accZero[Y]);
	setParameter(PARA_ACC_Z_ZERO, accZero[Z]);
	
	char x [80];
	sprintf(x,"ACC-Zero:%d:%d:%d:\r\n",accZero[X],accZero[Y],accZero[Z]);
	print_uart1(x);
}


/* get GyroRawValues - calculate Gyrovalues and Baise --------------------------*/
void getGyroRawValues(vs16 * gyroValues)
{

	gyroValues[X] = smoothValue(ADCSensorValue[GYRO_X], oldADCSensorValue[GYRO_X], parameter[PARA_SMOOTH_GYRO]) - gyroZero[X];
	gyroValues[Y] = smoothValue(ADCSensorValue[GYRO_Y], oldADCSensorValue[GYRO_Y], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Y];
	gyroValues[Z] = smoothValue(ADCSensorValue[GYRO_Z], oldADCSensorValue[GYRO_Z], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Z];
	
	u8 i;
	for (i = 0; i < 3; i++)
	{
		oldADCSensorValue[i] = ADCSensorValue[i];
	}
}


/* get ACCRawValues ---------------------------------------------------------*/
void getACCRawValues(vs16 * accValues)
{
	accValues[X] = smoothValue(ADCSensorValue[ACC_X], oldADCSensorValue[ACC_X], parameter[PARA_SMOOTH_ACC]);
	accValues[Y] = smoothValue(ADCSensorValue[ACC_Y], oldADCSensorValue[ACC_Y], parameter[PARA_SMOOTH_ACC]);
	accValues[Z] = smoothValue(ADCSensorValue[ACC_Z], oldADCSensorValue[ACC_Z], parameter[PARA_SMOOTH_ACC]);
	
	u8 i;
	for (i = 3; i < 6; i++)
	{
		oldADCSensorValue[i] = ADCSensorValue[i];
	}
}

