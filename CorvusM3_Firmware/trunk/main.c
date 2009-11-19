/*	Part of the Corvus M3 project

	http://corvusm3.blogspot.com/
	http://code.google.com/p/corvusm3/	

	Copyright (c) 2009 Thorsten Raab - thorsten.raab@gmx.at

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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











