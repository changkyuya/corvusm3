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

/* Include ------------------------------------------------------------------*/
#include "main.h"
#include "initsystem.h"

/* Variables ----------------------------------------------------------------*/
vu16 ADCDMASensorValue[56];

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
	initUART2();
	initUART3();
	//initUART4();
	/* initDMA for Sensors */
	initDMA();
	/* initADC for Sensros */
	initADC(); 
	/* inti System Ticker */
	/* does not work in Interrupt */
	/* Pause(ms) function */
	initSysTick();
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

		/* ADCCLK = PCLK2/4 */
		RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72MHz/6=12 MHz range must be 0.6-14MHz;

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

	/* Enable TIM2 clocks for LED, 
	   Enable TIM3 clocks for Statemachine  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);  // Timer
	/* Enable for PPM decode -------------------------------------------*/
	/* TIM1 clock enable 
	Enable TIM8 clocks for Servo */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8, ENABLE);
	
	/* Enable GPIO clocks A(UART1),B(UART3),C(LED, Sensors), ...*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* AFIO and USART1 clocks UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	/* UART3 clock (Motor/BLC) */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_UART4, ENABLE);
	
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);
	

}

/* Configures the used Timers. ----------------------------------------------*/
void TIM_Configuration(void)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure2;
	TIM_OCInitTypeDef  TIM_OCInitStructure3;
	TIM_OCInitTypeDef  TIM_OCInitStructure8;

	/* TIM2 configuration for LED -------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Period = 0x12B; 				//299
	TIM_TimeBaseStructure.TIM_Prescaler = 0xEA5F;       	//59999
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* Outoput Compare Init */
	TIM_OCStructInit(&TIM_OCInitStructure2);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure2.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure2.TIM_Pulse = 0x0;  
	TIM_OCInit(TIM2, &TIM_OCInitStructure2);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
	/* Immediate load of TIM2 Precaler value */
	TIM_PrescalerConfig(TIM2, 0xEA5F, TIM_PSCReloadMode_Immediate);
	/* Clear TIM2 update pending flag */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	/* Enable TIM2 Update interrupt */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM3 configuration for Statemachine (main control loop) --------------*/
	TIM_TimeBaseStructure.TIM_Period = 1000; 				//alle 1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 0x48;       	//72
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* Outoput Compare Init */
	TIM_OCStructInit(&TIM_OCInitStructure3);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure3.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure3.TIM_Pulse = 0x0;  
	TIM_OCInit(TIM3, &TIM_OCInitStructure3);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	/* Immediate load of TIM3 Precaler value */
	TIM_PrescalerConfig(TIM3, 0x48, TIM_PSCReloadMode_Immediate);
	/* Clear TIM3 update pending flag */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	/* Enable TIM3 Update interrupt */
	// enable in main after powerup
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	
	/* Timer for PPM decode ---------------------------------------------------*/
	TIM1_ICInitTypeDef TIM1_ICInitStructure; 
	//TIM_ICInitStructure.TIM_ICMode = TIM_ICMode_ICAP; 
	TIM1_ICInitStructure.TIM1_Channel = TIM1_Channel_1;   //Pin: PA8
	TIM1_ICInitStructure.TIM1_ICPolarity = TIM1_ICPolarity_Falling; 
	TIM1_ICInitStructure.TIM1_ICSelection = TIM1_ICSelection_DirectTI; 
	TIM1_ICInitStructure.TIM1_ICPrescaler = TIM1_ICPSC_DIV1; 
	TIM1_ICInitStructure.TIM1_ICFilter = 0x0; 
	TIM1_ICInit(&TIM1_ICInitStructure); 

	TIM1_SelectInputTrigger(TIM1_TS_TI2FP2); 

	TIM1_InternalClockConfig(); 
	TIM1_TimeBaseInitTypeDef TIM1_TimeBaseStructure; 
	TIM1_TimeBaseStructure.TIM1_Period = 0xFFFF; 
	TIM1_TimeBaseStructure.TIM1_Prescaler = 17; // fCK_PSC / (17 + 1) 1ms = 4000 
	TIM1_TimeBaseStructure.TIM1_ClockDivision = TIM1_CKD_DIV1; 
	TIM1_TimeBaseStructure.TIM1_CounterMode = TIM1_CounterMode_Up; 
	TIM1_TimeBaseInit(& TIM1_TimeBaseStructure);

	/* TIM enable counter */ 
	TIM1_Cmd(ENABLE); 
	/* Enable the CC1 Interrupt Request */ 
	TIM1_ITConfig(TIM_IT_CC1, ENABLE); 
	
	
	/* Timer for Servo 0-3 --------------------------------------------------*/
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 5999;
	TIM_TimeBaseStructure.TIM_Prescaler = 72;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure8.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure8.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_1;   
	TIM_OCInitStructure8.TIM_Pulse = 1500;
	TIM_OCInitStructure8.TIM_OCPolarity = TIM_OCPolarity_High; // maybee it works with TIM_OCPolarity_LOW???

	TIM_OCInit(TIM8, &TIM_OCInitStructure8);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure8.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_2;   
	TIM_OCInitStructure8.TIM_Pulse = 1500;

	TIM_OCInit(TIM8, &TIM_OCInitStructure8);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure8.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_3;   
	TIM_OCInitStructure8.TIM_Pulse = 1500;

	TIM_OCInit(TIM8, &TIM_OCInitStructure8);

	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure8.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_4;   
	TIM_OCInitStructure8.TIM_Pulse = 1500;

	TIM_OCInit(TIM8, &TIM_OCInitStructure8);

	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);

	/* Enable the TIM Main Output only needed for timer 1 and 8*/
    TIM8->BDTR |= ((u16)0x8000); //BDTR_MOE_Set;
	
	/* TIM8 enable counter */
	TIM_Cmd(TIM8, ENABLE);
	/* Immediate load of TIM8 Precaler value */
	//TIM_PrescalerConfig(TIM8, 72, TIM_PSCReloadMode_Immediate);
	/* Clear TIM8 update pending flag */
	//TIM_ClearFlag(TIM8, TIM_FLAG_Update);
}

