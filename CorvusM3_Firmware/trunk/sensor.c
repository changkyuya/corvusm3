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
#include <math.h>


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
vu16 oldADCSensorValue[7];
vu16 gyroZero[3];
vu16 accZero[3];
extern vu16 parameter[0xFF]; //parameter


/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	u8 i;
	gyroZero[X] = ADCSensorValue[GYRO_X];
	gyroZero[Y] = ADCSensorValue[GYRO_Y];
	gyroZero[Z] = ADCSensorValue[GYRO_Z];
	Pause(100);
	for (i = 0; i < 5; i++)
	{
		gyroZero[X] = (gyroZero[X] + ADCSensorValue[GYRO_X]) >> 1;
		gyroZero[Y] = (gyroZero[Y] + ADCSensorValue[GYRO_Y]) >> 1;
		gyroZero[Z] = (gyroZero[Z] + ADCSensorValue[GYRO_Z]) >> 1;
		Pause(100);
	}
	
	char x [80];
	sprintf(x,"Gyro-Zero:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}

/* Set ACC Values to Zero ---------------------------------------------------*/
void zeroACC()
{
	accZero[X] = ADCSensorValue[ACC_X];
	accZero[Y] = ADCSensorValue[ACC_Y];
	accZero[Z] = ADCSensorValue[ACC_Z];
	
	setParameter(PARA_ACC_X_ZERO, accZero[X]);
	setParameter(PARA_ACC_Y_ZERO, accZero[Y]);
	setParameter(PARA_ACC_Z_ZERO, accZero[Z]);
	
	char x [80];
	sprintf(x,"ACC-Zero:%d:%d:%d:\r\n",accZero[X],accZero[Y],accZero[Z]);
	print_uart1(x);
}


/* get GyroValues - calculate Gyrovalues and Baise --------------------------*/
void getGyroValues(vs16 * gyroValues)
{
	s8 i;
	
	gyroValues[X] = ADCSensorValue[GYRO_X] - gyroZero[X];
	gyroValues[Y] = ADCSensorValue[GYRO_Y] - gyroZero[Y];
	gyroValues[Z] = ADCSensorValue[GYRO_Z] - gyroZero[Z];
	
	
	
	// save values for smooth
	for (i = 0; i < 3; i++)
	{
		oldADCSensorValue[i] = ADCSensorValue[i];
	}
}

/* calculate ACC Angles -----------------------------------------------------*/
void getACCAngles (vs16 * accAngle)
{
	u8 i; 
	// x = (x - corrACC) * factorACC * 180 / PI
	// z = (z - corrACC) * factorACC * 180 / PI
	// 180 / PI = 57.2957795
	// minus 90 grad für level
	//ACCAngle[X] = atan2((ACCRaw[Z] + corrACC[X]) * factorACC[X], (ACCRaw[X] + corrACC[X]) * factorACC[X]) * 57.2957795 + 90;
	
	accAngle[X] = fastatan2(ADCSensorValue[ACC_Z] - parameter[PARA_ACC_X_ZERO] , ADCSensorValue[ACC_X] - parameter[PARA_ACC_X_ZERO] ) * 57.2957795 * 100.0;
	accAngle[Y] = atan2(ADCSensorValue[ACC_Z] - parameter[PARA_ACC_Y_ZERO] , ADCSensorValue[ACC_Y] - parameter[PARA_ACC_Y_ZERO] ) * 57.2957795 * 100.0;

	
	//char x [80];
	//sprintf(x,"test:%d:%d:\r\n",accAngle[X],accAngle[Y]);
	//print_uart1(x);
	
	// save values for smooth
	for (i = 3; i < 5; i++)
	{
		oldADCSensorValue[i] = ADCSensorValue[i];
	}
}
