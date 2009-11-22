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
		loadDefault();
	}
	
	for (i = 0;i <= USED_PARAMETER; i++)
	{
		EE_ReadVariable(VirtAddVarTab[i], &val);
		parameter[i] = val;
		// for test
		print_para(i, parameter[i]);
	}
}

/* load default parameter to flash ------------------------------------------*/
void loadDefault()
{
	//#########################################################################
	// store default set if flash
	//#########################################################################
	EE_WriteVariable(VirtAddVarTab[PARA_SET], 0x00);
	EE_WriteVariable(VirtAddVarTab[PARA_DEBUG], 0x00);
	EE_WriteVariable(VirtAddVarTab[PARA_HW], 0x01);
	//#########################################################################
	// info user over uart1
	send(DEFAULT);
}


/* read parameter -----------------------------------------------------------*/
u16 getParameter(u8 para)
{
	if (para == PARA_SET)
	{
		return parameter[PARA_SET];
	}
	else
	{
		return parameter[para + parameter[PARA_SET]];
	}
}

/* set parameter ------------------------------------------------------------*/
void setParameter(u8 para, u16 value)
{
	if (para == PARA_SET)
	{
		parameter[PARA_SET] = value;
	}
	else
	{
		parameter[para+parameter[PARA_SET]] = value;
	}
}

/* read from flash and set --------------------------------------------------*/
u16 readFlashParameter(u8 para)
{
	u16 val;
	if (para == PARA_SET)
	{
		EE_ReadVariable(VirtAddVarTab[PARA_SET], &val);
		parameter[PARA_SET] = val;
	}
	else
	{		
		EE_ReadVariable(VirtAddVarTab[para+parameter[PARA_SET]], &val);
		parameter[para+parameter[PARA_SET]] = val;
	}
	return val;
}


/* write parameter to flash -------------------------------------------------*/
void writeFlashParameter(u8 para, u16 value)
{
	if (para == PARA_SET)
	{
		EE_WriteVariable(VirtAddVarTab[PARA_SET], value);
		parameter[PARA_SET] = value;
	}
	else
	{
		EE_WriteVariable(VirtAddVarTab[para+parameter[PARA_SET]], value);
		parameter[para+parameter[PARA_SET]] = value;
	}
}



