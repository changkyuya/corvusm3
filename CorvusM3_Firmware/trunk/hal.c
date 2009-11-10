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

#include "hal.h"
#include "receiverppm.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/


/* fill channels in Struc ---------------------------------------------------*/
void getChannels()
{
	/* decide receiver -> lookup parameterset */
	if (1)
	{
		getPPMChannels();
	}
	else
	{
		// getSpektrumChannels
	}
	 // no valid channels -> Set CopterState
}








