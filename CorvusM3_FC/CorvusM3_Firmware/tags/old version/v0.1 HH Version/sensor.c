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
#include "i2c.h"


/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
extern vu16 ADCSensorValue[56];  //initsystem
vu32 overADCSensorValue[7];
//vu32 oldGyroValues[3]; // for smooth
vu32 oldAccValues[3]; // for smooth
vu32 gyroZero[3];
vu32 accZero[3];
extern vu16 parameter[0x190]; //parameter
extern vs32 compassAngle; //statemachine
extern vu32 msCount; //statemachine -> for compass all 20ms
s16 compassout [3];
extern vs32 copterAngle[3]; //statemachine

extern vu8 i2cDirection; //i2c

extern vu8 i2cReadBuffer[6]; //i2c
extern vu8 i2cReadIdx; //i2c
extern vu8 i2cToRead; //i2c

extern vu8 i2cWriteBuffer[6]; //i2c
extern vu8 i2cWriteIdx; //i2c
extern vu8 i2cToWrite; //i2c


/* initGyros to set Baise ---------------------------------------------------*/
void zeroGyro()
{
	//TODO
	oversamplingADC();
	
	gyroZero[X] = 0;
	gyroZero[Y] = 0;
	gyroZero[Z] = 0;
	
	u16 i;
	for (i = 0; i < 1024; i++)
	{	
		gyroZero[X] += overADCSensorValue[GYRO_X];
		gyroZero[Y] += overADCSensorValue[GYRO_Y];
		gyroZero[Z] += overADCSensorValue[GYRO_Z];
		
		Pause(1);
	}
	
	gyroZero[X] = gyroZero[X] >> 10;
	gyroZero[Y] = gyroZero[Y] >> 10;
	gyroZero[Z] = gyroZero[Z] >> 10;

	char x [80];
	sprintf(x,"Gyro-Zero:%d:%d:%d:\r\n",gyroZero[X],gyroZero[Y],gyroZero[Z]);
	print_uart1(x);
}




