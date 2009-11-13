#include "serial.h"

#include "led.h"


/* Private variables UART Interrupt---------------------------------------------------------*/

vu8 TxBuffer1[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer1[RxBufferSize];
vu8 TxInCounter1 = 0; 
u8 TxOutCounter1 = 0;
u8 RxInCounter1 = 0;
vu8 RxOutCounter1 = 0; 

vu8 TxBuffer3[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer3[RxBufferSize];
vu8 TxInCounter3 = 0; 
u8 TxOutCounter3 = 0;
u8 RxInCounter3 = 0;
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
	
}



/* [0xD4] USART1 Interrupt */
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if (RxInCounter1 != RxOutCounter1)
		{
			RxBuffer1[RxInCounter1++] = (USART_ReceiveData(USART1) & 0x7F);
		}
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter1 != TxOutCounter1)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART1, TxBuffer1[TxOutCounter1++]);
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
		if (RxInCounter3 != RxOutCounter3)
		{
			RxBuffer3[RxInCounter3++] = (USART_ReceiveData(USART3) & 0x7F);
		}
	}

	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter3 != TxOutCounter3)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART3, TxBuffer3[TxOutCounter3++]);
		}
		
	}
	
}









