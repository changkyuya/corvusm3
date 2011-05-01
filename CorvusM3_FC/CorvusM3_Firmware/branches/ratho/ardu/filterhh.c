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
extern vs32 targetAngle[3]; //statemachine
vs32 gyroRawValues1[3]; //-1 Value for Runge-Kutta or Integration
vs32 gyroRawValues2[3]; //-2 Value for Runge-Kutta or Integration
vs32 gyroRawValues3[3]; //-3 Value for Runge-Kutta or Integration


/* init filter --------------------------------------------------------------*/
void initFilterHH(vs32 * gyroAngle, vs32 * copterAngle)
{
	zeroGyro();
	setAngleFilterHH(gyroAngle, copterAngle);
}


/* set gyro start angle -----------------------------------------------------*/
void setAngleFilterHH(vs32 * gyroAngle, vs32 * copterAngle)
{
	copterAngle[X] = gyroAngle[X] = targetAngle[X] = 9000000;
	copterAngle[Y] = gyroAngle[Y] = targetAngle[Y] = 9000000;
	copterAngle[Z] = gyroAngle[Z] = targetAngle[Z] = 0;
	
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
}



/* map receiver to angles for roll nick -------------------------------------*/
void mapReceiverValuesFilterHH(vu16 * receiverChannel)
{
	// only use yaw if stick is more than 5 points out of center 
	
	if (receiverChannel[ROLL] < 1490 || receiverChannel[ROLL] > 1510)
	{
		targetAngle[X] += (receiverChannel[ROLL] - 1500) * parameter[PARA_STICK_FACTOR];
	}
	
	if (receiverChannel[NICK] < 1490 || receiverChannel[NICK] > 1510)
	{
		targetAngle[Y] += (receiverChannel[NICK] - 1500) * parameter[PARA_STICK_FACTOR];
	}
	
	// only use yaw if stick is more than 5 points out of center and pitch not min
	if (receiverChannel[YAW] < 1490 || receiverChannel[YAW] > 1510)
	{
		if(receiverChannel[PITCH] > 1010)
		{
			targetAngle[Z] -= (receiverChannel[YAW] - 1500) * parameter[PARA_STICK_FACTOR];
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
			targetAngle[i] += 36000000;
		}
	}
}