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

#include "filtercomp2.h"
#include "parameter.h"
#include "sensor.h"
#include <math.h>
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 gyroZero[3]; // sensor
extern vu16 parameter[0x190]; //parameter


/* calculate ACC Angles -----------------------------------------------------*/
void getACCAnglesFilterComp2(vs32 * accAngle)
{
	vs16 accRawValues[3];
	getACCRawValues(accRawValues);
	// x = (x - corrACC) * factorACC * 180 / PI
	// z = (z - corrACC) * factorACC * 180 / PI
	// 180 / PI = 57.2957795
	// minus 90 grad f�r level
	//ACCAngle[X] = atan2((ACCRaw[Z] + corrACC[X]) * factorACC[X], (ACCRaw[X] + corrACC[X]) * factorACC[X]) * 57.2957795 + 90;
	
	// atan2 works - if it is to slow we can use fastatan2
	//accAngle[X] = fastatan2(ADCSensorValue[ACC_Z] - parameter[PARA_ACC_X_ZERO] , ADCSensorValue[ACC_X] - parameter[PARA_ACC_X_ZERO] ) * 57.2957795 * 100.0;
	accAngle[X] = (vs32) (atan2(accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] , accRawValues[X] - parameter[PARA_ACC_X_ZERO] ) * 5729577.95);
	//change direction
	accAngle[Y] = (vs32) (atan2(accRawValues[Y] - parameter[PARA_ACC_Y_ZERO] , accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] ) * 5729577.95 + 9000000);

	
	//char x [80];
	//sprintf(x,"test:%d:%d:\r\n",accAngle[X],accAngle[Y]);
	//print_uart1(x);
	
}

/* set gyro start angle -----------------------------------------------------*/
void setGyroAngleFilterComp2(vs32 * gyroAngle)
{
	vs32 accAngle[2];
	getACCAnglesFilterComp2(accAngle);
	gyroAngle[X] = accAngle[X];
	gyroAngle[Y] = accAngle[Y];
	gyroAngle[Z] = 18000000;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n", gyroAngle[X], gyroAngle[Y], gyroAngle[Z]);
	print_uart1(x);
}

/* calculate Gyro Angles ----------------------------------------------------*/
// 2mV/�/sec
// 12bit = 4095, Vref = 3,3V  => 3,3/4095 = 0,00080586085860
// ADC * 3,3 / 4095 / 2000 * 1000 
void getGyroAnglesFilterComp2(vs32 * gyroAngle)
{
	vs16 gyroRawValues[3];
	getGyroRawValues(gyroRawValues);
	//gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100000);
    //gyroAngle[Y] -=y (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100000);
    //gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100000);
    gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100000);
    gyroAngle[Y] -= (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100000);
    gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100000);
	
		
	if (gyroAngle[Z] >= 36000000) 
	{
		gyroAngle[Z] -= 36000000;
	}
	if (gyroAngle[Z] < 0) 
	{
		gyroAngle[Z] = 36000000 - gyroAngle[Z];
	}
	
	//char x [80];
	//sprintf(x,"gyro raw value:%d:%d:%d\r\n",gyroAngle[X],gyroRawValues[X],gyroAngle[Z]);
	//print_uart1(x);
}


/* mix Gyro and ACC for Copter-Angel ----------------------------------------*/
void getCopterAnglesFilterComp2(vs32 * gyroAngle, vs32 * accAngle, vs32 * copterAngle)
{
	//needs about 50us (all 5)
	copterAngle[X] = weightingValues(accAngle[X], gyroAngle[X], parameter[PARA_ACC_FORCE]); 
	copterAngle[Y] = weightingValues(accAngle[Y], gyroAngle[Y], parameter[PARA_ACC_FORCE]); 
	copterAngle[Z] = gyroAngle[Z];
	
	// trimm gyro to new Angle
	gyroAngle[X] = weightingValues(copterAngle[X], gyroAngle[X], parameter[PARA_GYRO_CORR]); 
	gyroAngle[Y] = weightingValues(copterAngle[Y], gyroAngle[Y], parameter[PARA_GYRO_CORR]); 
}


/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterComp2(vu16 * receiverChannel, vs32 * targetAngle, vs32 * copterAngle)
{
	// 90 = neutral
	// max is 20 to 160� - this are 70� for 500 points
	targetAngle[X] = 2000000 + ((14000) * (receiverChannel[ROLL] - 1000));
	targetAngle[Y] = 2000000 + ((14000) * (receiverChannel[NICK] - 1000));
	targetAngle[Z] = copterAngle[Z] + (receiverChannel[YAW] - 1500) * 100000;
	
	if (targetAngle[Z] >= 36000000) 
	{
		targetAngle[Z] -= 36000000;
	}
	if (targetAngle[Z] < 0) 
	{
		targetAngle[Z] = 36000000 - targetAngle[Z];
	}
}