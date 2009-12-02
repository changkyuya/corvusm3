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

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/

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







