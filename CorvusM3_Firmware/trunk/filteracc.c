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
extern vu16 gyroZero[3]; // sensor
extern vu16 parameter[0xFF]; //parameter


/* calculate ACC Angles -----------------------------------------------------*/
void getACCAnglesFilterACC(vs16 * accAngle)
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
	accAngle[X] = atan2(accRawValues[Z] - parameter[PARA_ACC_X_ZERO] , accRawValues[X] - parameter[PARA_ACC_X_ZERO] ) * 57.2957795 * 100.0;
	accAngle[Y] = atan2(accRawValues[Z] - parameter[PARA_ACC_Y_ZERO] , accRawValues[Y] - parameter[PARA_ACC_Y_ZERO] ) * 57.2957795 * 100.0;

	
	//char x [80];
	//sprintf(x,"test:%d:%d:\r\n",accAngle[X],accAngle[Y]);
	//print_uart1(x);
	
}

/* set gyro start angle -----------------------------------------------------*/
void setGyroAngleFilterACC(vs16 * gyroAngle)
{
	vs16 accAngle[2];
	getACCAnglesFilterACC(accAngle);
	gyroAngle[X] = accAngle[X];
	gyroAngle[Y] = accAngle[Y];
	gyroAngle[Z] = 18000;
	
	char x [80];
	sprintf(x,"gyro start value:%d:%d:%d\r\n",gyroAngle[X],gyroAngle[Y],gyroAngle[Z]);
	print_uart1(x);
}

/* calculate Gyro Angles ----------------------------------------------------*/
// 2mV/�/sec
// 12bit = 4095, Vref = 3,3V  => 3,3/4095 = 0,00080586085860
// ADC * 3,3 / 4095 / 2000 * 1000 
void getGyroAnglesFilterACC(vs16 * gyroAngle)
{
	vs16 gyroRawValues[3];
	getGyroRawValues(gyroRawValues);
	gyroAngle[X] += gyroRawValues[X] * (( 3.3 / 4095.0 / 2.0 ) * 100 );
	gyroAngle[Y] += gyroRawValues[Y] * (( 3.3 / 4095.0 / 2.0 ) * 100 );
	gyroAngle[Z] += gyroRawValues[Z] * (( 3.3 / 4095.0 / 2.0 ) * 100 );
	
	//char x [80];
	//sprintf(x,"gyro raw value:%d:%d:%d\r\n",gyroAngle[X],gyroRawValues[X],gyroAngle[Z]);
	//print_uart1(x);
}




