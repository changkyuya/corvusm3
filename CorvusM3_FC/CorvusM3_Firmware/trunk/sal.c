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
#include "filtercomp2.h"
#include "filterhh.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/

/* init the filter ----------------------------------------------------------*/
void initFilter(vs32 * gyroAngle, vs32 * copterAngle)
{
	/* decide flightmode controller > lookup parameterset */
	switch (getParameter(PARA_SW) & PARA_SW_REGLER)
	{
		// HH-Mode
		case PARA_SW_HH:
			initFilterHH(gyroAngle, copterAngle);
			break;
		// ACC-Mode
		case PARA_SW_ACC:
			initFilterACC(gyroAngle, copterAngle);
			break;
		// ACC Comp2-Mode
		case PARA_SW_COMP2:
			initFilterComp2(gyroAngle, copterAngle);
			break;
		default:
			break;
	}	
}



/* set gyro angle to start value acc-angle ----------------------------------*/
void setAngles(vs32 * gyroAngle, vs32 * copterAngle)
{
	/* decide flightmode controller > lookup parameterset */
	switch (getParameter(PARA_SW) & PARA_SW_REGLER)
	{
		// HH-Mode
		case PARA_SW_HH:
			setAngleFilterHH(gyroAngle, copterAngle);
			break;
		// ACC-Mode
		case PARA_SW_ACC:
			setAngleFilterACC(gyroAngle, copterAngle);
			break;
		// ACC Comp2-Mode
		case PARA_SW_COMP2:
			setAngleFilterComp2(gyroAngle, copterAngle);
			break;
		default:
			break;
	}	
}




/* get Copter Angle ---------------------------------------------------------*/
void getCopterAngles(vs32 * gyroAngle, vs32 * accAngle, vs32 * copterAngle)
{
	/* decide flightmode controller > lookup parameterset */
	switch (getParameter(PARA_SW) & PARA_SW_REGLER)
	{
		// HH-Mode
		case PARA_SW_HH:
			getCopterAnglesFilterHH(gyroAngle, copterAngle);
			break;
		// ACC-Mode
		case PARA_SW_ACC:
			getCopterAnglesFilterACC(gyroAngle, accAngle, copterAngle);
			break;
		// ACC Comp2-Mode
		case PARA_SW_COMP2:
			getCopterAnglesFilterComp2(gyroAngle, accAngle, copterAngle);
			break;
		default:
			break;
	}	
}

void mapReceiverValues(vu16 * receiverChannel, vs32 * targetAngle)
{
	/* decide flightmode controller > lookup parameterset */
	switch (getParameter(PARA_SW) & PARA_SW_REGLER)
	{
		// HH-Mode
		case PARA_SW_HH:
			mapReceiverValuesFilterHH(receiverChannel, targetAngle);
			break;
		// ACC-Mode
		case PARA_SW_ACC:
			mapReceiverValuesFilterACC(receiverChannel, targetAngle);
			break;
		// ACC Comp2-Mode
		case PARA_SW_COMP2:
			mapReceiverValuesFilterComp2(receiverChannel, targetAngle);
			break;
		default:
			break;
	}	
}