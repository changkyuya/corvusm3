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

#include "motorhal.h"
#include "serial.h"
#include <stdio.h>

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 parameter[0x190]; //parameter
s16 motorTempOld[13]; 

/* send Motorcommands to BLMC over UART3 ------------------------------------*/
void sendMotor(volatile char * motor)
{
	motor[0] = 0xF5;
	/*
	u8 i;
	for (i = 1; i < 5; i++)
	{
		motor[i] = (motor[i] < parameter[PARA_MIN_GAS]) ? parameter[PARA_MIN_GAS] : motor[i];
	}
	*/
	print_uart3(motor);
}


/* stop all motors ----------------------------------------------------------*/
void stopAllMotors(volatile char * motor)
{
	motor[0] = 0xF5;
	motor[1] = 0x00;
	motor[2] = 0x00;
	motor[3] = 0x00;
	motor[4] = 0x00;
	motor[5] = 0x00;
	motor[6] = 0x00;
	motor[7] = 0x00;
	motor[8] = 0x00;
	motor[9] = 0x00;
	motor[10] = 0x00;
	motor[11] = 0x00;
	motor[12] = 0x00;
	print_uart3(motor);
}


/* map pid to motors 4 + ----------------------------------------------------*/
void sendPIDMotors4Plus(vs32 * PIDCorr, vu16 * receiverChannel, volatile char * motor)
{
	// no hal used at the moment
	s16 motorTemp[13];
	
	// map pitch to quax blmc values from 0-200
	u8 pitch = (receiverChannel[PITCH] - 1000) / 5;
	
	motorTemp[1] = 	pitch - PIDCorr[Y] - PIDCorr[Z];
	motorTemp[2] = 	pitch + PIDCorr[Y] - PIDCorr[Z];
	motorTemp[3] = 	pitch + PIDCorr[X] + PIDCorr[Z];
	motorTemp[4] = 	pitch - PIDCorr[X] + PIDCorr[Z];
	motorTemp[5] = 0;
	motorTemp[6] = 0;
	motorTemp[7] = 0;
	motorTemp[8] = 0;
	motorTemp[9] = 0;
	motorTemp[10] = 0;
	motorTemp[11] = 0;
	motorTemp[12] = 0;
	
	u8 i;
	for (i = 1; i < 13; i++) 
	{
		if ((motorTemp[i] - motorTempOld[i]) > 2)
		{
			motorTemp[i] = motorTempOld[i] + 2;
		}
		if ((motorTempOld[i] - motorTemp[i]) > 2)
		{
			motorTemp[i] = motorTempOld[i] - 2;
		}
		motorTempOld[i] = motorTemp[i];
	}
	
	
	
	limitMotors(motorTemp, motor);
			
	// command motors
	sendMotor(motor);
	
	//char x [80];
	//sprintf(x,"mot:%d:%d:%d:%d\r\n",min,max,receiverChannel[PITCH],pitch);
	//print_uart1(x);
}



/* map pid to motors 4 X ----------------------------------------------------*/
void sendPIDMotorsMixer(vs32 * PIDCorr, vu16 * receiverChannel, volatile char * motor)
{
	// no hal used at the moment
	s16 motorTemp[13];
	
	// map pitch to quax blmc values from 0-180 - 20 points for stability
	// does not work
	//u8 pitch = map(receiverChannel[PITCH],1000,2000,0,180);
	u8 pitch = (receiverChannel[PITCH] - 1000) / 5;
	
	u8 pitchFactor[12];
	u8 rollFactor[12];
	u8 nickFactor[12];
	u8 yawFactor[12];
	u8 i;
	u8 j;
	for (i = 0; i < 12; i++)
	{
		j = (i < 1);
		// PARA_MIXER_MOT1A = 37
		pitchFactor[i] = parameter[37 + j] >> 8;
		rollFactor[i] = parameter[37 + j] & 255;
		nickFactor[i] = parameter[38 + j] >> 8;
		yawFactor[i] = parameter[38 + j] & 255;
	
	}
	
	for (i = 0; i < 12; i++)
	{
		motorTemp[i + 1] = (pitch * pitchFactor[i] / 100) + (PIDCorr[X] * rollFactor[i] / 100) + (PIDCorr[Y] * nickFactor[i] / 100) + (PIDCorr[Y] * yawFactor[i] / 100);
	}
	
	
	limitMotors(motorTemp, motor);
			
	// command motors
	sendMotor(motor);
	
	//char x [80];
	//sprintf(x,"mot:%d:%d:%d:%d\r\n",min,max,receiverChannel[PITCH],pitch);
	//print_uart1(x);
}


/* limit motors to quax from 0-200 ------------------------------------------*/
void limitMotors(s16 * motorTemp, volatile char * motor)
{
	
	// this is for quax uart blmc !!!
	s16 min = parameter[PARA_MIN_GAS];
	s16 max = 200;
	
	u8 i;
	for (i = 1; i < 13; i++)
	{
		min = (motorTemp[i] < min) ? motorTemp[i] : min;
		max = (motorTemp[i] > max) ? motorTemp[i] : max;
	}
	
	//correkt all motors
	min = parameter[PARA_MIN_GAS] - min;
	max = 200 - max;
	
	for (i = 1; i < 13; i++)
	{
		motor[i] = motorTemp[i] + min - max;
	}
}