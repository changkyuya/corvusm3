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

/* Include ------------------------------------------------------------------*/
#include "main.h"
#include "statemachine.h"
#include "serial.h"
#include <stdio.h>
#include "hal.h"
#include "sal.h"
#include "led.h"
#include "parameter.h"
#include "blmc.h"
#include "sensor.h"
#include "servo.h"

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
vu32 msCount = 0;
vu32 msOldCount = 0;
vu16 receiverChannel[0xFF]; 
volatile float targetAngle[3];
extern vu16 parameter[0x190]; //parameter
u8 flightState = FLIGHT_START;
volatile char motor[5]; 
volatile float gyroAngle[3];
//vs16 gyroAngle[3];
volatile float accAngle[2];
//vs16 accAngle[2];
volatile float copterAngle[3];
vu8 errorCode;


	
/* [0xB4] TIM3 Interrupt ----------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	/* Clear TIM3 update interrupt */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	statemachine();
}

/* Main Control Loop --------------------------------------------------------*/
void statemachine(void)
{
	// Debug to measure time for loop - toggle debug-Pin PA00
	*DBG = 1;
	
	
	/* statemachine
	power on
	init system
	calibrate gyro
	check rc
	start motors
	flight
	stop motors -> check rc
	*/
	switch (flightState)
	{
		case FLIGHT_START:
			// do something - beep?
			flightState = FLIGHT_RC;
			break;
		
		case FLIGHT_RC:
			getChannels(receiverChannel);
			// test if valid signal
			mapReceiverValues(receiverChannel, targetAngle);
			break;
			
		default:
			break;
	}
	

	

	getCopterAngles(gyroAngle, accAngle, copterAngle);

	// only for test brushless controller !
	// map receiverChannel 1 to BLMC Motor 1
	motor[1] = map(receiverChannel[PITCH],1000,2000,0,200);
	motor[2] = 0x00;
	motor[3] = 0x00;
	motor[4] = 0x00;
	sendMotor(motor);
	
	//only for test servo !
	// map channel 1 to servo 1
	//setServoValue(2, receiverChannel[PITCH]);
	//setServoValue(3, receiverChannel[ROLL]);
	setAllServos(receiverChannel, copterAngle);

	
	
	/* Count Loops from Statemachine 1ms */
	msCount++;
	// Debug to measure time for loop - toggle debug-Pin PA00
	*DBG = 0;
}
