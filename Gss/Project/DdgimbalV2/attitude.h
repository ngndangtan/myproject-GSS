/**
  ******************************************************************************
  * @file: 		attitude.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho attitude.h
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "stm32f10x.h"
#include "globalData.h"
#include <math.h>
#include "gMath.h"

#define GYRO_SCALE 32.8

uint32_t getTime(uint32_t *lastTime);
float ultraFastAtan2(float y, float x);
void attitudeEstimate( void);
