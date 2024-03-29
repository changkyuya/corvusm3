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
extern vu32 gyroZero[3]; // sensor
extern vu16 parameter[0x190]; //parameter
vu32 y[2]; // for comp2 filter
extern vs32 compassAngle; //statemachine
extern vs32 targetAngle[3]; //statemachine



/* init filter --------------------------------------------------------------*/
void initFilterComp2(vs32 * gyroAngle, vs32 * copterAngle)
{
	
	// setup compass
	initCompass();
	
	zeroGyro();
	setAngleFilterComp2(gyroAngle, copterAngle);
}



/* set gyro start angle -----------------------------------------------------*/
void setAngleFilterComp2(vs32 * gyroAngle, vs32 * copterAngle)
{
	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	getRawValues(gyroRawValues, accRawValues);
	
	vs32 accAngle[2];
	getACCAnglesFilterComp2(accAngle, accRawValues);
	copterAngle[X] = gyroAngle[X] = accAngle[X];
	copterAngle[Y] = gyroAngle[Y] = accAngle[Y];
	copterAngle[Z] = gyroAngle[Z] = targetAngle[Z] = 0;
	//init y1 for filter
	y[X] = 0;
	y[Y] = 0;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n", gyroAngle[X], gyroAngle[Y], gyroAngle[Z]);
	print_uart1(x);
}


/* mix Gyro and ACC for Copter-Angel ----------------------------------------*/
void getCopterAnglesFilterComp2(vs32 * gyroAngle, vs32 * accAngle, vs32 * copterAngle)
{
	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	getRawValues(gyroRawValues, accRawValues);
	//getCompassAngle();
	
	getGyroAnglesFilterComp2(gyroAngle, gyroRawValues);
	getACCAnglesFilterComp2(accAngle, accRawValues);
	
	// Written by RoyLB at:
	// http://www.rcgroups.com/forums/showpost.php?p=12082524&postcount=1286
  
	s32 x1;
	s32 x2;
	// mapped to 1kHz loop
	x1 = ((accAngle[X] - copterAngle[X]) * parameter[PARA_ACC_FORCE] * parameter[PARA_ACC_FORCE] / 1000000) ;
	y[X] += x1 / 1000;
	x2 = y[X] + ((accAngle[X] - copterAngle[X]) * 2 * parameter[PARA_ACC_FORCE]) / 1000 + gyroAngle[X];
	copterAngle[X] += x2 / 1000;
	
	//char x [80];
	//sprintf(x,"dbg:%d:%d:%d\r\n", x1, y[X], x2);
	//print_uart1(x);
	
	x1 = ((accAngle[Y] - copterAngle[Y]) * parameter[PARA_ACC_FORCE] * parameter[PARA_ACC_FORCE] / 1000000) ;
	y[Y] += x1 / 1000;
	x2 = y[Y] + ((accAngle[Y] - copterAngle[Y]) * 2 * parameter[PARA_ACC_FORCE]) / 1000 + gyroAngle[Y];
	copterAngle[Y] += x2 / 1000;
	
	copterAngle[Z] = gyroAngle[Z];
	
	// trimm gyro to new Angle
	//gyroAngle[X] = weightingValues(copterAngle[X], gyroAngle[X], parameter[PARA_GYRO_CORR]); 
	//gyroAngle[Y] = weightingValues(copterAngle[Y], gyroAngle[Y], parameter[PARA_GYRO_CORR]); 
}


/* calculate Gyro Angles ----------------------------------------------------*/
// 2mV/�/sec
// 12bit = 4095, Vref = 3,3V  => 3,3/4095 = 0,00080586085860
// ADC * 3,3 / 4095 / 2000 * 1000 
void getGyroAnglesFilterComp2(vs32 * gyroAngle, vs32 * gyroRawValues)
{
	
	// calculate the angles for X and Y in getCopterAngelsFilterComp2
	gyroAngle[X] = (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100000);
    gyroAngle[Y] = (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100000);
    gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100);
	
		
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


/* calculate ACC Angles -----------------------------------------------------*/
void getACCAnglesFilterComp2(vs32 * accAngle, vs32 * accRawValues)
{
	// x = (x - corrACC) * factorACC * 180 / PI
	// z = (z - corrACC) * factorACC * 180 / PI
	// 180 / PI = 57.2957795
	// minus 90 grad f�r level
	//ACCAngle[X] = atan2((ACCRaw[Z] + corrACC[X]) * factorACC[X], (ACCRaw[X] + corrACC[X]) * factorACC[X]) * 57.2957795 + 90;
	
	// atan2 works - if it is to slow we can use fastatan2
	//accAngle[X] = fastatan2(ADCSensorValue[ACC_Z] - parameter[PARA_ACC_X_ZERO] , ADCSensorValue[ACC_X] - parameter[PARA_ACC_X_ZERO] ) * 57.2957795 * 100.0;
	accAngle[X] = (vs32) (atan2(accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] * 100 , accRawValues[X] - parameter[PARA_ACC_X_ZERO] * 100 ) * 5729577.95);
	//change direction
	accAngle[Y] = (vs32) (atan2(accRawValues[Y] - parameter[PARA_ACC_Y_ZERO] * 100 , accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] * 100 ) * 5729577.95 + 9000000);
	
	
	//char x [80];
	//sprintf(x,"test:%d:%d:\r\n",accAngle[X],accAngle[Y]);
	//print_uart1(x);
	
}


/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterComp2(vu16 * receiverChannel)
{
	// 90 = neutral
	// max is 20 to 160� - this are 70� for 500 points
	targetAngle[X] = 2000000 + ((14000) * (receiverChannel[ROLL] - 1000));
	targetAngle[Y] = 2000000 + ((14000) * (receiverChannel[NICK] - 1000));
	// only use yaw if stick is more than 5 points out of center and pitch not min
	if (receiverChannel[YAW] < 1495 || receiverChannel[YAW] > 1505)
	{
		if(receiverChannel[PITCH] > 1005)
		{
			targetAngle[Z] += (receiverChannel[YAW] - 1500) * parameter[PARA_STICK_FACTOR];
		}
	}
	
	if (targetAngle[Z] >= 36000000) 
	{
		targetAngle[Z] -= 36000000;
	}
	if (targetAngle[Z] < 0) 
	{
		targetAngle[Z] = 36000000 - targetAngle[Z];
	}
}