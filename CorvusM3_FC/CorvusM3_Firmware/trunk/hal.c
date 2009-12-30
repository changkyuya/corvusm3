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

#include "hal.h"
#include "receiverppm.h"
#include "receiverspektrum.h"
#include "parameter.h"
#include "motorhal.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 receiverPCChannel[9]; //comm
extern vu8 errorCode; //statemachine
extern vu32 msCount; //statemachine
extern vu32 msLastPCRC; //comm

/* fill channels in Struc ---------------------------------------------------*/
void getChannels(vu16 *receiverChannel)
{
	// spektrum is online - PPM not
	if (isSpektrumOnline() != 0)
	{
		getSpektrumChannels(receiverChannel);
		errorCode &= ~ERROR_RC;
	} 
	// PPM is online - Spektrum not
	else if (isPPMonline() != 0)
	{
		getPPMChannels(receiverChannel);
		errorCode &= ~ERROR_RC;
	} 
	else if ((getParameter(PARA_HW) & PARA_HW_PC) && (msLastPCRC + 500) > msCount) //if signal is all 500ms
	{
		receiverChannel[0] = receiverPCChannel[0];
		receiverChannel[1] = receiverPCChannel[1];
		receiverChannel[2] = receiverPCChannel[2];
		receiverChannel[3] = receiverPCChannel[3];
		receiverChannel[4] = receiverPCChannel[4];
		errorCode &= ~ERROR_RC;
	}
	else
	{
		// no signal - panic!
		receiverChannel[0] = 0;
		errorCode |= ERROR_RC;
	}

	
}



/* map pid to motors -------------------------------------------------------*/
void sendPIDMotors(vs32 * PIDCorr, vu16 * receiverChannel, volatile char * motor)
{
	if (getParameter(PARA_HW) & PARA_HW_4X)
	{
		// X mode
		sendPIDMotors4X(PIDCorr, receiverChannel, motor);
	}
	else
	{
		// normal + mode
		sendPIDMotors4Plus(PIDCorr, receiverChannel, motor);
	}
}