/* Configures the different GPIO ports.  ------------------------------------*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* UART1 for main communication -----------------------------------------*/
	/* Configure USART1 RTS and USART1 Tx as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_RTSPin | GPIO_TxPin;
	GPIO_InitStructure.GPIO_Pin = GPIO_TxPin1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	/* Configure USART1 CTS and USART1 Rx as input floating */
	//GPIO_InitStructure.GPIO_Pin = GPIO_CTSPin | GPIO_RxPin;
	GPIO_InitStructure.GPIO_Pin = GPIO_RxPin1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	
	
	/* UART2 ----------------------------------------------------------------*/
	/* Configure USART1 RTS and USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_TxPin2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
	/* Configure USART1 CTS and USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_RxPin2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/* UART3 for Motor/BLC --------------------------------------------------*/
	/* Configure USART1 RTS and USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_TxPin3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   
	/* Configure USART1 CTS and USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_RxPin3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	/* UART4 for GPS -------------------------------------------------------*/
	/* Configure USART1 RTS and USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_TxPin4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
	/* Configure USART1 CTS and USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_RxPin4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* Status LED -----------------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_LEDPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	// for Debug and Beeper Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_DBG | GPIO_BEEPER;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	// for Servo0-3 Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_SERVO0 | GPIO_SERVO1 | GPIO_SERVO2 | GPIO_SERVO3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	/* ADC for ACC, Gyro, .. -------------------------------------------------*/
	// 16 & 17 no need GPIO_Mode_AIN
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_GYRO_X | GPIO_GYRO_Y | GPIO_GYRO_Z | GPIO_ACC_X | GPIO_ACC_Y | GPIO_ACC_Z;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	// Voltage
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_V;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	
	/* Configure PA8 for PPM Dekode -----------------------------------------*/
	/* TIM1 channel 1 pin (PA.08) configuration */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/* Configures the nested vectored interrupt controller. ---------------------*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the TIM1 global Interrupt for PPM decode ----------------------*/ 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQChannel; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	/* Enable the USART1 Interrupt --------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt --------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART3 Interrupt --------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART4 Interrupt --------------------------------------------*/
	/*
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	
	/* Interrupt for Mainloop -----------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 
  	/* Interrupt for Status LED ---------------------------------------------*/
	/* Configure two bits for preemption priority - LED*/
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the TIM2 Interrupt - LED */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
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

	/* if I enable TXE befor RXNE then it does not work - no idea why ??? */
	/* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the 
    EVAL_COM1 receive data register is not empty */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 
	/* Enable the EVAL_COM1 Transmoit interrupt: this interrupt is generated when the 
    EVAL_COM1 transmit data register is empty */  
	// enalble Interrupt if data send in serial.c
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	
}

