/**
  ******************************************************************************
  * @file: 		motor.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho motor.h
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#ifndef  MOTOR_H

#define  MOTOR_H
#include "stm32f10x.h"
#include "globalData.h"
#include "timer.h"


#define motor_on					 GPIO_WriteBit(PORT_EN_MOTOR, PIN_EN_MOTOR, (BitAction)1)
#define motor_off				   GPIO_WriteBit(PORT_EN_MOTOR, PIN_EN_MOTOR, (BitAction)0)



/** define GSS --------------------------------------------------------*/
#define motorRight_A     TIM4->CCR3
#define motorRight_B     TIM3->CCR2

#define motorleft_A     TIM4->CCR4
#define motorleft_B     TIM3->CCR3



typedef struct __pid_t{
	uint8_t dir;
	float setpoint;
	float error,errorOld;
	float sum,sub;
	
	float out,outPwm;
} pid_t;

void motorActive(uint8_t power);

/** function GSS ------------------------------------------------------*/
void controlMotorGss(void);
void motorLeft(float vel);
void motorRight(float vel);
void pwmr(int mtr_a,int mtr_b);
void pwml(int mtr_a,int mtr_b);
#endif
