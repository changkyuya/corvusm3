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

#include "filterhh.h"
#include "parameter.h"
#include "sensor.h"
#include <math.h>
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 gyroZero[3]; // sensor
extern vu16 parameter[0x190]; //parameter


/* init filter --------------------------------------------------------------*/
void initFilterHH(vs32 * gyroAngle, vs32 * copterAngle)
{
	zeroGyro();
	setAngleFilterHH(gyroAngle, copterAngle);
}


/* set gyro start angle -----------------------------------------------------*/
void setAngleFilterHH(vs32 * gyroAngle, vs32 * copterAngle)
{
	copterAngle[X] = gyroAngle[X] = 0;
	copterAngle[Y] = gyroAngle[Y] = 0;
	copterAngle[Z] = gyroAngle[Z] = 0;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n", gyroAngle[X], gyroAngle[Y], gyroAngle[Z]);
	print_uart1(x);
}




/* mix Gyro and ACC for Copter-Angel ----------------------------------------*/
void getCopterAnglesFilterHH(vs32 * gyroAngle, vs32 * copterAngle)
{
	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	getRawValues(gyroRawValues, accRawValues);
	
	//char x [80];
	//sprintf(x,"raw value:%d:%d\r\n",gyroRawValues[X],accRawValues[X]);
	//print_uart1(x);
	
	// get basis angles from sensors
	getGyroAnglesFilterHH(gyroAngle, gyroRawValues);
	//getACCAnglesFilterHH(accAngle, accRawValues);
		

	copterAngle[X] = gyroAngle[X];
	copterAngle[Y] = gyroAngle[Y];
	copterAngle[Z] = gyroAngle[Z];
	
}


/* calculate Gyro Angles ----------------------------------------------------*/
// 2mV/°/sec
// 12bit = 4095, Vref = 3,3V  => 3,3/4095 = 0,00080586085860
// ADC * 3,3 / 4095 / 2000 * 1000 
void getGyroAnglesFilterHH(vs32 * gyroAngle, vs32 * gyroRawValues)
{
	//gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100000);
    //gyroAngle[Y] -=y (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100000);
    //gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100000);
    gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100);
    gyroAngle[Y] -= (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100);
    gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100);
	
	//char x [80];
	//sprintf(x,"gyro raw value:%d:%d:%d\r\n",gyroAngle[X],gyroRawValues[X],gyroAngle[Z]);
	//print_uart1(x);
	// overrun
	u8 i;
	for (i = 0; i < 3; i++)
	{
		
		if (gyroAngle[i] >= 36000000) 
		{
			gyroAngle[i] -= 36000000;
		}
		if (gyroAngle[i] < 0) 
		{
			gyroAngle[i] = 36000000 - gyroAngle[Z];
		}
	}
}



/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterHH(vu16 * receiverChannel, vs32 * targetAngle)
{
	// only use yaw if stick is more than 5 points out of center 
	
	if (receiverChannel[ROLL] < 1495 || receiverChannel[ROLL] > 1505)
	{
		targetAngle[X] += (receiverChannel[ROLL] - 1500) * 50;
	}
	
	if (receiverChannel[NICK] < 1495 || receiverChannel[NICK] > 1505)
	{
		targetAngle[Y] += (receiverChannel[NICK] - 1500) * 50;
	}
	
	// only use yaw if stick is more than 5 points out of center and pitch not min
	if (receiverChannel[YAW] < 1495 || receiverChannel[YAW] > 1505)
	{
		if(receiverChannel[PITCH] > 1005)
		{
			targetAngle[Z] += (receiverChannel[YAW] - 1500) * 50;
		}
	}
	
	// overrun
	u8 i;
	for (i = 0; i < 3; i++)
	{
		
		if (targetAngle[i] >= 36000000) 
		{
			targetAngle[i] -= 36000000;
		}
		if (targetAngle[i] < 0) 
		{
			targetAngle[i] = 36000000 - targetAngle[Z];
		}
	}
}