/* Init UART2 --------------------------------------------------------------------*/
void initUART2 (void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART2 configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);
	/* Enable the USART3 */
	USART_Cmd(USART2, ENABLE);

	/* if I enable TXE befor RXNE then it does not work - no idea why ??? */
	/* Enable the EVAL_COM3 Receive interrupt: this interrupt is generated when the 
    EVAL_COM3 receive data register is not empty */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
 
	/* Enable the EVAL_COM3 Transmoit interrupt: this interrupt is generated when the 
    EVAL_COM3 transmit data register is empty */  
	// enalble Interrupt if data send in serial.c
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

/* Init UART3 --------------------------------------------------------------------*/
void initUART3 (void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART3 configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE);

	/* if I enable TXE befor RXNE then it does not work - no idea why ??? */
	/* Enable the EVAL_COM3 Receive interrupt: this interrupt is generated when the 
    EVAL_COM3 receive data register is not empty */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
 
	/* Enable the EVAL_COM3 Transmoit interrupt: this interrupt is generated when the 
    EVAL_COM3 transmit data register is empty */  
	// enalble Interrupt if data send in serial.c
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

/* Init UART4 --------------------------------------------------------------------*/
void initUART4 (void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART4 configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART4, &USART_InitStructure);
	/* Enable the USART4 */
	USART_Cmd(UART4, ENABLE);

	/* if I enable TXE befor RXNE then it does not work - no idea why ??? */
	/* Enable the EVAL_COM3 Receive interrupt: this interrupt is generated when the 
    EVAL_COM3 receive data register is not empty */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	
 
	/* Enable the EVAL_COM3 Transmoit interrupt: this interrupt is generated when the 
    EVAL_COM3 transmit data register is empty */  
	// enalble Interrupt if data send in serial.c
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

/* Init DMA for Sensor ------------------------------------------------------*/
void initDMA (void)
{
	/* DMA1 channel1 configuration for Sensors */
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCDMASensorValue[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 56;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA_Channel1, ENABLE);
}

/* ADC1 configuration for Sensors (Gyro, ACC) -------------------------------*/
void initADC (void) 
{
	ADC_InitTypeDef  ADC_InitStructure;
	
	/* ADC1 configuration */
	ADC_DeInit(ADC1);
		
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 7;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel11 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5); //-> Gyro X
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5); //-> Gyro Y
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_55Cycles5); //-> Gyro Z
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5); //-> ACC X
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_55Cycles5); //-> ACC Y
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_55Cycles5); //-> ACC Z
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 7, ADC_SampleTime_55Cycles5);  //-> Volt

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}

/* init System Ticker with 1ms ----------------------------------------------*/
void initSysTick()
{
	// config for systick - delay
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_SetReload(72000);
	SysTick_ITConfig(ENABLE);
	SysTick_CounterCmd(SysTick_Counter_Enable);
}

