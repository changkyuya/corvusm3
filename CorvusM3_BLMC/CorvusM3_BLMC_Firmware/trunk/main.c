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
#include "parameter.h"
#include <stdio.h>

/* Variables ----------------------------------------------------------------*/
extern vu8 RxOutCounter1; //serial
extern vu8 RxInCounter1; //serial
extern vu32 msCount; //statemachine


	
int main(void)
{
	/* Initialize System */
	initSystem();

	/* Init virtual EEPROM */
	initEEPROM();
	
  
	print_uart1("CorvusM3 BLMC - Version 0.0a\r\n");
	
	// for test
	//char x [80];
	//sprintf(x,"%d:%d:%d\r\n",smoothValue(100,150,0),smoothValue(100,150,100),smoothValue(100,150,40));
	//print_uart1(x);
	
	
	setLEDStatus(LED_FLASH);
	
	// read parameters from flash
	loadParameter();
	
	
	// function open ....
	setLEDStatus(LED_BLINK);
	
  
	while (1)
	{

		// test accu
		if (getParameter(PARA_VOLT) > ADCSensorValue[VOLT])
		{
			// low accu
			errorCode |= ERROR_AKKU; // set bit
		}
		else
		{
			// accu OK
			errorCode &= ~ERROR_AKKU; // delete bit
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
		
		/* do LED -----------------------------------------------------------*/
		if (errorCode == 0)
		{
			setLEDStatus(LED_ON);
		}
		else if (errorCode < 2) // akku
		{
			setLEDStatus(LED_BLINK);
		}
		else if (errorCode < 8) // rc and sensor
		{
			setLEDStatus(LED_FLASH);
		}
		
	}
}











