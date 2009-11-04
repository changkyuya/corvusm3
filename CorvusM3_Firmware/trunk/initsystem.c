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

/* Include ------------------------------------------------------------------*/
#include "main.h"
#include "initsystem.h"

/* Main Initfunction --------------------------------------------------------*/
void initSystem()
{
	/* Configure the system clocks */
	RCC_Configuration();
	/* Configure TIMs */
	TIM_Configuration();
	/* Configure the GPIO ports UART */
	GPIO_Configuration();
	/* UART Interrupt */
	NVIC_Configuration();
	initUART1();
}

/* Configures the different system clocks  ----------------------------------*/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	
	/* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
    initialize the PLL and update the SystemFrequency variable. */
	//SystemInit();  
	
	/* RCC system reset(for debug purpose) */
	//RCC_DeInit();
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
    
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{	
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		 
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

	/* Enable TIM2 clocks  for LED */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // Timer
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LED, ENABLE); // GPIOc
	
	/* Enable GPIOx and AFIO clocks UART*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx | RCC_APB2Periph_AFIO, ENABLE);
	/* Enable USART1 clocks UART */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
}

/* Configures the used Timers. ----------------------------------------------*/
void TIM_Configuration(void)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* TIM2 configuration for LED -------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Period = 0x12B; //alle 250ms = 299
	TIM_TimeBaseStructure.TIM_Prescaler = 0xEA5F;       	//59999
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* Outoput Compare Init */
	TIM_OCStructInit(&TIM_OCInitStructure);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_Pulse = 0x0;  
	TIM_OCInit(TIM2, &TIM_OCInitStructure);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
	/* Immediate load of TIM2 Precaler value */
	TIM_PrescalerConfig(TIM2, 0xEA5F, TIM_PSCReloadMode_Immediate);
	/* Clear TIM2 update pending flag */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	/* Enable TIM2 Update interrupt */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/* Configures the different GPIO ports.  ------------------------------------*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* UART1 for main communication -----------------------------------------*/
	/* Configure USART1 RTS and USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_RTSPin | GPIO_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIO_SER, &GPIO_InitStructure);  //GPIO_SER = GPIOA 
	/* Configure USART1 CTS and USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_CTSPin | GPIO_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_SER, &GPIO_InitStructure);  //GPIO_SER = GPIOA 
	
	/* Status LED -----------------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_LEDPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  //GPIO_LED = GPIOC	
	
	/* ADC for ACC and Gyro -------------------------------------------------*/
	// 16 & 17 no need GPIO_Mode_AIN
	/* Gyro Z */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_GYRO_Z;
	GPIO_Init(GPIO_GYRO, &GPIO_InitStructure); 
	/* Gyro X */
	GPIO_InitStructure.GPIO_Pin = GPIO_GYRO_X;
	GPIO_Init(GPIO_GYRO, &GPIO_InitStructure); 
	/* Gyro Y */
	GPIO_InitStructure.GPIO_Pin = GPIO_GYRO_Y;
	GPIO_Init(GPIO_GYRO, &GPIO_InitStructure); 
	/* ACC X */
	GPIO_InitStructure.GPIO_Pin = GPIO_ACC_X;
	GPIO_Init(GPIO_ACC, &GPIO_InitStructure); 
	/* ACC Y */
	GPIO_InitStructure.GPIO_Pin = GPIO_ACC_Y;
	GPIO_Init(GPIO_ACC, &GPIO_InitStructure); 
	/* ACC Z */
	GPIO_InitStructure.GPIO_Pin = GPIO_ACC_Z;
	GPIO_Init(GPIO_ACC, &GPIO_InitStructure); 

}

/* Configures the nested vectored interrupt controller. ---------------------*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USART1 Interrupt --------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
  	/* Interrupt for Status LED ---------------------------------------------*/
	/* Configure two bits for preemption priority - LED*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the TIM2 Interrupt - LED */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/* Init UART1 --------------------------------------------------------------------*/
void initUART1 (void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART1 configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
 
	/* Enable the EVAL_COM1 Transmoit interrupt: this interrupt is generated when the 
    EVAL_COM1 transmit data register is empty */  
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	/* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the 
    EVAL_COM1 receive data register is not empty */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
}

