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
vu16 parameter[0x190]; 
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
	u16 i;

	// test if flash is clear
	EE_ReadVariable(401, &val);
	if (val != 99)
	{
		loadDefault();
	}
	EE_WriteVariable(401, 99);
	
	for (i = 0;i <= USED_PARAMETER; i++)
	{
		EE_ReadVariable(VirtAddVarTab[i], &val);
		parameter[i] = val;
		// for test
		print_para(i);
		Delay(2); // prevent txbuffer overvlow
	}
}

/* load default parameter to flash ------------------------------------------*/
void loadDefault()
{
	//#########################################################################
	// store default set if flash - only set 1
	//#########################################################################
	EE_WriteVariable(VirtAddVarTab[PARA_SET], 0x00);
	EE_WriteVariable(VirtAddVarTab[PARA_DEBUG], 0x00);
	EE_WriteVariable(VirtAddVarTab[PARA_HW], 0x01);
	EE_WriteVariable(VirtAddVarTab[PARA_VOLT], 0x8BC); //2236 = 10 Volt
	EE_WriteVariable(VirtAddVarTab[PARA_SW], 0x01); 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_X_ZERO], 0x7FF); 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_Y_ZERO], 0x7F1); 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_Z_ZERO], 0x7F8); 
	EE_WriteVariable(VirtAddVarTab[PARA_SMOOTH_ACC], 0x00); 
	EE_WriteVariable(VirtAddVarTab[PARA_SMOOTH_GYRO], 0x00); 
	EE_WriteVariable(VirtAddVarTab[PARA_SMOOTH_RC], 0x00); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_X_90], 0x898); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_Y_90], 0x898); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_Z_90], 0x898); 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_FORCE], 0x1F4); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_CORR], 0x1F4); 
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO0_CH], 0x5);
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO1_CH], 0x6); 
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO2_CH], 0x7); 
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO3_CH], 0x8);  
	EE_WriteVariable(VirtAddVarTab[PARA_CAM_X], 0x5014);   //servo 2 , 0, wert 500
	EE_WriteVariable(VirtAddVarTab[PARA_CAM_Y], 0x7724);   //servo 3 , 0, wert 500
	EE_WriteVariable(VirtAddVarTab[PARA_MIN_GAS], 0x24);  // min gas 36 
	EE_WriteVariable(VirtAddVarTab[PARA_P_X], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_P_Y], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_I_X], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_I_Y], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_D_X], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_D_Y], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_P_X], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_I_Y], 0x0);  
	EE_WriteVariable(VirtAddVarTab[PARA_D_Z], 0x0);  
	//#########################################################################
	u16 val;
	u16 i;
	for (i = 0;i <= USED_PARAMETER; i++)
	{
		EE_ReadVariable(VirtAddVarTab[i], &val);
		parameter[i] = val;
		// for test
		print_para(i);
		Delay(2); // prevent txbuffer overvlow
	}
	// info user over uart1
	send(DEFAULT);
}


/* read parameter -----------------------------------------------------------*/
u16 getParameter(u16 para)
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
void setParameter(u16 para, u16 value)
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
u16 readFlashParameter(u16 para)
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
void writeFlashParameter(u16 para, u16 value)
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


/* flash parameter ---------------------------------------------------------*/
void flashAllParameter()
{	
	u16 i;
	for (i = 0;i <= USED_PARAMETER; i++)
	{
		writeFlashParameter(i, getParameter(i));
		print_para(i);
		Delay(2); // prevent txbuffer overvlow
	}
	// info user over uart1
	send(OK);
}


