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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOOLS_H
#define __TOOLS_H

#include "main.h"

/* define -------------------------------------------------------------------*/
#define PI 3.1415926


/* Function prototypes -------------------------------------------------------*/
void Delay(vu32 nTime);
void SysTickHandler(void);
void Pause(u32 approxms);
s32 smoothValue(s32 actual, s32 previous, u16 smooth); 
float fastatan2(float y, float x);
s32 weightingValues(s32 first, s32 secound, u16 weightingFirst); 

#endif /* __TOOLS_H */
