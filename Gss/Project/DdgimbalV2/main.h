/**
  ******************************************************************************
  * @file    	main.h
  * @author  	Gremsy Team
  * @version 	1.2.0
  * @date    	2/11/2013
  * @brief   	Header file for main.c module.
  ******************************************************************************

  <h2><center> GREMSY Co.,Ltd </center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif 
/*
*************************************************************************************************************************************
*															INCLUDED FILES															*
*************************************************************************************************************************************
*/
//lib stm32
#include "stm32f10x.h"
//lib interface
#include "usart1.h"
#include "gMavlinkV1_0.h"
#include "ublox_gps.h"
//lib control motor
#include "timer.h"
#include "motor.h"
//lib maths & process string
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//lib datas & save datas
#include "eeprom.h"
#include "globalData.h"
#include "startup.h"
#include "attitude.h"
#include "paramDefault.h"
//lib sensors & calib sensor
#include "sensorCalib.h"
#include "mpu6000.h"
//lib radios
#include "sbus.h"
#include "gpio.h"
#include "radio.h"
//lib adc
#include "adc.h"
/*
*************************************************************************************************************************************
*															PRIVATE DEFINE															*
*************************************************************************************************************************************
*/
                     

/*
*************************************************************************************************************************************
*															PRIVATE MACRO															*
*************************************************************************************************************************************
*/



/*
*************************************************************************************************************************************
*															PRIVATE TYPE	DEFINE														*
*************************************************************************************************************************************
*/

/*
*************************************************************************************************************************************
*															PRIVATE VARIABLES														*
*************************************************************************************************************************************
*/
extern const u16 pwmSin[];
extern uint32_t durationAtti;
extern uint16_t timerMav;
extern uint16_t timerRadio;
extern gConfig_t gConfig;
extern gSensor_t gSensor,gSensorF;
extern gAtti_t   gAtti; 
extern gState_t  gState;
extern gMotor_t gMotor;
extern gMav_t gMav;
extern gRadio_t gRadio;
extern gBattery_t gBattery;
//extern float gFactor;
extern float pitchSet,rollSet, yawSet;
extern float errorTilt,errorRoll,errorPan;

extern int16_t gyrox_1;	
extern int16_t gyroy_1;
extern int16_t gyroz_1;
extern int16_t accx_1;	
extern int16_t accy_1;
extern int16_t accz_1;
extern int16_t gyrox;	
extern int16_t gyroy;	  
extern int16_t gyroz;
extern int16_t accx;	
extern int16_t accy;
extern int16_t accz;

extern int16_t gyrox_1F;	
extern int16_t gyroy_1F;
extern int16_t gyroz_1F;
extern int16_t accx_1F;	
extern int16_t accy_1F;
extern int16_t accz_1F;
extern int16_t gyroxF;	
extern int16_t gyroyF;	  
extern int16_t gyrozF;
extern int16_t accxF;	
extern int16_t accyF;
extern int16_t acczF;

extern uint32_t timerAtti;
extern uint32_t timerAttiF;	

extern global_struct global_data;

extern uint16_t len;
extern uint8_t buffer[255];
extern  mavlink_message_t msg;
extern u8 flag_switch_computer_control;

extern gsensor_spi mpu6000Val;
extern gsensor_spi mpu6000FVal;

extern uint16_t *unique;
extern uint16_t VirtAddVarTab[NumbOfVar];

extern u32 keycodeMcu;
//gss//
extern float rightSet,leftSet;
/*
*************************************************************************************************************************************
*															PRIVATE FUNCTION PROTOTYPES												*
*************************************************************************************************************************************
*/
void delayms(__IO uint32_t num);
void EXTI9_5_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void USART3_IRQHandler(void);
void TIM2_IRQHandler(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*
*************************************************************************************************************************************
*							  						   		GLOBAL FUNCTION PROTOTYPES												*
*************************************************************************************************************************************
*/




#ifdef __cplusplus
}
#endif


#endif /* __MAIN_H */


/******************* (C) COPYRIGHT 2009 ARMVietNam *****END OF FILE****/
