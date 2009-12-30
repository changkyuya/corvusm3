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

vu8 TxBuffer1[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer1[TxBufferSize];
vu8 TxInCounter1 = 0; 
vu8 TxOutCounter1 = 0;
vu8 RxInCounter1 = 0;
vu8 RxOutCounter1 = 0; 

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



/* [0xD4] USART1 Interrupt */
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{		
		/* Read one byte from the receive data register */
		RxBuffer1[RxInCounter1++] = (USART_ReceiveData(USART1) & 0x7F);
	
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











