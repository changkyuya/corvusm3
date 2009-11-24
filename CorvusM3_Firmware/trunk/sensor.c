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
//for debug
#include "serial.h"
#include <stdio.h>

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
vu16 gyroZero[3];


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
	sprintf(x,"G-Gyro-Zero:1:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}

/* get GyroValues - calculate Gyrovalues and Baise --------------------------*/
void getGyroValues(vs16 * gyroValues)
{
	gyroValues[X] = ADCSensorValue[GYRO_X] - gyroZero[X];
	gyroValues[Y] = ADCSensorValue[GYRO_Y] - gyroZero[Y];
	gyroValues[Z] = ADCSensorValue[GYRO_Z] - gyroZero[Z];
}
