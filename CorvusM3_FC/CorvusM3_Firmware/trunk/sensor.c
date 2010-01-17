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

#include "sensor.h"
#include "parameter.h"
//for debug
#include "serial.h"
#include <stdio.h>
#include <math.h>


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[56];  //initsystem
vu32 overADCSensorValue[7];
vu32 oldGyroValues[3]; // for smooth
vu32 oldAccValues[3]; // for smooth
vu32 gyroZero[3];
vu32 accZero[3];
extern vu16 parameter[0x190]; //parameter
extern vs32 compassAngle; //statemachine
extern vu32 msCount; //statemachine -> for compass all 20ms
s16 compassout [3];
extern vs32 copterAngle[3]; //statemachine
u8 comp1[6];
u8 compCount = 0;
s32 compval;

/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	oversamplingADC();
	
	gyroZero[X] = overADCSensorValue[GYRO_X];
	gyroZero[Y] = overADCSensorValue[GYRO_Y];
	gyroZero[Z] = overADCSensorValue[GYRO_Z];
	
	Pause(50);
	oversamplingADC();
	
	gyroZero[X] += overADCSensorValue[GYRO_X];
	gyroZero[Y] += overADCSensorValue[GYRO_Y];
	gyroZero[Z] += overADCSensorValue[GYRO_Z];
	
	gyroZero[X] = gyroZero[X] >> 1;
	gyroZero[Y] = gyroZero[Y] >> 1;
	gyroZero[Z] = gyroZero[Z] >> 1;

	char x [80];
	sprintf(x,"Gyro-Zero:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}




/* Set ACC Values to Zero ---------------------------------------------------*/
void zeroACC()
{
	vs32 gyroValues[3];
	vs32 accValues[3];
	getRawValues(gyroValues, accValues);
	
	accZero[X] = accValues[X] / 100;
	accZero[Y] = accValues[Y] / 100;
	accZero[Z] = ((accValues[X] / 100) + (accValues[Y] / 100)) >> 1;
	
	setParameter(PARA_ACC_X_ZERO, accZero[X]);
	setParameter(PARA_ACC_Y_ZERO, accZero[Y]);
	setParameter(PARA_ACC_Z_ZERO, accZero[Z]);
	
	char x [80];
	sprintf(x,"ACC-Zero:%d:%d:%d:\r\n",accZero[X],accZero[Y],accZero[Z]);
	print_uart1(x);
}


/* get RawValues - calculate Gyrovalues and Baise --------------------------*/
void getRawValues(vs32 * gyroValues, vs32 * accValues)
{
	
	oversamplingADC();
	
	//overADCSensorValue[0] = (ADCSensorValue[0]+ADCSensorValue[7]+ADCSensorValue[14]+ADCSensorValue[21]+ADCSensorValue[28]+ADCSensorValue[35]+ADCSensorValue[42]+ADCSensorValue[49])>>3;
	//char x [200];
	//sprintf(x,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",ADCSensorValue[0],ADCSensorValue[7],ADCSensorValue[14],ADCSensorValue[21],ADCSensorValue[28],ADCSensorValue[35],ADCSensorValue[42],ADCSensorValue[49]);
	//print_uart1(x);
	
	gyroValues[X] = smoothValue(overADCSensorValue[GYRO_X], oldGyroValues[X], parameter[PARA_SMOOTH_GYRO]) - gyroZero[X];
	gyroValues[Y] = smoothValue(overADCSensorValue[GYRO_Y], oldGyroValues[Y], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Y];
	gyroValues[Z] = smoothValue(overADCSensorValue[GYRO_Z], oldGyroValues[Z], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Z];
	
	accValues[X] = smoothValue(overADCSensorValue[ACC_X], oldAccValues[X], parameter[PARA_SMOOTH_ACC]);
	accValues[Y] = smoothValue(overADCSensorValue[ACC_Y], oldAccValues[Y], parameter[PARA_SMOOTH_ACC]);
	accValues[Z] = smoothValue(overADCSensorValue[ACC_Z], oldAccValues[Z], parameter[PARA_SMOOTH_ACC]);
	
	u8 i;
	for (i = 0; i < 3; i++)
	{
		oldGyroValues[i] = gyroValues[i];
		oldAccValues[i] = accValues[i];
	}
}


/* ADC oversampling ---------------------------------------------------------*/
void oversamplingADC()
{
	// 8 time oversampling
	u8 i;
	u8 j = 0;
	
	for (i = 0; i < 7; i++)
	{
		overADCSensorValue[i] = ADCSensorValue[i];
	}
	for (i = 7; i < 56; i++)
	{
		overADCSensorValue[j++] += ADCSensorValue[i];
		if (j > 6)
		{
			j = 0;
		}
	}
	for (i = 0; i < 7; i++)
	{
		overADCSensorValue[i] = (overADCSensorValue[i] * 1000 ) >> 3;
	}
}


/* setup compass hmc5843 ----------------------------------------------------*/
void initCompass()
{
	/* setup compass for countinuos mode and 50 Hz*/
	
	// note that you need to wait at least 5ms after power on to initialize
	Pause(10);


	// Put the HMC5843 into continuous mode 0x02;0x00
	writeI2C(0x02, 0x00);
	
	// for test
	//u8 test[1];
	//char x [80];
	//getI2C(test, 0x02, 0x01);
	//sprintf(x,"Compass Register Mode set:%d\r\n",test[0]);
	//print_uart1(x);
	
	// Put the HMC5843 into 50HZ mode	0x00;0x18  (0x10 = 10HZ)
	writeI2C(0x00, 0x18);
	//getI2C(test, 0x00, 0x01);
	//sprintf(x,"Compass Register A set:%d\r\n",test[0]);
	//print_uart1(x);
	
	// note that you need to wait 100ms after this before first calling recieve
	Pause(200);
	
	getCompassAngle();
}

/* return the compass angel -------------------------------------------------*/
void getCompassAngle()
{
	
	// we only get new values all 20ms
	// we try to split in parts
	if (msCount % 20 == 0)
	{
		//we start
		compCount = 1;
	}
	
	switch (compCount)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			getI2C(&comp1[compCount-1], (compCount + 2), 0x01);
			compCount++;
			break;
		case 7:
		case 8:
			compassout[compCount-7]=((s16)((u16)comp1[2*(compCount-7)] << 8) + comp1[2*(compCount-7)+1]);
			// correct calibration
			compassout[compCount-7] += parameter[PARA_COMP_CORR_X + (compCount-7)];
			compCount++;
			break;
		case 9:
			// angle compesnation
			compCount++;
			break;
		case 10:
			// angle compesnation
			compCount++;
			break;
		case 11:
			// calc angle
			compval = (s32) (atan2(compassout[1], compassout[0]) * 5729577.95);
			compCount++;
			break;
		case 12:		
			if(compval < 0)
			{
				compval += 36000000;
			}
			compCount++;
			break;
		case 13:
			compassAngle = compval + (parameter[PARA_COMP_DECL] * 1000);
			compCount++;
			break;
		default:
			break;
		
		
	}
	/*
	if (msCount % 20 == 0)
	{
		
		// read 6 values from register 03
		getI2C(comp1, 0x03, 0x06);
		
		for(int i=0; i<3; i++)
		{
			compassout[i]=((s16)((u16)comp1[2*i] << 8) + comp1[2*i+1]);
			// correct calibration
			compassout[i] += parameter[PARA_COMP_CORR_X + i];
		}
	  
		// angle compensation
		
		// X’ = X cos nick + Y sin roll sin nick – Z cos roll sin nick
		// Y’ = Y cos roll + Z sin roll
		
		// X and Y from compass are different to copterAngle X/Y !!!
		//compassout[0] = compassout[0] * cos(copterAngle[X]/100000.0-90) + compassout[1] * sin(copterAngle[Y]/100000.0-90) * sin(copterAngle[X]/100000.0-90) - compassout[2] * cos(copterAngle[Y]/100000.0-90) * sin(copterAngle[X]/100000.0-90);
		//compassout[1] = compassout[1] * cos(copterAngle[Y]/100000.0-90) + compassout[2] * sin(copterAngle[Y]/100000.0-90);
		
		compval = (s32) (atan2(compassout[1], compassout[0]) * 5729577.95);  
	  
		if(compval < 0)
		{
			compval += 36000000;
		}
		// correct declination
		compassAngle = compval + (parameter[PARA_COMP_DECL] * 1000);
	  
		// for test
		//char x [80];
		//sprintf(x,"Kompass Test:%d:%d:%d:%d:%d:%d\r\n",comp1[0],comp1[1],comp1[2],comp1[3],comp1[4],comp1[5]);
		//print_uart1(x);
		//sprintf(x,"Kompass Test - :%d:%d:%d = %d\r\n",out[0],out[1],out[2],compval);
		//print_uart1(x);
		
	}*/
}

