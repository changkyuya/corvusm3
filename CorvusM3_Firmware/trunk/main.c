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

/* Variables ----------------------------------------------------------------*/
extern vu8 RxOutCounter1; //serial
extern vu8 RxInCounter1; //serial
extern vu32 msCount; //statemachine

	
int main(void)
{
	/* Initialize System */
	initSystem();
  
	char x[] = "CorvusM3 - Version 0.0a\r\n";
	print_uart1(x);
  
	while (1)
	{
		// Controlloop --> statemachine() --> Timer 3
		

		
		// if something in RxBuffer
		if (RxOutCounter1 != RxInCounter1)
		{
			getComm();
		}
		
		/* Debug Output 10Hz ---------------------------------------------------*/
		if (msCount % 100 == 0)
		{
			doDebug();
		}
		
	}
}











