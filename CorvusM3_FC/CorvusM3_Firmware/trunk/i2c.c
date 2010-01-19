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

#include "i2c.h"
#include "serial.h"

/* Enums --------------------------------------------------------------------*/

/* Variables ----------------------------------------------------------------*/
vu8 i2cDirection;

vu8 i2cReadBuffer[6];
vu8 i2cReadIdx;
vu8 i2cToRead;

vu8 i2cWriteBuffer[6];
vu8 i2cWriteIdx;
vu8 i2cToWrite;



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



/* read I2C buffer from HMC5843 ---------------------------------------------*/
void getI2C(u8* pBuffer, u8 ReadAddrN, u8 NumByteToReadN)
{
	
	i2cDirection = I2C_TRANSMITTER; // = 1
	
	/* While the bus is busy - we have only one device */
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
	I2C_SendData(I2C1, ReadAddrN);  

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
	while(NumByteToReadN)  
	{
		if(NumByteToReadN == 1)
		{
		  // Disable Acknowledgement 
		  I2C_AcknowledgeConfig(I2C1, DISABLE);
		  
		  // Send STOP Condition 
		  I2C_GenerateSTOP(I2C1, ENABLE);
		}
		// Test on EV7 and clear it 
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			// Read a byte from the HMC5843 
			*pBuffer = I2C_ReceiveData(I2C1);

			// Point to the next location where the byte read will be saved 
			pBuffer++; 

			// Decrement the read bytes counter 
			NumByteToReadN--;        
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
		
}



/* Interrupt Handler for I2C Compass ----------------------------------------*/
/* [0xBC] I2C1 Event Interrupt ----------------------------------------------*/
void I2C1_EV_IRQHandler(void)
{
	switch (I2C_GetLastEvent(I2C1))
	{
		case I2C_EVENT_MASTER_MODE_SELECT:                 /* EV5 */
			if(i2cDirection == I2C_TRANSMITTER)
			{
				print_uart1("a");
				/* Master Transmitter ----------------------------------------------*/
				/* Send slave Address for write */
				I2C_Send7bitAddress(I2C1, HMC5843_ADDRESS, I2C_Direction_Transmitter);
			}
			else
			{
				print_uart1("b");
				/* Master Receiver -------------------------------------------------*/
				/* Send slave Address for read */
				I2C_Send7bitAddress(I2C1, HMC5843_READ_ADDRESS, I2C_Direction_Receiver);
			}
			break;
		
		/* Test on I2C1 EV6 and first EV8 and clear them */
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: 
			print_uart1("c");
			/* Clear EV6 by setting again the PE bit */
			//I2C_Cmd(I2C1, ENABLE);
			/* Send the HMC5843's internal address to read to */
			I2C_SendData(I2C1, i2cWriteBuffer[i2cWriteIdx++]);  
			break;
		
		/* Test on I2C1 EV8 and clear it */
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:  /* Without BTF, EV8 */     
			if(i2cWriteIdx < (i2cToWrite))
			{
				print_uart1("d");
				/* Transmit I2C1 data */
				I2C_SendData(I2C1, i2cWriteBuffer[i2cWriteIdx++]);

			}
			else
			{
				print_uart1("e");
				//I2C_TransmitPEC(I2C1, ENABLE); 
				/* Disable I2C1 event and buffer interrupts */
				I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				
			}            
			break;
	
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED: /* With BTF EV8-2 */
			print_uart1("f");
			I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
			/* I2C1 Re-START Condition */
			I2C_GenerateSTART(I2C1, ENABLE);
			break;
			
		/* Master Receiver -------------------------------------------------------*/
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
			if(i2cToRead == 1)
			{
				print_uart1("g");
				/* Disable I2C1 acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);
				/* Send I2C1 STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
			break;

		/* Test on I2C1 EV7 and clear it */
		case I2C_EVENT_MASTER_BYTE_RECEIVED:
			print_uart1("h");
			/* Store I2C1 received data */
			i2cReadBuffer[i2cReadIdx++] = I2C_ReceiveData (I2C1);
				// for test
				//char x [80];
				//sprintf(x,"%d-%d#",NumByteToRead,compBuffer[6 - NumByteToRead]);
				//print_uart1(x);
			
			/* Disable ACK and send I2C1 STOP condition before receiving the last data */
			if(i2cReadIdx == (i2cToRead - 1))
			{
				print_uart1("i");
				/* Disable I2C1 acknowledgement */
				I2C_AcknowledgeConfig(I2C1, DISABLE);
				/* Send I2C1 STOP Condition */
				I2C_GenerateSTOP(I2C1, ENABLE);
			}
			
			
			/* Disable interrupt */
			if(i2cReadIdx == i2cToRead)
			{
				print_uart1("j");
				/* Disable I2C1 event and buffer interrupts */
				I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
				/* Enable Acknowledgement to be ready for another reception */
				I2C_AcknowledgeConfig(I2C1, ENABLE);
			}
			
			break;

		default:
			
			//I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
			break;
	}
}


/* [0xC0] I2C1 Error Interrupt */
void I2C1_ER_IRQHandler(void)
{
	/* Check on I2C1 AF flag and clear it */
	if (I2C_GetITStatus(I2C1, I2C_IT_AF)) 
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
	}
}