/* read I2C buffer from HMC5843 ---------------------------------------------*/
void getI2C(u8* pBuffer, u8 ReadAddr, u8 NumByteToRead)
{
	
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	/* Send STRAT condition */ 
	I2C_GenerateSTART(I2C1, ENABLE); 

	/* Test on EV5 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	/* Send address for read */ 
	I2C_Send7bitAddress(I2C1, HMC5843_ADDRESS, I2C_Direction_Transmitter); 

	/* Test on EV6 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);

	/* Send the HMC5843's internal address to read to */
	I2C_SendData(I2C1, ReadAddr);  

	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send HMC5843 address for read */
	I2C_Send7bitAddress(I2C1, HMC5843_ADDRESS, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
		  /* Disable Acknowledgement */
		  I2C_AcknowledgeConfig(I2C1, DISABLE);
		  
		  /* Send STOP Condition */
		  I2C_GenerateSTOP(I2C1, ENABLE);
		}
		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			/* Read a byte from the HMC5843 */
			*pBuffer = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			pBuffer++; 

			/* Decrement the read bytes counter */
			NumByteToRead--;        
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
		
}


/* write to HMC -------------------------------------------------------------*/
void writeI2C(u8 WriteAddr, u8 pBuffer)
{
	/* Send STRAT condition */ 
	I2C_GenerateSTART(I2C1, ENABLE); 

	/* Test on EV5 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	/* Send address for write */ 
	I2C_Send7bitAddress(I2C1, HMC5843_ADDRESS, I2C_Direction_Transmitter); 

	/* Test on EV6 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 

	// Put the HMC5843 into continuous mode
	/* Send the internal register address to write to */ 
	I2C_SendData(I2C1, WriteAddr); 

	/* Test on EV8 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

	/* Send the byte to be written */ 
	I2C_SendData(I2C1, pBuffer); 

	/* Test on EV8 and clear it */ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); 

	/* Send STOP condition */ 
	I2C_GenerateSTOP(I2C1, ENABLE); 
}