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
#include "tools.h"

#include <stdio.h>
#include "serial.h"

/* variables ----------------------------------------------------------------*/
static vu32 TimingDelay = 0;	// for Delay function

/* Delay function - systick -------------------------------------------------*/
void Delay(vu32 nTime)
{ 
	TimingDelay = nTime;
	while(TimingDelay != 0);
}

/* Interrupt Handler for SysTick --------------------------------------------*/
/* [0x3C] SysTick Exception */
void SysTickHandler(void)
{
  if (TimingDelay != 0x00)
  { 
	TimingDelay--;
  }
}

/* Delay does not work in Interrupt -----------------------------------------*/
void Pause(u32 approxms)
{
	// pause(x) delay for APPROXIMATELY x milliseconds, works within interrupt
	volatile unsigned int i=40*approxms;
	volatile unsigned short j;
	while(i--) for(j=0;j<200;j++);
}


