/**
  ******************************************************************************
  * @file: 		sensorCalib.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   calib cac gia tri cam bien( acc, gyro)
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "sensorCalib.h"
#include "gMavlinkV1_0.h"
#include "gMath.h"

extern gSensor_t gSensor,gSensorF;
extern gState_t gState;
extern uint16_t VirtAddVarTab[];
extern global_struct global_data;
extern gMav_t 		gMav;

int j=0;	 	  
int32_t gyroXsum=0;
int32_t gyroYsum=0;
int32_t gyroZsum=0;
	
int gyroCalibrate(void)
{	
	if (j++==3000)
	{
		gSensor.gyroXOffset=	(int16_t)(gyroXsum/3000);
		gSensor.gyroYOffset=	(int16_t)(gyroYsum/3000);
		gSensor.gyroZOffset=	(int16_t)(gyroZsum/3000);
		j=0;
		
		EE_WriteVariable(VirtAddVarTab[PARAM_GYROX_OFFSET], gSensor.gyroXOffset);
		EE_WriteVariable(VirtAddVarTab[PARAM_GYROY_OFFSET], gSensor.gyroYOffset);
		EE_WriteVariable(VirtAddVarTab[PARAM_GYROZ_OFFSET], gSensor.gyroZOffset);
		
		global_data.param[PARAM_GYROX_OFFSET]=gSensor.gyroXOffset;
		global_data.param[PARAM_GYROY_OFFSET]=gSensor.gyroYOffset;
		global_data.param[PARAM_GYROZ_OFFSET]=gSensor.gyroZOffset;
		
		gState.gyroCalibrated = 1;
		gyroXsum = 0;
		gyroYsum = 0;
		gyroZsum = 0;
	}
	else if(j < 3000)
	{
		gyroXsum+=gSensor.gyroX;
		gyroYsum+=gSensor.gyroY;
		gyroZsum+=gSensor.gyroZ;
	}
	
	return 1;	 
}

u16 cntCalAcc = 0;
int accXsum = 0;
int accYsum = 0;
int accZsum = 0;
int accXface[7];
int accYface[7];
int accZface[7];

int accelCalibrate(void)
{
	if(gState.numFaceAcc == 0){
		accelCalibrateOne();
	}
	else if(gState.numFaceAcc == 1){
		accelCalibrateSix();
	}

	return 0;
}

int accelCalibrateOne(void){
	if (cntCalAcc++ == 3000)
	{		
		int16_t accXOffset = accXsum/3000;
		int16_t accYOffset = accYsum/3000;
		int16_t accZOffset = 4096 + accZsum/3000;
		
		cntCalAcc = 0;
		
		gSensor.accXOffset = accXOffset;
		gSensor.accYOffset = accYOffset;
		gSensor.accZOffset = accZOffset;
		
		EE_WriteVariable(VirtAddVarTab[PARAM_ACCX_OFFSET], gSensor.accXOffset);
		EE_WriteVariable(VirtAddVarTab[PARAM_ACCY_OFFSET], gSensor.accYOffset);
		EE_WriteVariable(VirtAddVarTab[PARAM_ACCZ_OFFSET], gSensor.accZOffset);
			
		global_data.param[PARAM_ACCX_OFFSET] = gSensor.accXOffset;
		global_data.param[PARAM_ACCY_OFFSET] = gSensor.accYOffset;
		global_data.param[PARAM_ACCZ_OFFSET] = gSensor.accZOffset;
		
		gState.numFaceAccCalib = 0;
		gState.numFace = 0;
		accXsum = 0;
		accYsum = 0;
		accZsum = 0;
	}
	else 
	{
		accXsum += gSensor.accX;
		accYsum += gSensor.accY;
		accZsum += gSensor.accZ;
	}
	return 0;
}

int accelCalibrateSix(void){
	if (cntCalAcc++ == 3000)
	{
		if(gState.numFaceAccCalib <= 6){
				accXface[gState.numFaceAccCalib] = (int16_t)(accXsum/3000);
				accYface[gState.numFaceAccCalib] = (int16_t)(accYsum/3000);
				accZface[gState.numFaceAccCalib] = (int16_t)(accZsum/3000);
				accXsum = 0;
				accYsum = 0;
				accZsum = 0;
				gState.numFace++;
				if(gState.numFace > 7) gState.numFace = 7;
		}
		else{
				u8 cntFaceX = 0,	cntFaceY = 0,	cntFaceZ = 0;
				int sumXFaces = 0,sumYFaces = 0,sumZFaces = 0;
				u8 i = 0;
				
				for(i = 0 ; i <= 6; i++){
					if(accXface[i] < 1000 && accXface[i] > -1000 && accXface[i] != 0){
						cntFaceX++;
						sumXFaces += accXface[i];
					}
					
					if(accYface[i] < 1000 && accYface[i] > -1000 && accYface[i] != 0){
						cntFaceY++;
						sumYFaces += accYface[i];
					}
					
					if(accZface[i] < 1000 && accZface[i] > -1000 && accZface[i] != 0){
						cntFaceZ++;
						sumZFaces += accZface[i];
					}
					
					accXface[i] = 0;
					accYface[i] = 0;
					accZface[i] = 0;
				}
				
				if(cntFaceX != 0)	gSensor.accXOffset = (int16_t)(sumXFaces/cntFaceX);
				else										gSensor.accXOffset = 0;
				
				if(cntFaceY != 0)	gSensor.accYOffset = (int16_t)(sumYFaces/cntFaceY);
				else										gSensor.accYOffset = 0;
				
				if(cntFaceZ != 0)	gSensor.accZOffset =	(int16_t)(sumZFaces/cntFaceZ);
				else										gSensor.accZOffset = 0;
				
				EE_WriteVariable(VirtAddVarTab[PARAM_ACCX_OFFSET], gSensor.accXOffset);
				EE_WriteVariable(VirtAddVarTab[PARAM_ACCY_OFFSET], gSensor.accYOffset);
				EE_WriteVariable(VirtAddVarTab[PARAM_ACCZ_OFFSET], gSensor.accZOffset);
				
				global_data.param[PARAM_ACCX_OFFSET] = gSensor.accXOffset;
				global_data.param[PARAM_ACCY_OFFSET] = gSensor.accYOffset;
				global_data.param[PARAM_ACCZ_OFFSET] = gSensor.accZOffset;
				
				gState.numFaceAccCalib = 0;
				gState.numFace = 0;
		}
	}
	else if(cntCalAcc < 3000)
	{
		accXsum += gSensor.accX;
		accYsum += gSensor.accY;
		accZsum += gSensor.accZ;
	}
	
	return 0;
}


