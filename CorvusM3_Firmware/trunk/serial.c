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

#include "serial.h"
#include "receiverspektrum.h"


/* Private variables UART Interrupt---------------------------------------------------------*/

vu8 TxBuffer1[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer1[RxBufferSize];
vu8 TxInCounter1 = 0; 
vu8 TxOutCounter1 = 0;
vu8 RxInCounter1 = 0;
vu8 RxOutCounter1 = 0; 

vu8 TxBuffer3[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer3[RxBufferSize];
vu8 TxInCounter3 = 0; 
vu8 TxOutCounter3 = 0;
vu8 RxInCounter3 = 0;
vu8 RxOutCounter3 = 0; 


/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_uart1 (char * s)
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
/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_uart3 (char * s)
{

	while(*s)   // Check for end of string
	{
		// this 2 lines are for direct output without interrupt
		//USART_SendData(USART1, *s++);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		TxBuffer3[TxInCounter3++] = *s++;
   	}
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
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



/* [0xDC] USART3 Interrupt */
/* Used for Spektrum RX ! ---------------------------------------------------*/
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART3_IRQHandler(void)
{
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* write byte to receive buffer */
		RxBuffer3[RxInCounter3++] = (USART_ReceiveData(USART3) & 0x7F);
		getSpektrumChannels_IT();  //receiverspektrum.c
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









