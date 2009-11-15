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

#include <stdio.h>
#include "serial.h"

#include "led.h"
#include "eeprom.h" 

/* Variables ----------------------------------------------------------------*/



// EEPROM - TEST
/* Virtual address defined by the user: 0xFFFF value is prohibited */
//u16 VirtAddVarTab[NumbOfVar] = {0x5555, 0x6666, 0x7777};
	
int main(void)
{
	/* Initialize S ystem */
	initSystem();
	
	
	/* Test EEPROM -------------------------------------------------------------*/

	/* Unlock the Flash Program Erase controller */
	//FLASH_Unlock();

	/* EEPROM Init */
	//EE_Init();


	//EE_WriteVariable(VirtAddVarTab[0], 1234);
	
	//u16 testvar;
	//EE_ReadVariable(VirtAddVarTab[0], &testvar);
	//sprintf(x,"%d",testvar);
	
	// in wrong order
	//u8 ii = 0;
	//while (testvar != 0) {
	//	x[ii++] = (testvar % 10) + 0x30;  // 0x30 is ansi 0
	//	testvar = testvar / 10;
	//}
	
	//print_uart1(x);
  
	// test LED
	//setLEDStatus(LED_FLASH);
  
	while (1)
	{
		// Mainloop --> statemachine() --> Timer 3
	}
}











