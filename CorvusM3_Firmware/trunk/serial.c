#include "serial.h"

#include "led.h"


/* Private variables for UART  Hardware flow Controll---------------------------------------------------------
USART_InitTypeDef USART_InitStructure;
u8 TxBuffer[] = "\n\rUSART Hyperterminal Hardware Flow Control Example: USART\n\r";
u8 RxBuffer[RxBufferSize];
u8 NbrOfDataToTransfer = TxBufferSize;
u8 TxCounter = 0;
u8 RxCounter = 0;*/





/* Private variables UART Interrupt---------------------------------------------------------*/

vu8 TxBuffer[TxBufferSize]; // = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal communication using Interrupt\n\r";
vu8 RxBuffer[RxBufferSize];
vu8 TxInCounter = 0; 
u8 TxOutCounter = 0;
u8 RxInCounter = 0;
vu8 RxOutCounter = 0; 


/* send byte array over TxBuffer and Interrupt ------------------------------*/
void print_uart1 (char * s)
{

	while(*s)   // Check for end of string
	{
		// this 2 lines are for direct output without interrupt
		//USART_SendData(USART1, *s++);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		TxBuffer[TxInCounter++] = *s++;
   	}
	
}



/* [0xD4] USART1 Interrupt */
/* Fill RX/TX Buffer --------------------------------------------------------*/
void USART1_IRQHandler(void)
{
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if (RxInCounter != RxOutCounter)
		{
			RxBuffer[RxInCounter++] = (USART_ReceiveData(USART1) & 0x7F);
		}
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		if (TxInCounter != TxOutCounter)
		{
			/* Write one byte to the transmit data register */
			USART_SendData(USART1, TxBuffer[TxOutCounter++]);
		}
		
	}
  
  
}










