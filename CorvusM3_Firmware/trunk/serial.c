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

u8 TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";
u8 RxBuffer[RxBufferSize];
u8 NbrOfDataToTransfer = TxBufferSize;
u8 NbrOfDataToRead = RxBufferSize;
u8 TxCounter = 0; 
u16 RxCounter = 0; 



void print_uart1 (char * s)
{

	while(*s)   // Check for end of string
	{
      USART_SendData(USART1, *s++);
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
   	}
	
}


/* [0xD4] USART1 Interrupt */
void USART1_IRQHandler(void)
{
	//DEFAULT_EXCEPTION_HANDLER(USART1_IRQHandler, "USART1", 53, 0xD4);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		RxBuffer[RxCounter++] = (USART_ReceiveData(USART1) & 0x7F);

		if(RxCounter == NbrOfDataToRead)
		{
		  /* Disable the EVAL_COM1 Receive interrupt */
		  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		}
		
		// Try to send byte to register --> only for test
		TxBuffer[0] = RxBuffer[RxCounter-1];
		TxCounter = 0;
		
		// test LED
		if (TxBuffer[0] == '0')
		{
			setLEDStatus(LED_OFF);
		}
		
		if (TxBuffer[0] == '1')
		{
			setLEDStatus(LED_FLASH);
		}
		
		if (TxBuffer[0] == '2')
		{
			setLEDStatus(LED_BLINK);
		}
		
		if (TxBuffer[0] == '3')
		{
			setLEDStatus(LED_ON);
		}
		
		
		//NbrOfDataToTransfer = TxBufferSize;
		NbrOfDataToTransfer = 1;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		/* Write one byte to the transmit data register */
		USART_SendData(USART1, TxBuffer[TxCounter++]);

		if(TxCounter == NbrOfDataToTransfer)
		{
		  /* Disable the EVAL_COM1 Transmit interrupt */
		  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
  
  
}










