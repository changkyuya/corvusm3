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

#include "main.h"
#include "initsystem.h"    
#include "comm.h"
#include "serial.h"
#include "eeprom.h" 
#include "debug.h"
#include "eeprom.h"
#include "parameter.h"
#include "hal.h"
#include "sal.h"
#include "blmc.h"
#include "sensor.h"
#include <stdio.h>

/* Variables ----------------------------------------------------------------*/
extern vu8 RxOutCounter1; //serial
extern vu8 RxInCounter1; //serial
extern vu32 msCount; //statemachine
extern vu16 ADCSensorValue[7];  //initsystem
extern vu16 receiverChannel[9];  //statemachine
extern volatile char motor[5]; //blmc
extern volatile float gyroAngle[3]; //statemachine



	
int main(void)
{
	/* Initialize System */
	initSystem();

	/* Init virtual EEPROM */
	initEEPROM();
	
  
	print_uart1("CorvusM3 - Version 0.0a\r\n");
	
	
	// read sensors for calibration
	setLEDStatus(LED_FLASH);
	// function open ....
	
	// read parameters from flash
	loadParameter();
	
	// calibrate sensor
	// wait 1 secound befor calibrate
	Delay(1000);
	zeroGyro();
	setGyroAngles(gyroAngle);
	
	// function open ....
	setLEDStatus(LED_BLINK);

	// test channels not used for test
	getChannels(receiverChannel);
	if (receiverChannel[0])
	{
		setLEDStatus(LED_ON);
	}
	
	// Controlloop --> statemachine() --> Timer 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
	while (1)
	{




	
		
		// test accu
		if (getParameter(PARA_VOLT) > ADCSensorValue[VOLT])
		{
			// low accu
			setLEDStatus(LED_FLASH);
		}
		else
		{
			// accu OK
			// must be overritten by flight errors like no receiver!!!!!!!!!!
			//setLEDStatus(LED_ON);
		}
		
		// if something in RxBuffer
		if (is_read_uart1())
		{
			getComm();
		}
		
		/* Debug Output 20Hz ---------------------------------------------------*/
		if (msCount % 50 == 0)
		{
			doDebug();
		}
		
	}
}











