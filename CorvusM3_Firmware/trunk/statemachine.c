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
#include "motorhal.h"
#include "sensor.h"
#include "servo.h"
#include "pid.h"

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
vu32 msCount = 0;
vu32 msOldCount = 0;
vu16 receiverChannel[9]; 
vs32 targetAngle[3];
extern vu16 parameter[0x190]; //parameter
u8 flightState = FLIGHT_START;
volatile char motor[5]; 
vs32 gyroAngle[3];
//vs16 gyroAngle[3];
vs32 accAngle[2];
//vs16 accAngle[2];
vs32 copterAngle[3];
vu8 errorCode;
vu8 neutralRC;
vs32 PIDCorr[3];


	
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
	
	// start with new values from sensors
	getCopterAngles(gyroAngle, accAngle, copterAngle);
	// try to get channels
	getChannels(receiverChannel);
	// map channelvalues to targetAngle
	mapReceiverValues(receiverChannel, targetAngle, copterAngle);

	
	
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
			if (receiverChannel[0] == 1) // we have valid signal
			{
				flightState = FLIGHT_MOTOR;
			}
			break;
			
		case FLIGHT_MOTOR:
			// check special commands
			//start with neutral
			if (remoteCommands() == RC_NEUTRAL)
			{
				neutralRC = 1;
				errorCode &= ~ERROR_SENSOR; // delete bit
			}
			// like calibrate?
			if (remoteCommands() == RC_CALIBRATE && neutralRC == 1)
			{
				neutralRC = 0;
				errorCode |= ERROR_SENSOR; // set bit - led flashing
				zeroGyro();
				setGyroAngles(gyroAngle);
			}
			
			// start motors
			if (remoteCommands() == RC_MOTORS && neutralRC == 1)
			{
				neutralRC = 0;
				flightState = FLIGHT_FLYING;
				print_uart1("Motors ON\r\n");
			}
			break;
			
		case FLIGHT_FLYING:
			//start with neutral
			if (remoteCommands() == RC_NEUTRAL)
			{
				neutralRC = 1;
			}
			// flightcontroll
			calcPIDCorr(PIDCorr, copterAngle, targetAngle);
			// map PIDCorr to 4 Motors
			mapPIDMotors(PIDCorr, receiverChannel, motor);
			// command motors
			sendMotor(motor);
			
			// we like to stop!
			// no other remotecommands allowed
			if (remoteCommands() == RC_MOTORS && neutralRC == 1) 
			{
				neutralRC = 0;
				flightState = FLIGHT_MOTOR;
				stopAllMotors(motor);
				print_uart1("Motors OFF\r\n");
			}
			break;
		default:
			break;
	}
	

	

	// only for test brushless controller !
	// map receiverChannel 1 to BLMC Motor 1
	//motor[1] = map(receiverChannel[PITCH],1000,2000,0,200);
	//sendMotor(motor);
	
	
	// send new values to servo
	setAllServos(receiverChannel, copterAngle);

	
	
	/* Count Loops from Statemachine 1ms */	
	msCount++;
	// Debug to measure time for loop - toggle debug-Pin PA00
	*DBG = 0;
}


/* check remote commands ----------------------------------------------------*/
u8 remoteCommands(void)
{
	if (receiverChannel[PITCH] < 1010 && receiverChannel[YAW] > 1990) //we like to start motors
	{
		return RC_MOTORS;
	}
	if (receiverChannel[PITCH] > 1990 && receiverChannel[YAW] > 1990) //we like to calibrate
	{
		return RC_CALIBRATE;
	}
	if (receiverChannel[PITCH] < 1010 && receiverChannel[YAW] > 1400 && receiverChannel[YAW] < 1600 ) // neutral
	{
		return RC_NEUTRAL;
	}
	
	return RC_NO;
}
