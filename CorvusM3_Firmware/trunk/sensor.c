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
vu32 overADCSensorValue[7];
vu32 oldGyroValues[3]; // for smooth
vu32 oldAccValues[3]; // for smooth
vu32 gyroZero[3];
vu32 accZero[3];
extern vu16 parameter[0x190]; //parameter


/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	oversamplingADC();
	
	gyroZero[X] = overADCSensorValue[GYRO_X];
	gyroZero[Y] = overADCSensorValue[GYRO_Y];
	gyroZero[Z] = overADCSensorValue[GYRO_Z];

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
	
	accZero[X] = accValues[X] / 100;
	accZero[Y] = accValues[Y] / 100;
	accZero[Z] = ((accValues[X] / 100) + (accValues[Y] / 100)) >> 1;
	
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
	
	oversamplingADC();
	
	//overADCSensorValue[0] = (ADCSensorValue[0]+ADCSensorValue[7]+ADCSensorValue[14]+ADCSensorValue[21]+ADCSensorValue[28]+ADCSensorValue[35]+ADCSensorValue[42]+ADCSensorValue[49])>>3;
	//char x [200];
	//sprintf(x,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",ADCSensorValue[0],ADCSensorValue[7],ADCSensorValue[14],ADCSensorValue[21],ADCSensorValue[28],ADCSensorValue[35],ADCSensorValue[42],ADCSensorValue[49]);
	//print_uart1(x);
	
	gyroValues[X] = smoothValue(overADCSensorValue[GYRO_X], oldGyroValues[X], parameter[PARA_SMOOTH_GYRO]) - gyroZero[X];
	gyroValues[Y] = smoothValue(overADCSensorValue[GYRO_Y], oldGyroValues[Y], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Y];
	gyroValues[Z] = smoothValue(overADCSensorValue[GYRO_Z], oldGyroValues[Z], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Z];
	
	accValues[X] = smoothValue(overADCSensorValue[ACC_X], oldAccValues[X], parameter[PARA_SMOOTH_ACC]);
	accValues[Y] = smoothValue(overADCSensorValue[ACC_Y], oldAccValues[Y], parameter[PARA_SMOOTH_ACC]);
	accValues[Z] = smoothValue(overADCSensorValue[ACC_Z], oldAccValues[Z], parameter[PARA_SMOOTH_ACC]);
	
	u8 i;
	for (i = 0; i < 3; i++)
	{
		oldGyroValues[i] = gyroValues[i];
		oldAccValues[i] = accValues[i];
	}
}


/* ADC oversampling ---------------------------------------------------------*/
void oversamplingADC()
{
	// 8 time oversampling
	u8 i;
	u8 j = 0;
	
	for (i = 0; i < 7; i++)
	{
		overADCSensorValue[i] = ADCSensorValue[i];
	}
	for (i = 7; i < 56; i++)
	{
		overADCSensorValue[j++] += ADCSensorValue[i];
		if (j > 6)
		{
			j = 0;
		}
	}
	for (i = 0; i < 7; i++)
	{
		overADCSensorValue[i] = (overADCSensorValue[i] * 1000 ) >> 3;
	}
}

