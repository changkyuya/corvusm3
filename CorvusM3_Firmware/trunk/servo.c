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
vu16 servoCount = 0;
vu16 servo[4];

/* Servo Timer Interrupt 4 --------------------------------------------------*/
void TIM4_IRQHandler(void)
{	
	/* Clear TIM4 update interrupt */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	doServo();
}

/* Output to Servo Pins -----------------------------------------------------*/
void doServo()
{
	// for test
	// we use 1.5 ms = 1500us and a gap 20ms-1.5ms = 18500us
	
	// if servo not set use 1500
	if (servo[0] == 0)
	{
		servo[0] = 1500;
	}
	if (servo[1] == 0)
	{
		servo[1] = 1500;
	}
	if (servo[2] == 0)
	{
		servo[2] = 1500;
	}
	if (servo[3] == 0)
	{
		servo[3] = 1500;
	}
	
	switch (servoCount)
	{
		case 0:
			// Signal ON
			*SERVO0 = 1;
			servoCount++;
			changeServoInterrupt(servo[0]);
			//changeServoInterrupt(1125);
			break;
		case 1:
			// Signal OFF
			*SERVO0 = 0;
			*SERVO1 = 1;
			servoCount++;
			changeServoInterrupt(servo[1]);
			//changeServoInterrupt(2254);
			break;
		case 2:
			// Signal OFF
			*SERVO1 = 0;
			*SERVO2 = 1;
			servoCount++;
			changeServoInterrupt(servo[2]);
			//changeServoInterrupt(2254);
			break;
		case 3:
			// Signal OFF
			*SERVO2 = 0;
			*SERVO3 = 1;
			servoCount++;
			changeServoInterrupt(servo[3]);
			//changeServoInterrupt(2254);
			break;
		default:
			// Signal OFF
			*SERVO3 = 0;
			// wait for next loop
			servoCount = 0;
			changeServoInterrupt(20000 - servo[0] - servo[1] - servo[2] - servo[3]);
			//changeServoInterrupt(16254);
			break;
	}
}


/* change Interrupt to next Timeslot ----------------------------------------*/
void changeServoInterrupt(vu16 nextTime)
{
	TIM4->ARR = nextTime; 				//alle 1ms
	
}


/* set server value ---------------------------------------------------------*/
void setServoValue(u8 servoNr, u16 value)
{
	servo[servoNr] = value;
}