/* Set ACC Values to Zero ---------------------------------------------------*/
void zeroACC()
{
	//TODO
	oversamplingADC();	

	accZero[X] = 0;
	accZero[Y] = 0;
	accZero[Z] = 0;
	
	u16 i;
	for (i = 0; i < 1024; i++)
	{	
		accZero[X] += overADCSensorValue[ACC_X];
		accZero[Y] += overADCSensorValue[ACC_Y];
		accZero[Z] += overADCSensorValue[ACC_Z];
		
		Pause(1);
	}
	
	accZero[X] = accZero[X] >> 10;
	accZero[Y] = accZero[Y] >> 10;
	accZero[Z] = accZero[Z] >> 10;
	
	
	accZero[X] = accZero[X] / 100;
	accZero[Y] = accZero[Y] / 100;
	accZero[Z] = (accZero[X] + accZero[Y]) >> 1;
	
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
	//TODO
	oversamplingADC();
	
	//char x [200];
	//sprintf(x,"%d:%d:%d:%d:%d:%d:%d:%d\r\n",ADCSensorValue[0],ADCSensorValue[7],ADCSensorValue[14],ADCSensorValue[21],ADCSensorValue[28],ADCSensorValue[35],ADCSensorValue[42],ADCSensorValue[49]);
	//print_uart1(x);
	
	// no gyro smooth
	gyroValues[X] = overADCSensorValue[GYRO_X] - gyroZero[X];
	gyroValues[Y] = overADCSensorValue[GYRO_Y] - gyroZero[Y];
	gyroValues[Z] = overADCSensorValue[GYRO_Z] - gyroZero[Z];
	//gyroValues[Z] = smoothValue(ADCSensorValue[GYRO_Z] * 1000, oldGyroValues[Z], parameter[PARA_SMOOTH_GYRO]) - gyroZero[Z];
	
	accValues[X] = smoothValue(overADCSensorValue[ACC_X], oldAccValues[X], parameter[PARA_SMOOTH_ACC]);
	accValues[Y] = smoothValue(overADCSensorValue[ACC_Y], oldAccValues[Y], parameter[PARA_SMOOTH_ACC]);
	accValues[Z] = smoothValue(overADCSensorValue[ACC_Z], oldAccValues[Z], parameter[PARA_SMOOTH_ACC]);
	
	u8 i;
	for (i = 0; i < 3; i++)
	{
		//oldGyroValues[i] = gyroValues[i];
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


	// Put the HMC5843 into continuous mode address:0x02 value:0x00
	i2cDirection = I2C_TRANSMITTER;
	i2cWriteBuffer[0] = 0x02;
	i2cWriteBuffer[1] = 0x00;
	i2cWriteIdx = 0;
	i2cToWrite = 2;
	/* Enable I2C1 event and buffer interrupts */
	I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	Pause(100);
	
	// Put the HMC5843 into 50HZ mode	address:0x00, value:0x18  (0x10 = 10HZ)
	i2cDirection = I2C_TRANSMITTER;
	i2cWriteBuffer[0] = 0x00;
	i2cWriteBuffer[1] = 0x18;
	i2cWriteIdx = 0;
	i2cToWrite = 2;
	/* Enable I2C1 event and buffer interrupts */
	I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	
	// note that you need to wait 100ms after this before first calling recieve
	Pause(200);
	
	getCompassAngle();	
}

/* return the compass angel -------------------------------------------------*/
void getCompassAngle()
{
	
	// we only get new values all 300ms
	// we try to split in parts
	if (msCount % 300 == 0)
	{
		//we start at address 0x03 and read 6 values
		i2cDirection = I2C_TRANSMITTER;
		i2cWriteBuffer[0] = 0x03;
		i2cWriteIdx = 0;
		i2cToWrite = 1;
		/* Enable I2C1 event and buffer interrupts */
		I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
		/* Send I2C1 START condition */
		I2C_GenerateSTART(I2C1, ENABLE);
	}
	
	if ((msCount + 100) % 300 == 0)
	{
		
		// values to read
		i2cDirection = I2C_RECEIVER;
		i2cReadIdx = 0;
		i2cToRead = 6;
		/* Enable I2C1 event and buffer interrupts */
		I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
		/* Send I2C1 START condition */
		I2C_GenerateSTART(I2C1, ENABLE);
		
	}
	
	// at offset we calculate degree from compass
	if ((msCount + 200) % 300 == 0)
	{
		for(int i=0; i<3; i++)
		{
			compassout[i]=((s16)((u16)i2cReadBuffer[2*i] << 8) + i2cReadBuffer[2*i+1]);
			// correct calibration
			compassout[i] += parameter[PARA_COMP_CORR_X + i];
		}
	  
		// angle compensation
		
		// X’ = X cos nick + Y sin roll sin nick – Z cos roll sin nick
		// Y’ = Y cos roll + Z sin roll
		
		// X and Y from compass are different to copterAngle X/Y !!!
		//compassout[0] = compassout[0] * cos(copterAngle[X]/100000.0-90) + compassout[1] * sin(copterAngle[Y]/100000.0-90) * sin(copterAngle[X]/100000.0-90) - compassout[2] * cos(copterAngle[Y]/100000.0-90) * sin(copterAngle[X]/100000.0-90);
		//compassout[1] = compassout[1] * cos(copterAngle[Y]/100000.0-90) + compassout[2] * sin(copterAngle[Y]/100000.0-90);
		
		s32 compval = (s32) (atan2(compassout[1], compassout[0]) * 5729577.95);  
	  
		if(compval < 0)
		{
			compval += 36000000;
		}
		// correct declination
		compassAngle = compval + (parameter[PARA_COMP_DECL] * 1000);
	  
		// for test
		//char x [80];
		//sprintf(x,"Kompass Test:%d:%d:%d:%d:%d:%d\r\n",i2cReadBuffer[0],i2cReadBuffer[1],i2cReadBuffer[2],i2cReadBuffer[3],i2cReadBuffer[4],i2cReadBuffer[5]);
		//print_uart1(x);
		//sprintf(x,"Kompass Test - :%d:%d:%d = %d\r\n",compassout[0],compassout[1],compassout[2],compval);
		//print_uart1(x);
	}
	
	
	
}

	