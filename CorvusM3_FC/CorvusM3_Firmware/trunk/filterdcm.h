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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILTERDCM_H
#define __FILTERDCM_H

#include "main.h"

/* Struct -------------------------------------------------------------------*/


/* Define --------------------------------------------------------------------*/

#define Kp_ROLLPITCH 0.0125  //0.010 // Pitch&Roll Proportional Gain
#define Ki_ROLLPITCH 0.000010 // Pitch&Roll Integrator Gain
#define Kp_YAW 0.5     // Yaw Porportional Gain  
#define Ki_YAW 0.00005 // Yaw Integrator Gain

/* Function prototypes -------------------------------------------------------*/
void initFilterDCM(vs32 * gyroAngle, vs32 * copterAngle);
void setAngleFilterDCM(vs32 * gyroAngle, vs32 * copterAngle);
void getCopterAnglesFilterDCM(vs32 * copterAngle);
void mapReceiverValuesFilterDCM(vu16 * receiverChannel);
float Vector_Dot_Product(float vector1[3],float vector2[3]);
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3]);
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2);
void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3]);
void Matrix_Multiply(float a[3][3], float b[3][3],float mat[3][3]);
void Normalize(void);
void Drift_correction(void);
void Accel_adjust(void);
void Matrix_update(vs32 * gyroRawValues, vs32 * accRawValues);
void Euler_angles(vs32 * copterAngle);

#endif /* __FILTERDCM_H */