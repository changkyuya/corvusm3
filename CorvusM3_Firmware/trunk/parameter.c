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

#include "parameter.h"
#include "eeprom.h"
#include "comm.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu16 parameter[0xFF]; 
/* Virtual address defined by the user: 0xFFFF value is prohibited */
// we use 0 - 199
u16 VirtAddVarTab[NumbOfVar];


/* init virtual EEPROM in FLASH ---------------------------------------------*/
void initEEPROM()
{
	/* Set virtual address */
	u16 i;
	for (i = 0; i < 0xC8; i++)
	{
		VirtAddVarTab[i] = i;
	}
	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();
	/* EEPROM Init */
	EE_Init();
}

/* load Parameter from virtual EEPROM ---------------------------------------*/
void loadParameter()
{
	u16 val;
	u8 i;
	
	// test if flash is OK
	EE_ReadVariable(VirtAddVarTab[0], &val);
	if (val > USED_PARAMETER)
	{
		// store default set if flash
		EE_WriteVariable(VirtAddVarTab[PARA_SET], 0x00);
		EE_WriteVariable(VirtAddVarTab[PARA_DEBUG], 0x00);
		send(DEFAULT);
		
	}
	
	for (i = 0;i <= USED_PARAMETER; i++)
	{
		EE_ReadVariable(VirtAddVarTab[i], &val);
		parameter[i] = val;
		// for test
		print_para(i, parameter[i]);
	}
}


