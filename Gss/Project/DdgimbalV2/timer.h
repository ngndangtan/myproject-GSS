/**
  ******************************************************************************
  * @file: 	  timer.h
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho timer.h
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "stm32f10x.h"
#define MOTOR_FREQ_36KHZ 1000
#define MOTOR_FREQ_18KHZ 2000
#define MOTOR_FREQ_9KHZ  4000
#define FREQ_1KHZ 			 36000

#define PIN_EN_MOTOR			GPIO_Pin_5
#define PORT_EN_MOTOR			GPIOA

void timerInit(	void);
