/**
  ******************************************************************************
  * @file: 		gMavlinkV1_0.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho gMavlinkV1_0.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

//include
#include "stm32f10x.h"
#include "mavlink_types.h"

//define 
#define PC_MODE_VALUE_HIGH 				400
#define PC_MODE_VALUE_LOW 				-400
#define PC_PITCH_SET_ANGLE_SCALE 	1.0
#define PC_ROLL_SET_ANGLE_SCALE 	1.0
#define PC_YAW_SET_ANGLE_SCALE 		1.0

#define PC_PITCH_SET_SPEED_SCALE 	1.0
#define PC_ROLL_SET_SPEED_SCALE 	1.0
#define PC_YAW_SET_SPEED_SCALE 		1.0

#define PC_NO_SIGNAL_COUNT 				100

#define PC_TILT_CHANNEL		0
#define PC_ROLL_CHANNEL		1
#define PC_PAN_CHANNEL		2
#define PC_MODE_CHANNEL		3

//fuction
void param2Config(uint16_t index);
void getParamsDefault(void);
void getParamsFromFlashAfterPowerOn(void) ;
void mavlinkSend(void);
void handle_mavlink_message(mavlink_channel_t chan, mavlink_message_t* msg);
int  mavlinkReceive(void);
