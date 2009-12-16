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

#include "servo.h"
#include <stdio.h>
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu16 servo[4];
extern vu16 parameter[0x190]; //parameter


/* set all Servos to next 4 Channels ----------------------------------------*/
void setAllServos(vu16 * receiverChannel, volatile float * copterAngle)
{
	s16 servoVals[4];
	
	
	servoVals[0] = (receiverChannel[parameter[PARA_SERVO0_CH]] == 0) ? 1500 : receiverChannel[parameter[PARA_SERVO0_CH]];
	servoVals[1] = (receiverChannel[parameter[PARA_SERVO1_CH]] == 0) ? 1500 : receiverChannel[parameter[PARA_SERVO1_CH]];
	servoVals[2] = (receiverChannel[parameter[PARA_SERVO2_CH]] == 0) ? 1500 : receiverChannel[parameter[PARA_SERVO2_CH]];
	servoVals[3] = (receiverChannel[parameter[PARA_SERVO3_CH]] == 0) ? 1500 : receiverChannel[parameter[PARA_SERVO3_CH]];

		
	if (parameter[PARA_CAM_X] != 0)
	{
		if (parameter[PARA_CAM_X] < 10000) // servo 0
		{
			if (parameter[PARA_CAM_X] < 1000) //no invert
			{
				servoVals[0] += (s16) ((copterAngle[X] - 90.0) * (parameter[PARA_CAM_X] / 90.0));
			}
			else // invert
			{
				servoVals[0] = (s16) 3000 - (((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 1000) / 90.0)) + servoVals[0]);
			}
		}
		else if (parameter[PARA_CAM_X] < 20000) // servo 1
		{
			if ((parameter[PARA_CAM_X] - 10000) < 1000) //no invert
			{
				servoVals[1] += (s16) ((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 10000) / 90.0));
			}
			else // invert
			{
				servoVals[1] = (s16) 3000 - (((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 11000) / 90.0)) + servoVals[1]);
			}
		
		}
		else if (parameter[PARA_CAM_X] < 30000) // servo 2
		{
			if ((parameter[PARA_CAM_X] - 20000) < 1000) //no invert
			{
				servoVals[2] += (s16) ((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 20000) / 90.0));
			}
			else // invert
			{
				servoVals[2] = (s16) 3000 - (((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 21000) / 90.0)) + servoVals[2]);
			}
		
		}
		else if (parameter[PARA_CAM_X] < 40000) // servo 3
		{
			if ((parameter[PARA_CAM_X] - 30000) < 1000) //no invert
			{
				servoVals[3] += (s16) ((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 30000) / 90.0));
			}
			else // invert
			{
				servoVals[3] = (s16) 3000 - (((copterAngle[X] - 90.0) * ((parameter[PARA_CAM_X] - 31000) / 90.0)) + servoVals[3]);
			}
		
		}
	}
	
		
	if (parameter[PARA_CAM_Y] != 0)
	{
		if (parameter[PARA_CAM_Y] < 10000) // servo 0
		{
			if (parameter[PARA_CAM_Y] < 1000) //no invert
			{
				servoVals[0] += (s16) ((copterAngle[Y] - 90.0) * (parameter[PARA_CAM_Y] / 90.0));
			}
			else // invert
			{
				servoVals[0] = (s16) 3000 - (((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 1000) / 90.0)) + servoVals[0]);
			}
		}
		else if (parameter[PARA_CAM_Y] < 20000) // servo 1
		{
			if ((parameter[PARA_CAM_Y] - 10000) < 1000) //no invert
			{
				servoVals[1] += (s16) ((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 10000) / 90.0));
			}
			else // invert
			{
				servoVals[1] = (s16) 3000 - (((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 11000) / 90.0)) + servoVals[1]);
			}
		
		}
		else if (parameter[PARA_CAM_Y] < 30000) // servo 2
		{
			if ((parameter[PARA_CAM_Y] - 20000) < 1000) //no invert
			{
				servoVals[2] += (s16) ((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 20000) / 90.0));
			}
			else // invert
			{
				servoVals[2] = (s16) 3000 - (((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 21000) / 90.0)) + servoVals[2]);
			}
		
		}
		else if (parameter[PARA_CAM_Y] < 40000) // servo 3
		{
			if ((parameter[PARA_CAM_Y] - 30000) < 1000) //no invert
			{
				servoVals[3] += (s16) ((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 30000) / 90.0));
			}
			else // invert
			{
				servoVals[3] = (s16) 3000 - (((copterAngle[Y] - 90.0) * ((parameter[PARA_CAM_Y] - 31000) / 90.0)) + servoVals[3]);
			}
		
		}
	}
	
	setServoValue(0, servoVals[0]);
	setServoValue(1, servoVals[1]);
	setServoValue(2, servoVals[2]);
	setServoValue(3, servoVals[3]);
}


/* set server value ---------------------------------------------------------*/
void setServoValue(u8 servoNr, u16 value)
{
	servo[servoNr] = constrain(value, 1000, 2000);
}




