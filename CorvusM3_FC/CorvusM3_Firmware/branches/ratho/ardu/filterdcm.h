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

#define Kp_ROLLPITCH 125  // Pitch&Roll Proportional Gain
#define Ki_ROLLPITCH 1 // Pitch&Roll Integrator Gain


/* Function prototypes -------------------------------------------------------*/
void initFilterDCM(vs32 * gyroAngle, vs32 * copterAngle);
void setAngleFilterDCM(vs32 * gyroAngle, vs32 * copterAngle);
void getCopterAnglesFilterDCM(vs32 * copterAngle);
void mapReceiverValuesFilterDCM(vu16 * receiverChannel);
s32 Vector_Dot_Product(vs32 vector1[3],vs32 vector2[3]);
void Vector_Cross_Product(vs32 vectorOut[3], vs32 v1[3],vs32 v2[3]);
void Vector_Scale(vs32 vectorOut[3],vs32 vectorIn[3], vs32 scale2);
void Vector_Add(vs32 vectorOut[3],vs32 vectorIn1[3], vs32 vectorIn2[3]);
void Matrix_Multiply(vs32 a[3][3], vs32 b[3][3],vs32 mat[3][3]);
void Normalize(void);
void Drift_correction(void);
void Matrix_update();
void Euler_angles(vs32 * copterAngle);

#endif /* __FILTERDCM_H */