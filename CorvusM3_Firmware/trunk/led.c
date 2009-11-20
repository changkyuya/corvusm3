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

#include "led.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
/* Led Status */
u8 ledStatus = LED_OFF;
/* Count for flash */
vu8 ledCount = 0;

/* Set Led status -----------------------------------------------------------*/
void setLEDStatus(u8 stat)
{
	ledStatus = stat;
}

/* Interrupt Handler for LED ------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	/* Clear TIM2 update interrupt */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	/* switch LED */
	switch (ledStatus)
	{
		/* switch led off */
		case LED_OFF:
			*LED = 0;
			break;
		/* switch led on */
		case LED_ON:
			*LED = 1;
			break;
		/* flash/toggle led in 250ms */
		case LED_FLASH:
			*LED ^= 1;
			break;
		/* blink/toggle led all 2 circles - 750ms */
		case LED_BLINK:
			if (ledCount == 3)
			{
				*LED ^= 1;
				ledCount = 0;
			}
			ledCount++;
			break;
		default:
			*LED = 0;
			ledCount = 0;
	}
}




