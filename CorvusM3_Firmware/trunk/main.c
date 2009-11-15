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

#include "eeprom.h" 

/* Variables ----------------------------------------------------------------*/
//test
extern vu8 TxBuffer1[0xFF];
extern vu8 TxInCounter1;
extern vu8 TxOutCounter1;
extern vu8 RxBuffer1[0xFF];
extern vu8 RxOutCounter1;
extern vu8 RxInCounter1;

extern vu8 TxBuffer3[0xFF];
extern vu8 TxInCounter3;
extern vu8 TxOutCounter3;
extern vu8 RxBuffer3[0xFF];
extern vu8 RxOutCounter3;
extern vu8 RxInCounter3;

	
int main(void)
{
	/* Initialize System */
	initSystem();
  
	
  
	while (1)
	{
		// Controlloop --> statemachine() --> Timer 3
		
		TxBuffer1[TxInCounter1++] = 0x33;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			
			
		setLEDStatus(LED_ON);
		Delay(1000);
		setLEDStatus(LED_OFF);
		Delay(1000);
		
		//Read Serial Ports
		
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}











