/**
  ******************************************************************************
  * @file: 	 motor.c 
  * @author: GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   control motor with pid
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "motor.h"
#include "gpio.h"
#include "gMath.h"
#include "attitude.h"
extern gAtti_t gAtti;
extern gConfig_t gConfig;
extern gMotor_t gMotor;
extern gState_t gState;
extern gRadio_t  gRadio;

///GSS/////////////////////////
extern _hall_t hall_r,hall_l;

pid_t pidLeft,pidRight;

void motorLeft(float vel){
	if(vel >= 0){
		pidLeft.dir = 0;
		pidLeft.setpoint = vel;
	}
	else{
		pidLeft.dir = 1;
		pidLeft.setpoint = -vel;
	}
	pidLeft.error = pidLeft.setpoint - hall_l.vel;
	
//	if(pidLeft.error<0)		pidLeft.dir=~pidLeft.dir;
		
	pidLeft.sum += pidLeft.error;
	pidLeft.sub = pidLeft.error - pidLeft.errorOld;
	
	
	pidLeft.out = (pidLeft.error*gConfig.pitchKp + pidLeft.sum*gConfig.pitchKi/1000.0 + pidLeft.sub*gConfig.pitchKd/1000.0);
	
	if(pidLeft.out > 900){
			pidLeft.sum -= pidLeft.error;
		pidLeft.out = 900;
	}
	else if(pidLeft.out < 0){
		pidLeft.out = 0;
	}
	else{
		pidLeft.errorOld = pidLeft.error;
		}
	
	if(pidLeft.dir == 0){
		pwml(0,pidLeft.out);
	}
	else{
		pwml(pidLeft.out,0);
	}
}

void motorRight(float vel){
	if(vel >= 0){
		pidRight.dir = 0;
		pidRight.setpoint = vel;
	}
	else{
		pidRight.dir = 1;
		pidRight.setpoint = -vel;
	}
	pidRight.error = pidRight.setpoint - hall_r.vel;

//	if(pidRight.error<0)		pidRight.dir=~pidRight.dir;	
	
	pidRight.sum += pidRight.error;
	pidRight.sub = pidRight.error - pidRight.errorOld;
	pidRight.errorOld = pidRight.error;
	
	pidRight.out = (pidRight.error*gConfig.yawKp + pidRight.sum*gConfig.yawKi/1000.0 + pidRight.sub*gConfig.yawKd/1000.0);
	
	if(pidRight.out > 900){
		pidRight.out = 900;
		pidRight.sum -= pidRight.error;
	}
	else if(pidRight.out < 0){
		pidRight.out = 0;
	}

	
	if(pidRight.dir == 0){
		pwmr(0,pidRight.out);
	}
	else{
		pwmr(pidRight.out,0);
	}
}



void pwmr(int mtr_a,int mtr_b)
{
	motorRight_B =1000-(float)(mtr_a);
	motorRight_A =1000-(float)(mtr_b);
}
void pwml(int mtr_a,int mtr_b)
{

	motorleft_A =1000 - mtr_a;
	motorleft_B =1000 - mtr_b;
//if(mtr_a>30)motorleft_A =1000-mtr_a;else motorleft_A=1000-(mtr_a*2);
//	if(mtr_b>30)motorleft_B =1000- mtr_b;else motorleft_B=1000-(mtr_b*2);
}

void motorActive(uint8_t power)
{
		if (power)
    {	
				motor_on;       
	  }
		else
		{	
				motor_off;
		}

}





