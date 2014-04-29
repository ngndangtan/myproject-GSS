/**
  ******************************************************************************
  * @file: 		globalData.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao cac bien sua dung trong chuong trinh
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "globalData.h"
u8 mav_tx_buffer[256];
 
uint16_t VirtAddVarTab[NumbOfVar] ;  

int16_t gyrox_1=0;	
int16_t gyroy_1=0;
int16_t gyroz_1=0;
int16_t accx_1=0;	
int16_t accy_1=0;
int16_t accz_1=0;
int16_t gyrox=0;	
int16_t gyroy=0;
int16_t gyroz=0;
int16_t accx=0;	
int16_t accy=0;
int16_t accz=0;


uint32_t durationAtti;

uint32_t timerAtti;	

uint16_t updateMav;
uint16_t updateRadio;

uint16_t *unique = (uint16_t *)0x1FFFF7E8;
u32 keycodeMcu;
//float gFactor;
float pitchSet,rollSet, yawSet, yawSetOld, pitchSpeed,rollSpeed,yawSpeed;
float errorPan,errorRoll,errorTilt,errorSpeedTilt;

u8 mav_rx_buffer[MAV_RX_BUFF_SIZE];
u8 mav_rx_tmp[MAV_RX_BUFF_SIZE];

u8 gps_buffer[GPS_BUFF_SIZE]; 
u8 UBX_buffer[UBX_MAXPAYLOAD];
u8 Data_UBX[GPS_BUFF_SIZE];
u8 check_a;
u8 check_b;
u8 UBX_step;
gUblox_t gUblox;

gConfig_t gConfig;
gSensor_t gSensor;
gAtti_t   gAtti;
gMotor_t  gMotor;
gState_t  gState;
gMav_t 		gMav;
gRadio_t 	gRadio;
global_struct global_data;
gBattery_t 		gBattery;
//bien hall gss__________________________________________//
_hall_t hall_r,hall_l;
u8 status_motor;
//bien motor gss__________________________________________//
float leftSet,rightSet;
