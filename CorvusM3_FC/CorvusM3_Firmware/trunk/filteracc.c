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

#include "filteracc.h"
#include "parameter.h"
#include "sensor.h"
#include <math.h>
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu32 gyroZero[3]; // sensor
extern vu16 parameter[0x190]; //parameter
extern vs32 compassAngle; //statemachine
extern vs32 targetAngle[3]; //statemachine
extern vs32 gyroRawValues1[3]; //filter HH
extern vs32 gyroRawValues2[3]; //filter HH
extern vs32 gyroRawValues3[3]; //filter HH


/* init filter --------------------------------------------------------------*/
void initFilterACC(vs32 * gyroAngle, vs32 * copterAngle)
{
	
	// setup compass only if HW_Setup is 4
	if (getParameter(PARA_HW) & PARA_HW_COMP)
	{
		initCompass();
	}
	zeroGyro();
	setAngleFilterACC(gyroAngle, copterAngle);
}


/* set gyro start angle -----------------------------------------------------*/
void setAngleFilterACC(vs32 * gyroAngle, vs32 * copterAngle)
{
	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	vs32 accAngle[2];
		
	getRawValues(gyroRawValues, accRawValues);
	getACCAnglesFilterACC(accAngle, accRawValues);
	
	copterAngle[X] = gyroAngle[X] = accAngle[X];
	copterAngle[Y] = gyroAngle[Y] = accAngle[Y];
	copterAngle[Z] = gyroAngle[Z] = targetAngle[Z] = 0;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n", gyroAngle[X], gyroAngle[Y], gyroAngle[Z]);
	print_uart1(x);
}




/* mix Gyro and ACC for Copter-Angel ----------------------------------------*/
void getCopterAnglesFilterACC(vs32 * gyroAngle, vs32 * accAngle, vs32 * copterAngle)
{

	vs32 accRawValues[3];
	vs32 gyroRawValues[3];
	getRawValues(gyroRawValues, accRawValues);
	
	// we read compass only if HW_Bit is set
	if (getParameter(PARA_HW) & PARA_HW_COMP)
	{
		getCompassAngle();
	}
	
	
	// get basis angles from sensors
	getGyroAnglesFilterACC(gyroAngle, gyroRawValues);
	getACCAnglesFilterACC(accAngle, accRawValues);
	
	s32 factorDegree = (abs(copterAngle[X] - 9000000) / 100000) + (abs(copterAngle[Y] - 9000000) / 100000)+ 10;
	factorDegree = factorDegree * factorDegree;
	u32 accForce = (parameter[PARA_ACC_FORCE] * 100 / factorDegree) + 1;
	u32 gyroCorr = (parameter[PARA_GYRO_CORR] * 100 / factorDegree) + 1;
	
	//needs about 50us (all 5)
	copterAngle[X] = weightingValues(accAngle[X], gyroAngle[X], accForce); 
	copterAngle[Y] = weightingValues(accAngle[Y], gyroAngle[Y], accForce); 
	copterAngle[Z] = gyroAngle[Z];
	
	//char x [80];
	//sprintf(x,"%d:%d:%d\r\n",factorDegree,abs(copterAngle[X] - 9000000),abs(copterAngle[Y] - 9000000));
	//print_uart1(x);
	
	// trimm gyro to new Angle
	gyroAngle[X] = weightingValues(copterAngle[X], gyroAngle[X], gyroCorr); 
	gyroAngle[Y] = weightingValues(copterAngle[Y], gyroAngle[Y], gyroCorr); 
}


