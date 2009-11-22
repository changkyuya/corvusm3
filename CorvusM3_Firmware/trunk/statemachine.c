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
#include "led.h"
#include "parameter.h"

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[7];  //initsystem
char x [10];  // for Sensor Tests
vu32 msCount = 0;
vu32 msOldCount = 0;
vu16 receiverChannel[9]; 
extern vu16 parameter[0xFF]; //parameter
u8 bootState = SWITCH_ON;
u8 flightState = FLIGHT_START;


	
/* [0xB4] TIM3 Interrupt ----------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	statemachine();
}

/* Main Control Loop --------------------------------------------------------*/
void statemachine(void)
{
	/* Clear TIM3 update interrupt */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	// statemachine for flight
	switch (bootState)
	{
		// switched on board
		case SWITCH_ON:
			setLEDStatus(LED_OFF);
			bootState = READ_SENSOR_FIRST;
			break;
		// read sensor data for gyro calibration
		case READ_SENSOR_FIRST:
			setLEDStatus(LED_FLASH);
			// function open ....
			bootState = LOAD_PARA;
			break;
		// read parameters from flash
		case LOAD_PARA:
			loadParameter();
			bootState = CALIBRATE_SENSOR;
			msOldCount = msCount;
			break;
		// calibrate sensor
		case CALIBRATE_SENSOR:
			// wait 1 secound for calibrate
			if (msOldCount + 1000 < msCount)
			{
				// function open ....
				setLEDStatus(LED_ON);
				bootState = GO_FLIGHT;
			}
			break;
	}
	
	// flight states
	if (bootState == GO_FLIGHT)
	{
		if (flightState == FLIGHT_START)
		{
			// do something - beep?
			flightState = FLIGHT_RC_ON;
		}
		// first step is to activate RC
		if (flightState > FLIGHT_START)
		{
			getChannels();
		}
	}
	

	
	/* Count Loops from Statemachine 1ms */
	msCount++;
}
