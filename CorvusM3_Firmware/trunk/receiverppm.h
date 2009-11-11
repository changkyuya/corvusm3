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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RECEIVERPPM_H
#define __RECEIVERPPM_H

#include "main.h"

/* Struct --------------------------------------------------------------------*/


/* Define --------------------------------------------------------------------*/
#define PPM_NO		0
#define PPM_SYNC 	1
#define PPM_OK		2
#define PPM_WRONG	3


/* Function prototypes -------------------------------------------------------*/
void TIM1_CC_IRQHandler(void);
void getPPMChannels(void);

#endif /* __RECEIVERPPM_H */