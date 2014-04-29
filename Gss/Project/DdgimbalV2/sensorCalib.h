/**
  ******************************************************************************
  * @file: 		sensorCalib.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho sensorCalib.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

//include---------------------------------------------------------------------------------------------------------------------------
#include "globalData.h"
#include "eeprom.h"
#include "motor.h"

//function -------------------------------------------------------------------------------------------------------------------------
int gyroCalibrate(void);
int accelCalibrate(void);
int gyroCalibrateF(void);
int accelCalibrateF(void);
int accelCalibrateOne(void);
int accelCalibrateSix(void);
