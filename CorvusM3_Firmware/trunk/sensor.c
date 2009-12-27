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
extern vu16 ADCSensorValue[56];  //initsystem
vu32 oldGyroValues[3]; // for smooth
vu32 oldAccValues[3]; // for smooth
vu32 gyroZero[3];
vu32 accZero[3];
extern vu16 parameter[0x190]; //parameter


/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	vs32 gyroValues[3];
	vs32 accValues[3];
	getRawValues(gyroValues, accValues);
	
	gyroZero[X] = gyroValues[X];
	gyroZero[Y] = gyroValues[Y];
	gyroZero[Z] = gyroValues[Z];
	
	u8 i;
	for (i = 0; i < 7; i++)
	{
		gyroZero[X] += gyroValues[X];
		gyroZero[Y] += gyroValues[Y];
		gyroZero[Z] += gyroValues[Z];
		Pause(100);
	}
	
	gyroZero[X] = gyroZero[X] >> 3; 
	gyroZero[Y] = gyroZero[Y] >> 3;
	gyroZero[Z] = gyroZero[Z] >> 3;

	char x [80];
	sprintf(x,"Gyro-Zero:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}




/* Set ACC Values to Zero ---------------------------------------------------*/
void zeroACC()
{
	vs32 gyroValues[3];
	vs32 accValues[3];
	getRawValues(gyroValues, accValues);
	
	accZero[X] = accValues[X];
	accZero[Y] = accValues[Y];
	accZero[Z] = (accValues[X] + accValues[Y]) >> 1;
	
	setParameter(PARA_ACC_X_ZERO, accZero[X]);
	setParameter(PARA_ACC_Y_ZERO, accZero[Y]);
	setParameter(PARA_ACC_Z_ZERO, accZero[Z]);
	
	char x [80];
	sprintf(x,"ACC-Zero:%d:%d:%d:\r\n",accZero[X],accZero[Y],accZero[Z]);
	print_uart1(x);
}


/* get RawValues - calculate Gyrovalues and Baise --------------------------*/
void getRawValues(vs32 * gyroValues, vs32 * accValues)
{

	gyroValues[X] = smoothValue(ADCSensorValue[GYRO_X] * 1000, oldGyroValues[X], parameter[PARA_SMOOTH_GYRO]) - gyroZero[X];
	gyroValues[Y] = smoothValue(ADCSensorValue[GYRO_Y] * 1000, oldGyroValues[Y], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Y];
	gyroValues[Z] = smoothValue(ADCSensorValue[GYRO_Z] * 1000, oldGyroValues[Z], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Z];
	
	accValues[X] = smoothValue(ADCSensorValue[ACC_X] * 1000, oldAccValues[X], parameter[PARA_SMOOTH_ACC]);
	accValues[Y] = smoothValue(ADCSensorValue[ACC_Y] * 1000, oldAccValues[Y], parameter[PARA_SMOOTH_ACC]);
	accValues[Z] = smoothValue(ADCSensorValue[ACC_Z] * 1000, oldAccValues[Z], parameter[PARA_SMOOTH_ACC]);
	
	u8 i;
	for (i = 0; i < 3; i++)
	{
		oldGyroValues[i] = gyroValues[i];
		oldAccValues[i] = accValues[i];
	}
}


