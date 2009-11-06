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