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

#include "serial.h"
#include "receiverspektrum.h"
#include <stdio.h>


/* Private variables UART Interrupt---------------------------------------------------------*/
extern vu32 msCount;  //statemachine
vu32 oldSpektrumMsCount = 0;
vu8 spektrumBytes[33];  // 0 ... status 1-32 ... bytes from receiver
vu8 byteCount = 1;


vu8 TxBuffer1[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer1[TxBufferSize];
vu8 TxInCounter1 = 0; 
vu8 TxOutCounter1 = 0;
vu8 RxInCounter1 = 0;
vu8 RxOutCounter1 = 0; 

vu8 TxBuffer2[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer2[RxBufferSize];
vu8 TxInCounter2 = 0; 
vu8 TxOutCounter2 = 0;
vu8 RxInCounter2 = 0;
vu8 RxOutCounter2 = 0; 

vu8 TxBuffer3[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer3[RxBufferSize];
vu8 TxInCounter3 = 0; 
vu8 TxOutCounter3 = 0;
vu8 RxInCounter3 = 0;
vu8 RxOutCounter3 = 0; 

vu8 uartmap = FALSE;


/* map uart1 to uart3 -------------------------------------------------------*/
void uartMap()
{
	// Controlloop --> statemachine() --> Timer 3
	// no controlloop
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	
	
	uartmap = TRUE;
}

/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_uart1 (const char * s)
{
	while(*s)   // Check for end of string
	{
		// this 2 lines are for direct output without interrupt
		//USART_SendData(USART1, *s++);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		TxBuffer1[TxInCounter1++] = *s++;
		
   	}
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
}

/* read byte from UART1 Buffer ----------------------------------------------*/
char read_uart1 ()
{
	return RxBuffer1[RxOutCounter1++];
}

/* is byte to read ----------------------------------------------------------*/
s8 is_read_uart1()
{
	if (RxInCounter1 != RxOutCounter1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_uart3 (volatile char * s)
{

	// does not work - we will send 0x00 also
	//while(*s)   // Check for end of string
	//u8 i;
	//for(i = 0; i < 5; i++)
	while(*s)
	{
		// this 2 lines are for direct output without interrupt
		//USART_SendData(USART1, *s++);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		//TxBuffer1[TxInCounter1++] = *s++;
		TxBuffer3[TxInCounter3++] = *s++;
		
   	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/* read byte from UART1 Buffer ----------------------------------------------*/
char read_uart2 ()
{
	return RxBuffer2[RxOutCounter2++];
}

/* is byte to read ----------------------------------------------------------*/
s8 is_read_uart2()
{
	if (RxInCounter2 != RxOutCounter2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



/* [0xD4] USART1 Interrupt */
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{		
		if (uartmap == FALSE)
		{	/* Read one byte from the receive data register */
			RxBuffer1[RxInCounter1++] = (USART_ReceiveData(USART1) & 0x7F);
		}
		// direct mapping to uart 3
		else
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART3, USART_ReceiveData(USART1));
		}
	}

	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter1 != TxOutCounter1)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART1, TxBuffer1[TxOutCounter1++]);
		} 
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
}



/* [0xD8] USART2 Interrupt */
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		u8 data = (USART_ReceiveData(USART2)); // & 0x7F);
		
		/* 	find gap
		there is a gap all 16 byte with 11ms
		2 frames are a complete set
		1.2. byte unknow
		3.4. high/low byte 10bit resolution 4bit channel
		...
		*/
		if (oldSpektrumMsCount + 7 < msCount)
		{
			// second gap - 1 frame complete
			if (byteCount == 17)
			{
				spektrumBytes[0] = SPEKTRUM_OK;
			}
			// frame not OK - new sync
			else if (byteCount == 33)
			{
				spektrumBytes[0] = SPEKTRUM_OK;
				// for debug
				//TxBuffer1[TxInCounter1++] = '\r';
				//TxBuffer1[TxInCounter1++] = '\n';
				
				// start new frame
				byteCount = 1;
			}
			else
			{
				spektrumBytes[0] = SPEKTRUM_NO;
				// for debug
				//TxBuffer1[TxInCounter1++] = '#';
				//TxBuffer1[TxInCounter1++] = '\r';
				//TxBuffer1[TxInCounter1++] = '\n';
				
				// start new frame
				byteCount = 1;
			}
		}
		
		spektrumBytes[byteCount++] = data;

		//test receiver output
		//TxBuffer1[TxInCounter1++] = spektrumBytes[0];  //data
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		
		oldSpektrumMsCount = msCount;
	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter2 != TxOutCounter2)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART2, TxBuffer2[TxOutCounter2++]);
		}
		else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
		
	}
}

	

/* [0xDC] USART3 Interrupt */
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART3_IRQHandler(void)
{
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{		
		if (uartmap == FALSE)
		{
			/* Read one byte from the receive data register */
			RxBuffer3[RxInCounter3++] = (USART_ReceiveData(USART3) & 0x7F);
		}
		// direct mapping to uart 1
		else
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART1, USART_ReceiveData(USART3));
		}
	}

	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter3 != TxOutCounter3)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART3, TxBuffer3[TxOutCounter3++]);
		}
		else
		{
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
		
	}
	
	
}









