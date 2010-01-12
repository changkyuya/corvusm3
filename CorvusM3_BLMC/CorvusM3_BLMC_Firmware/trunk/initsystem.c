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
vu16 ADCSensorValue[56];

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

	/* Enable TIMER clocks  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);  // Timer
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8, ENABLE);
	
	/* Enable GPIO clocks A,B,C,D */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	/* AFIO and USART1 clocks UART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

/* Configures the used Timers. ----------------------------------------------*/
void TIM_Configuration(void)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM1_TimeBaseInitTypeDef  TIM1_TimeBaseStructure;
	TIM1_OCInitTypeDef  TIM1_OCInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure8;
	TIM_OCInitTypeDef  TIM_OCInitStructure4;
	TIM_OCInitTypeDef  TIM_OCInitStructure2;
	TIM_OCInitTypeDef  TIM_OCInitStructure3;
	
	/* Timer 1---------------------------------------------------------------------------*/
	/* Time base configuration */
	/* TIM1CLK = 72 MHz, Prescaler = 0, TIM1 counter clock = 72 MHz
       TIM1 frequency = TIM1CLK/(TIM1_Period + 1) = 17.57 KHz */
	TIM1_TimeBaseStructure.TIM1_Period = 4095;
	TIM1_TimeBaseStructure.TIM1_Prescaler = 0;
	TIM1_TimeBaseStructure.TIM1_ClockDivision = 0;
	TIM1_TimeBaseStructure.TIM1_CounterMode = TIM1_CounterMode_Up;

	TIM1_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);
	
	// 3_PWM_B
	/* PWM1 Mode configuration: Channel1 */
	TIM1_OCInitStructure8.TIM1OCMode = TIM1_OCMode_PWM1;
	TIM1_OCInitStructure8.TIM1_OutputState = TIM1_OutputState_Enable;
	TIM1_OCInitStructure8.TIM1_Channel = TIM1_Channel_1;   
	TIM1_OCInitStructure8.TIM1_Pulse = 2047;
	TIM1_OCInitStructure8.TIM1_OCPolarity = TIM1_OCPolarity_High; 
	TIM1_OCInit(TIM1, &TIM1_OCInitStructure);
	TIM1_OC1PreloadConfig(TIM1, TIM1_OCPreload_Enable);

	// 3_PWM_A
	/* PWM1 Mode configuration: Channel4 */
	TIM1_OCInitStructure8.TIM1_Channel = TIM1_Channel_4;   
	TIM1_OCInitStructure8.TIM1_Pulse = 2047;
	TIM1_OCInit(TIM1, &TIM1_OCInitStructure);
	TIM1_OC4PreloadConfig(TIM1, TIM1_OCPreload_Enable);

	TIM1_ARRPreloadConfig(TIM1, ENABLE);
	/* Enable the TIM Main Output only needed for timer 1 and 8*/
    TIM1->BDTR |= ((u16)0x8000); //BDTR_MOE_Set;
	/* TIM8 enable counter */
	TIM1_Cmd(TIM1, ENABLE);
	/* Immediate load of TIM8 Precaler value */
	//TIM_PrescalerConfig(TIM8, 72, TIM_PSCReloadMode_Immediate);
	/* Clear TIM8 update pending flag */
	//TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	
	
	
	
	/* Timer 3---------------------------------------------------------------------------*/
	/* Time base configuration */
	/* TIM1CLK = 72 MHz, Prescaler = 0, TIM1 counter clock = 72 MHz
       TIM1 frequency = TIM1CLK/(TIM1_Period + 1) = 17.57 KHz */
	TIM_TimeBaseStructure.TIM_Period = 4095;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	// 2_PWM_C
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure3.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure3.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure3.TIM_Channel = TIM_Channel_1;   
	TIM_OCInitStructure3.TIM_Pulse = 2047;
	TIM_OCInitStructure3.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInit(TIM3, &TIM_OCInitStructure3);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	// 2_PWM_B
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure3.TIM_Channel = TIM_Channel_2;   
	TIM_OCInitStructure3.TIM_Pulse = 2047;
	TIM_OCInit(TIM3, &TIM_OCInitStructure3);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM8 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	/* Immediate load of TIM8 Precaler value */
	//TIM_PrescalerConfig(TIM8, 72, TIM_PSCReloadMode_Immediate);
	/* Clear TIM8 update pending flag */
	//TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	
	
	
	/* Timer 4---------------------------------------------------------------------------*/
	/* Time base configuration */
	/* TIM1CLK = 72 MHz, Prescaler = 0, TIM1 counter clock = 72 MHz
       TIM1 frequency = TIM1CLK/(TIM1_Period + 1) = 17.57 KHz */
	TIM_TimeBaseStructure.TIM_Period = 4095;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// 2_PWM_A
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure4.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure4.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure4.TIM_Channel = TIM_Channel_1;   
	TIM_OCInitStructure4.TIM_Pulse = 2047;
	TIM_OCInitStructure4.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInit(TIM4, &TIM_OCInitStructure4);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// 1_PWM_C
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure4.TIM_Channel = TIM_Channel_2;   
	TIM_OCInitStructure4.TIM_Pulse = 2047;
	TIM_OCInit(TIM4, &TIM_OCInitStructure4);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// 1_PWM_B
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure4.TIM_Channel = TIM_Channel_3;   
	TIM_OCInitStructure4.TIM_Pulse = 2047;
	TIM_OCInit(TIM4, &TIM_OCInitStructure4);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	// 1_PWM_A
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure4.TIM_Channel = TIM_Channel_4;   
	TIM_OCInitStructure4.TIM_Pulse = 2047;
	TIM_OCInit(TIM4, &TIM_OCInitStructure4);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM8 enable counter */
	TIM_Cmd(TIM4, ENABLE);
	/* Immediate load of TIM8 Precaler value */
	//TIM_PrescalerConfig(TIM8, 72, TIM_PSCReloadMode_Immediate);
	/* Clear TIM8 update pending flag */
	//TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	
	
	
	/* Timer 8---------------------------------------------------------------------------*/
	/* Time base configuration */
	/* TIM1CLK = 72 MHz, Prescaler = 0, TIM1 counter clock = 72 MHz
       TIM1 frequency = TIM1CLK/(TIM1_Period + 1) = 17.57 KHz */
	TIM_TimeBaseStructure.TIM_Period = 4095;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	// 4_PWM_C
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure8.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure8.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_1;   
	TIM_OCInitStructure8.TIM_Pulse = 2047;
	TIM_OCInitStructure8.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInit(TIM8, &TIM_OCInitStructure8);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);

	// 4_PWM_B
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_2;   
	TIM_OCInitStructure8.TIM_Pulse = 2047;
	TIM_OCInit(TIM8, &TIM_OCInitStructure8);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

	// 4_PWM_A
	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_3;   
	TIM_OCInitStructure8.TIM_Pulse = 2047;
	TIM_OCInit(TIM8, &TIM_OCInitStructure8);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	// 3_PWM_C
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure8.TIM_Channel = TIM_Channel_4;   
	TIM_OCInitStructure8.TIM_Pulse = 2047;
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
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
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
	
	
	/* Status LED -----------------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_LEDPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	/* DBG */
	GPIO_InitStructure.GPIO_Pin = GPIO_DBGPIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	/* 1. Switch */
	GPIO_InitStructure.GPIO_Pin = GPIO_1_SWITCH_A;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_1_SWITCH_B;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_1_SWITCH_C;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	/* 2. Switch */
	GPIO_InitStructure.GPIO_Pin = GPIO_2_SWITCH_A;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_2_SWITCH_B;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_2_SWITCH_C;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	/* 3. Switch */
	GPIO_InitStructure.GPIO_Pin = GPIO_3_SWITCH_A;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_3_SWITCH_B;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_3_SWITCH_C;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	/* 4. Switch */
	GPIO_InitStructure.GPIO_Pin = GPIO_4_SWITCH_A;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_4_SWITCH_B;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_4_SWITCH_C;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	
	/* ADC --------------------------------------------------------------*/
	// ADC
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	// 1. ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_1_ADC_A | GPIO_1_ADC_B | GPIO_1_ADC_C | GPIO_1_ADC_S;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	// 2. ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_2_ADC_A | GPIO_2_ADC_B | GPIO_2_ADC_C | GPIO_2_ADC_S;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	// 3. ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_3_ADC_A | GPIO_3_ADC_B | GPIO_3_ADC_C | GPIO_3_ADC_S;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	// 4. ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_4_ADC_A | GPIO_4_ADC_B;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_4_ADC_C | GPIO_4_ADC_S;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 


}

/* Configures the nested vectored interrupt controller. ---------------------*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the USART1 Interrupt --------------------------------------------*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
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

