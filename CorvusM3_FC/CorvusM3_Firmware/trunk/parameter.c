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
#include "sal.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vs32 gyroAngle[3]; //statemachine
extern vs32 copterAngle[3]; //statemachine

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
		// we have sometimes a problem ...
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
	EE_WriteVariable(VirtAddVarTab[PARA_SET], 0);
	EE_WriteVariable(VirtAddVarTab[PARA_DEBUG], 0);
	EE_WriteVariable(VirtAddVarTab[PARA_HW], 3); //0=RC,1=PC,2=MotMix,4=Compass
	EE_WriteVariable(VirtAddVarTab[PARA_VOLT], 2236); //2236 = 10 Volt
	EE_WriteVariable(VirtAddVarTab[PARA_SW], 0);  //0=HH,1=acc,2=comp2 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_X_ZERO], 20470); //20470
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_Y_ZERO], 20330); //20330
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_Z_ZERO], 24450); //24450
	EE_WriteVariable(VirtAddVarTab[PARA_SMOOTH_ACC], 00); 
	EE_WriteVariable(VirtAddVarTab[PARA_STICK_FACTOR], 50); 
	EE_WriteVariable(VirtAddVarTab[PARA_SMOOTH_RC], 00); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_X_90], 2200); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_Y_90], 2200); 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_Z_90], 2200); 
	EE_WriteVariable(VirtAddVarTab[PARA_ACC_FORCE], 0); //500 
	EE_WriteVariable(VirtAddVarTab[PARA_GYRO_CORR], 0); //500
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO0_CH], 5); //Kanal 5
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO1_CH], 6); //Kanal 6
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO2_CH], 7); //Kanal 7 
	EE_WriteVariable(VirtAddVarTab[PARA_SERVO3_CH], 8); //Kanal 8  
	EE_WriteVariable(VirtAddVarTab[PARA_CAM_X], 20500);   //servo 2 , 0, wert 500
	EE_WriteVariable(VirtAddVarTab[PARA_CAM_Y], 30500);   //servo 3 , 0, wert 500
	EE_WriteVariable(VirtAddVarTab[PARA_MIN_GAS], 10);  // min gas 24 
	EE_WriteVariable(VirtAddVarTab[PARA_P_X], 1000); //23:1000  
	EE_WriteVariable(VirtAddVarTab[PARA_I_X], 0); //24: 
	EE_WriteVariable(VirtAddVarTab[PARA_D_X], 3000);  //25:3000
	EE_WriteVariable(VirtAddVarTab[PARA_P_Y], 1000);  //26:
	EE_WriteVariable(VirtAddVarTab[PARA_I_Y], 0); //27: 
	EE_WriteVariable(VirtAddVarTab[PARA_D_Y], 3000);  //28:
	EE_WriteVariable(VirtAddVarTab[PARA_P_Z], 1000);  //29:
	EE_WriteVariable(VirtAddVarTab[PARA_I_Z], 500);  //30:
	EE_WriteVariable(VirtAddVarTab[PARA_D_Z], 0);  //31:
	EE_WriteVariable(VirtAddVarTab[PARA_COMP_CORR_X], 0);  
	EE_WriteVariable(VirtAddVarTab[PARA_COMP_CORR_Y], 0);  
	EE_WriteVariable(VirtAddVarTab[PARA_COMP_CORR_Z], 0);  
	EE_WriteVariable(VirtAddVarTab[PARA_COMP_DECL], 0);  
	EE_WriteVariable(VirtAddVarTab[PARA_COMP_FORCE], 0); 
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT1A], 25600); //100, 0  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT1B], 40092); //-100, -100
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT2A], 25600); //100, 0  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT2B], 25756); //100, -100  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT3A], 25700); //100, 100 
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT3B], 100); //0, 100  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT4A], 25828); //100, -100  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT4B], 100); //0, 100  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT5A], 25600); //100, 0 - Motor not used 
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT5B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT6A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT6B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT7A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT7B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT8A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT8B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT9A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT9B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT10A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT10B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT11A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT11B], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT12A], 25600);  
	EE_WriteVariable(VirtAddVarTab[PARA_MIXER_MOT12B], 25600);   
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
		// if we change the SW Parameter we have to init the filter
		if (para == PARA_SW) // = 4
		{
			initFilter(gyroAngle, copterAngle);
		}
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
		// if we change the SW Parameter we have to init the filter
		if (para == PARA_SW) // = 4
		{
			initFilter(gyroAngle, copterAngle);
		}
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
		// if we change the SW Parameter we have to init the filter
		if (para == PARA_SW) // = 4
		{
			initFilter(gyroAngle, copterAngle);
		}
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


