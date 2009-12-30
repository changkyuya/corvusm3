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

#include "debug.h"
#include "serial.h"
#include <stdio.h>
#include "parameter.h"


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0x190];  //statemachine

/* doDebug - send Debug infos over Serial -----------------------------------*/
void doDebug()
{
	char x [200];			
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_ADC)
	{
		
		//sprintf(x,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",ADCSensorValue[0],ADCSensorValue[7],ADCSensorValue[14],ADCSensorValue[21],ADCSensorValue[28],ADCSensorValue[35],ADCSensorValue[42],ADCSensorValue[49]);
		//print_uart1(x);
	}
	
	/* Graphoutput must start with 'G' and first value is timer (not used) */
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_SENSOR)
	{
		
	}
	

	/* Debug output for receiver channels */
	// if parameter value is 1 - 1. bit is set
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_REC)
	{
		
	}
	

	/* Debug output for receiver channels */
	// if parameter value is 1 - 1. bit is set
	if (getParameter(PARA_DEBUG) & PARA_DEBUG_MOT)
	{
		
	}
	
}