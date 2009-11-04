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
#include "initsystem.h"

#include "stm32exceptions.h"
#include <stdio.h>

#include "serial.h"
#include "led.h"

#include "eeprom.h" 





// EEPROM - TEST
/* Virtual address defined by the user: 0xFFFF value is prohibited */
u16 VirtAddVarTab[NumbOfVar] = {0x5555, 0x6666, 0x7777};









	
	
int main(void)
{
	/* Initialize System */
	initSystem();


	
	
	
	/* TEST ADC --------------------------------------------------------------------------------*/

	u16 test;
	char x [10];
	
	test = 1111;
	sprintf(x,"%d",test);
	print_uart1(x);


	

	ADC_InitTypeDef  ADC_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72MHz/6=12 MHz range must be 0.6-14MHz

    ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // This ADC independent from other ADC
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // Convert only specified chanel	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Convert on demand only
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Do conversion on demand
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC 12 bits data into 16 bits with 0 paddings
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC1, &ADC_InitStructure);	// Put all settings into structure

	ADC_Cmd(ADC1, ENABLE);				// Start ADCn
	ADC_ResetCalibration(ADC1);			// Reset calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);			// Start final calibration
	while(ADC_GetCalibrationStatus(ADC1));
  


	
	  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	

	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	
	
	test =  ADC_GetConversionValue(ADC1);
  
	
	sprintf(x,"%d",test);
	print_uart1(x);






	/* Test EEPROM -------------------------------------------------------------*/

	/* Unlock the Flash Program Erase controller */
	FLASH_Unlock();

	/* EEPROM Init */
	EE_Init();


	EE_WriteVariable(VirtAddVarTab[0], 1234);
	
	u16 testvar;
	EE_ReadVariable(VirtAddVarTab[0], &testvar);
	//sprintf(x,"%d",testvar);
	
	// in wrong order
	u8 ii = 0;
	while (testvar != 0) {
		x[ii++] = (testvar % 10) + 0x30;  // 0x30 is ansi 0
		testvar = testvar / 10;
	}
	
	print_uart1(x);
  
  
  while (1)
	{
		//PWR_EnterSTANDBYMode(); // Interrupt does not wake up ...
		
		//*LED ^= 1;											// toggle led
		//for (int i = 4; i > 0; i--)							// wait
		//{
		//	busyWait(0x8000);
		//}
	}
}











