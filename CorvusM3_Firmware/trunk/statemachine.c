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
	// Debug to measure time for loop - toggle debug-Pin PA00
	*DBG ^= 1;
	
	

	
	// flight states

	if (flightState == FLIGHT_START)
	{
		// do something - beep?
		flightState = FLIGHT_RC;
	}
	// first step is to activate RC
	if (flightState > FLIGHT_START)
	{
		getChannels();
		// test if valid signal
		if (receiverChannel[0])
		{
			flightState = FLIGHT_RC_ON;
			setLEDStatus(LED_ON);
		}
	}

	

	
	/* Count Loops from Statemachine 1ms */
	msCount++;
}
