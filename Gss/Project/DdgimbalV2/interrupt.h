/**
  ******************************************************************************
  * @file: 		interrupt.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   chua cac ham ngat
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "stm32f10x.h"

void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void USART3_IRQHandler(void);
void TIM2_IRQHandler(void);
void MPU6000_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

void readVel(void);
