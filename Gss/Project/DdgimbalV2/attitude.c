/**
  ******************************************************************************
  * @file: 	 	attitude.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   xu ly tin hieu mpu600 dua ra tin hieu goc nghien de dieu khien
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

 #include "attitude.h"

 extern uint32_t timerAtti;
 extern uint32_t durationAtti;
 extern gSensor_t gSensor;
 extern gAtti_t gAtti;
 extern gConfig_t gConfig;
 extern gMotor_t  gMotor;
 
uint32_t getTime(uint32_t *lastTime)
{  
	uint32_t currentTime;
  uint32_t t=0;
   
  currentTime = SysTick->VAL;
  
  if (currentTime<*lastTime)
  {  
		t=*lastTime-currentTime;
  }	
  else t=(uint32_t)(0xFFFFFF)+*lastTime-currentTime;
  
  return t;		   //  scale to 1 us
}

int timeProcessAtti = 0;

void attitudeEstimate(void)
{  
	float phi_acc, theta_acc;
	float p,q,r;
	float cos_phi ;
	float tan_theta;
	float sin_phi;
	float cos_theta;
//	float sin_theta;
	float ax,ay,az;
	float T;
	static int updateAcc=0;
	float gFactor;
 	u32 temptime = SysTick->VAL;
	 
	durationAtti = getTime(&timerAtti)/9;
	timerAtti=SysTick->VAL;
	T= (float)durationAtti/1000.0;
	gAtti.time=T;
	
	p=( float)(gSensor.gyroX-gSensor.gyroXOffset)/GYRO_SCALE;			//x
  q=( float)(gSensor.gyroY-gSensor.gyroYOffset)/GYRO_SCALE;			 //y
  r=( float)(gSensor.gyroZ-gSensor.gyroZOffset)/GYRO_SCALE;

	cos_phi = 	cos(gAtti.phi*3.14159/180);
	tan_theta = tan(gAtti.theta*3.14159/180);
	sin_phi = 	sin(gAtti.phi*3.14159/180);
	cos_theta = cos(gAtti.theta*3.14159/180);
	
// 	cos_phi = 	cosT(gAtti.phi);
// 	tan_theta = tanT(gAtti.theta);
// 	sin_phi = 	sinT(gAtti.phi);
// 	cos_theta = cosT(gAtti.theta);
	
  gAtti.phiDot=		p+sin_phi*tan_theta*q+cos_phi*tan_theta*r;
  gAtti.thetaDot=	cos_phi*q-sin_phi*r;
	
	gAtti.psiDot=(sin_phi/cos_theta)*q+(cos_phi/cos_theta)*r;

  gAtti.phi+=gAtti.phiDot*T*0.001;
  gAtti.theta+=gAtti.thetaDot*T*0.001;
  gAtti.psi+=gAtti.psiDot*T*0.001;
	
  if (gAtti.phi>180) gAtti.phi-=360;
  if (gAtti.phi<-180) gAtti.phi+=360;
  if (gAtti.theta>180) gAtti.theta-=360;
  if (gAtti.theta<-180) gAtti.theta+=360;
  if (gAtti.psi>180) gAtti.psi-=360;
  if (gAtti.psi<-180) gAtti.psi+=360;
	
  if (updateAcc++==50) 
  {
		updateAcc=0;
		ax=gSensor.accX-gSensor.accXOffset;
		ay=gSensor.accY-gSensor.accYOffset;
		az=gSensor.accZ-gSensor.accZOffset;
		theta_acc=-ultraFastAtan2(-ax,sqrt(az*az+ay*ay));
		
		if (az>0)
			phi_acc=ultraFastAtan2(-ay,-sqrt(az*az+0.01*ax*ax));
		else 
			phi_acc=ultraFastAtan2(-ay,sqrt(az*az+0.01*ax*ax));
 
		gFactor=sqrt(ax*ax+ay*ay+az*az)/GRAVITY;
   
		if (absFast(gFactor-1) < 0.2)
		{
			if(gAtti.accCorrection == 1){
				gAtti.phi = gConfig.gyroTrust*gAtti.phi+ (1-gConfig.gyroTrust)*phi_acc;
				gAtti.theta = gConfig.gyroTrust*gAtti.theta+(1-gConfig.gyroTrust)*theta_acc;
			}				
		}
	}

	timeProcessAtti = (temptime - SysTick->VAL)/9;
}
