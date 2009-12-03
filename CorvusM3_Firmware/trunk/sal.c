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

#include "sal.h"
#include "parameter.h"
#include "filteracc.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/

/* fill ACC Angles in Struc ---------------------------------------------------*/
void getACCAngles(volatile float * accAngle)
{
	/* decide receiver -> lookup parameterset */
	if (getParameter(PARA_SW) & PARA_SW_ACC) 
	{
		getACCAnglesFilterACC(accAngle);
	}
	else
	{
		// other Filter
	}
	
}

/* set gyro angle to start value acc-angle ----------------------------------*/
void setGyroAngle(volatile float * gyroAngle)
{
	/* decide receiver -> lookup parameterset */
	if (getParameter(PARA_SW) & PARA_SW_ACC) 
	{
		setGyroAngleFilterACC(gyroAngle);
	}
	else
	{
		// other Filter
	}
}


/* get Gyro Angles ----------------------------------------------------------*/
void getGyroValues(volatile float * gyroAngle)
{
	/* decide receiver -> lookup parameterset */
	if (getParameter(PARA_SW) & PARA_SW_ACC) 
	{
		getGyroAnglesFilterACC(gyroAngle);
	}
	else
	{
		// other Filter
	}
	
}







