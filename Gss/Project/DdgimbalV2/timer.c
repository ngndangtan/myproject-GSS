/**
  ******************************************************************************
  * @file: 	  timer.c
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao cac chuc nang timer 
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "timer.h"
void timerInit(	void)
{	 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
  /* Description:
		  PA8 --> TIM1_CH1
	
		  PA6 --> TIM3_CH1 
		  PA7 --> TIM3_CH2	
		  PB0 --> TIM3_CH3 
		  PB1 --> TIM3_CH4
	
		  PB6 --> TIM4_CH1	
		  PB7 --> TIM4_CH2   
		  PB8 --> TIM4_CH3 
		  PB9 --> TIM4_CH4
	
			TIM2 dinh thoi gian xu ly 1ms
	*/
	
	
	#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	/* configuration gpio of pin enable l6234*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_EN_MOTOR ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_EN_MOTOR, &GPIO_InitStructure);
	
	/* configuration gpio for pwm */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/** PWM ------------------------------------------------------------------------------------------------------------------------*/
	/* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = MOTOR_FREQ_36KHZ;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1 Configuration in PWM mode */
 // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	//disable khi dung spi2 tranh dung channel Not.
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	 
	/* set Timer 3 clock running at 36Mhz */
  TIM_TimeBaseStructure.TIM_Period =MOTOR_FREQ_36KHZ;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_PrescalerConfig(TIM3, 0, TIM_PSCReloadMode_Immediate); 
  	
//  /* PWM Mode configuration for Channel 1 : TIM3_CCR1  */
// 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	TIM_OCInitStructure.TIM_Pulse = 0;						   
// 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM Mode configuration for Channel 2 : TIM3_CCR2  */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM Mode configuration for Channel 3 : TIM3_CCR3  */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM Mode configuration for Channel 4 : TIM3_CCR4  */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
 
	/* set Timer 4 clock running at 36Mhz */
	TIM_TimeBaseStructure.TIM_Period = MOTOR_FREQ_36KHZ;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_PrescalerConfig(TIM4, 0, TIM_PSCReloadMode_Immediate);
  	
  /* PWM Mode configuration for Channel 1 : TIM4_CCR1  */	 
// 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
// 	TIM_OCInitStructure.TIM_Pulse = 0;						
// 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//	/* PWM Mode configuration for Channel 2 : TIM4_CCR2  */	 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse =0;						  
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	/* PWM Mode configuration for Channel 3 : TIM4_CCR3  */	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	/* PWM Mode configuration for Channel 4 : TIM4_CCR4  */	 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0;						  
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
  		
	TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
	
	/* TIME BASE -------------------------------------------------------------------------------------------------------------------*/
	/* set Timer 2 clock running at 36Mhz */
	TIM_TimeBaseStructure.TIM_Period = FREQ_1KHZ;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Immediate);

  TIM_ARRPreloadConfig(TIM2, ENABLE);
	/* set timer 2 interrup to make 1ms update tick */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);			
  TIM_Cmd(TIM2, ENABLE);		
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;					  //3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;									//0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
  NVIC_Init(&NVIC_InitStructure);
}