/* calculate Gyro Angles ----------------------------------------------------*/
// 2mV/°/sec
// 12bit = 4095, Vref = 3,3V  => 3,3/4095 = 0,00080586085860
// ADC * 3,3 / 4095 / 2000 * 1000 
void getGyroAnglesFilterACC(vs32 * gyroAngle, vs32 * gyroRawValues)
{
	u8 i;
	//################# 
	//http://tom.pycke.be/mav/70/gyroscope-to-roll-pitch-and-yaw
	//try runge-kutta integration http://de.wikipedia.org/wiki/Runge-Kutta-Verfahren
	//integration(i) = integration(i-1) + 1/6 ( vali-3 + 2 vali-2 + 2 vali-1 + vali)
	
	vs32 actualGyroRawValues[3];
	//actualGyroRawValues[X] = (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100);
	//actualGyroRawValues[Y] = (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100);
	//actualGyroRawValues[Z] = (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100);
	
	//calc with integer
	actualGyroRawValues[X] = (gyroRawValues[X] * parameter[PARA_GYRO_X_90]) / 24818;
	actualGyroRawValues[Y] = (gyroRawValues[Y] * parameter[PARA_GYRO_Y_90]) / 24818;
	actualGyroRawValues[Z] = (gyroRawValues[Z] * parameter[PARA_GYRO_Z_90]) / 24818;
	
	gyroAngle[X] -= (gyroRawValues3[X] + 2 * gyroRawValues2[X] + 2 * gyroRawValues1[X] + actualGyroRawValues[X]) / 6;
	gyroAngle[Y] -= (gyroRawValues3[Y] + 2 * gyroRawValues2[Y] + 2 * gyroRawValues1[Y] + actualGyroRawValues[Y]) / 6;
	gyroAngle[Z] -= (gyroRawValues3[Z] + 2 * gyroRawValues2[Z] + 2 * gyroRawValues1[Z] + actualGyroRawValues[Z]) / 6; 

	
	//char x [80];
	//sprintf(x,"%d\r\n",actualGyroRawValues[X]);
	//print_uart1(x);
	
	for (i = 0; i < 3; i++)
	{
		gyroRawValues3[i] = gyroRawValues2[i];
		gyroRawValues2[i] = gyroRawValues1[i];
		gyroRawValues1[i] = actualGyroRawValues[i];
	}
	
	//#### end test runge-kutta integration ####
	
	/*
	gyroAngle[X] -= (vs32) (gyroRawValues[X] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_X_90] * 100);
    gyroAngle[Y] -= (vs32) (gyroRawValues[Y] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Y_90] * 100);
    gyroAngle[Z] -= (vs32) (gyroRawValues[Z] * ( 3.3 / 4095.0 / 2000.0 ) * parameter[PARA_GYRO_Z_90] * 100);
	*/
	
	// overrun
	//u8 i;
	for (i = 0; i < 3; i++)
	{
		
		if (gyroAngle[i] >= 36000000) 
		{
			gyroAngle[i] -= 36000000;
		}
		if (gyroAngle[i] < 0) 
		{
			gyroAngle[i] += 36000000;
		}
	}
	
	//char x [80];
	//sprintf(x,"gyro raw value:%d:%d:%d\r\n",gyroAngle[X],gyroRawValues[X],gyroAngle[Z]);
	//print_uart1(x);
}


/* calculate ACC Angles -----------------------------------------------------*/
void getACCAnglesFilterACC(vs32 * accAngle, vs32 * accRawValues)
{
	// x = (x - corrACC) * factorACC * 180 / PI
	// z = (z - corrACC) * factorACC * 180 / PI
	// 180 / PI = 57.2957795
	// minus 90 grad für level
	//ACCAngle[X] = atan2((ACCRaw[Z] + corrACC[X]) * factorACC[X], (ACCRaw[X] + corrACC[X]) * factorACC[X]) * 57.2957795 + 90;
	
	// atan2 works - if it is to slow we can use fastatan2
	//accAngle[X] = fastatan2(ADCSensorValue[ACC_Z] - parameter[PARA_ACC_X_ZERO] , ADCSensorValue[ACC_X] - parameter[PARA_ACC_X_ZERO] ) * 57.2957795 * 100.0;
	accAngle[X] = (vs32) (atan2(accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] * 100 , accRawValues[X] - parameter[PARA_ACC_X_ZERO] * 100 ) * 5729577.95);
	accAngle[Y] = (vs32) (atan2(accRawValues[Y] - parameter[PARA_ACC_Y_ZERO] * 100 , accRawValues[Z] - parameter[PARA_ACC_Z_ZERO] * 100 ) * 5729577.95 + 9000000);
	

	//char x [80];
	//sprintf(x,"test:%d:%d:\r\n",accAngle[X],accAngle[Y]);
	//print_uart1(x);
	
}


/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterACC(vu16 * receiverChannel)
{
	// 90 = neutral
	// max is 20 to 160° - this are 70° for 500 points
	targetAngle[X] = 2000000 + ((14000) * (receiverChannel[ROLL] - 1000));
	targetAngle[Y] = 2000000 + ((14000) * (receiverChannel[NICK] - 1000));
	// only use yaw if stick is more than 5 points out of center and pitch not min
	if (receiverChannel[YAW] < 1495 || receiverChannel[YAW] > 1505)
	{
		if(receiverChannel[PITCH] > 1005)
		{
			targetAngle[Z] -= (receiverChannel[YAW] - 1500) * parameter[PARA_STICK_FACTOR];
		}
	}
	
	if (targetAngle[Z] >= 36000000) 
	{
		targetAngle[Z] -= 36000000;
	}
	if (targetAngle[Z] < 0) 
	{
		targetAngle[Z] += 36000000;
	}
}