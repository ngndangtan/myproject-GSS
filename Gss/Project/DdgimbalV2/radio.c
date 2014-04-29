/**
  ******************************************************************************
  * @file: 	  radio.c	  
  * @author:  GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   procees signals control (sbus, pwm, gmotion, pc, joystick)
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
/* include -------------------------------------------------------------------------------------------------------------------------*/
#include "radio.h"
#include "sbus.h"
#include "gpio.h"
#include "gMath.h"
#include "adc.h"

/*define -------------------------------------------------------------------------------------------------------------------------*/

/*variable -----------------------------------------------------------------------------------------------------------------------*/
extern gRadio_t  gRadio;
//extern gMotor_t  gMotor;
extern gConfig_t gConfig;

extern gState_t  	gState;
extern adcStruct adcPan,adcTilt,adcBaterry;
//GSS///////////
extern _hall_t hall_r,hall_l;
extern u8 status_motor;
float angle_old;
extern float rightSet,leftSet;
extern gAtti_t   gAtti; 

extern int data_sensor1,data_sensor2,sensor_head,sensor_end;
extern gState_t gState;
/////
/*fuction ------------------------------------------------------------------------------------------------------------------------*/

void radioApply(void){

}

void text_sensor(void){	
			float temp;
			char update_angle;
	
			int16_t levelSbusChan = (gRadio.sbusChan[gConfig.sbusPitchChan] - 	gConfig.rcPitchTrim);
			int16_t driveSbusChan 	= (gRadio.sbusChan[gConfig.sbusYawChan] - 		gConfig.rcYawTrim);
	
			if(gState.button_push)gState.button_push=0;// khi nhan nut
			if(levelSbusChan > 50) {

				levelSbusChan -= 50;
			}
			else if(levelSbusChan < -50) {

				levelSbusChan += 50;
			}
			else{
				levelSbusChan = 0;
			}
			
			if(driveSbusChan > 50){
				driveSbusChan -= 50;
				}
			else if(driveSbusChan < -50){
				driveSbusChan += 50;
			}
			else{				
				driveSbusChan = 0;
			}
			
			gRadio.noSbusCount++;
			gRadio.level=(gRadio.level*gConfig.rcPitchLPF+	levelSbusChan)/(1+gConfig.rcPitchLPF);
			gRadio.drive=  (gRadio.drive*gConfig.rcYawLPF+			driveSbusChan)/(1+gConfig.rcYawLPF);
			gRadio.mode= 	gRadio.sbusChan[gConfig.sbusModeChan];
			
			if((gRadio.mode>-100)&&(gRadio.mode<100)){
				angle_old=gAtti.psi;				
				}
			else{
				temp=(gAtti.psi-angle_old)*1.5;				
				}
	
			if(gRadio.level>0) gRadio.level = gRadio.level*gRadio.level/1500.0;
			else gRadio.level = -gRadio.level*gRadio.level/1500.0;

			if(gRadio.mode<-100)//bam line
			{
				if(data_sensor1!=0){
				if(update_angle==1){
					update_angle=0;
				}
				leftSet		=   gRadio.level- (data_sensor1/2.0);//-(temp);//+( gRadio.level*data_sensor1/900);//*(hall_r.vel/hall_l.vel)
				rightSet	=   gRadio.level+ (data_sensor1/2.0);//-( gRadio.level*data_sensor1/900);
				}
				else{
						if(update_angle==0){
							update_angle=1;
							angle_old=gAtti.psi;
						}		
				temp=(gAtti.psi-angle_old)*1.5;					
				leftSet		=   (0.5*gRadio.level*(hall_r.vel/hall_l.vel))+  (0.5*( gRadio.level-temp ));//+( gRadio.level*data_sensor1/900);//*(hall_r.vel/hall_l.vel)
				rightSet	=   (0.5*gRadio.level*(hall_l.vel/hall_r.vel))+  (0.5*( gRadio.level+temp ));// (data_sensor1/2.0);//-( gRadio.level*data_sensor1/900);		
				}
			}
			else if(gRadio.mode>100)//dieu khien bang tay
			{
			if(gRadio.level>0)
			{
				if(gRadio.drive > 0) {
					leftSet = gRadio.level;	
					rightSet = gRadio.level-(gRadio.drive*gRadio.drive/100.0);
			}
				else {
					leftSet = (gRadio.level)-(gRadio.drive*gRadio.drive/100.0);//gRadio.level* gRadio.level/300;
					rightSet =(gRadio.level);//(gRadio.level* gRadio.level/300 - (gRadio.yaw*(gRadio.level + gRadio.yaw)/300));
				}
			}
			else{
				if(gRadio.drive > 0) {
					leftSet = (gRadio.level) ;	//(gRadio.level* gRadio.level/300 - (gRadio.yaw*(gRadio.level + gRadio.yaw)/300));
					rightSet =gRadio.level + (gRadio.drive*gRadio.drive/100.0); //gRadio.level* gRadio.level/300;
				}
				else {
					leftSet =(gRadio.level)+(gRadio.drive*gRadio.drive/100.0); 	//gRadio.level* gRadio.level/300;
					rightSet = (gRadio.level);;//(pitchSet* pitchSet/300 - (gRadio.yaw*(pitchSet - gRadio.yaw)/300));
				}
			}
		}
			else{
				leftSet		=   0;
				rightSet	=   0;
	}
}

void sbusProcess(void){
}

void ppmProcess(void){			
}

void pcProcess(void){
	 int16_t drivePcChan;
	 int16_t levelPcChan;
	

	
	static char update_angle;
	static float temp,dotinhtuong;
	
	drivePcChan = 	gRadio.pcChan[PC_PAN_CHANNEL]*gConfig.yawPower*8.0;
	levelPcChan = 	gRadio.pcChan[PC_TILT_CHANNEL]*gConfig.pitchPower*8.0;
	
	//mode hand	
	if (gConfig.rcYawMode == CONFIG_RC_MODE_HAND) 
	{	
			if(levelPcChan>0)
			{
				if(drivePcChan > 0) {
					leftSet = levelPcChan-drivePcChan;//(drivePcChan*drivePcChan/100.0);	
					rightSet = levelPcChan+drivePcChan;//(drivePcChan*drivePcChan/100.0);
			}
				else {
					leftSet = (levelPcChan)-drivePcChan;//(drivePcChan*drivePcChan/100.0);//gRadio.level* gRadio.level/300;
					rightSet =(levelPcChan)+drivePcChan;//(drivePcChan*drivePcChan/100.0);//(gRadio.level* gRadio.level/300 - (gRadio.yaw*(gRadio.level + gRadio.yaw)/300));
				}
			}
			else{
				if(drivePcChan > 0) {
					leftSet = levelPcChan+drivePcChan;	//(gRadio.level* gRadio.level/300 - (gRadio.yaw*(gRadio.level + gRadio.yaw)/300));
					rightSet =levelPcChan-drivePcChan; //gRadio.level* gRadio.level/300;
				}
				else {
					leftSet =levelPcChan+drivePcChan; 	//gRadio.level* gRadio.level/300;
					rightSet = levelPcChan-drivePcChan;//(pitchSet* pitchSet/300 - (gRadio.yaw*(pitchSet - gRadio.yaw)/300));
				}
			}
	}
	//mode line
	else{
		static int32_t goc_trai,goc_phai;
		static char update_status;
		static char count_L;
		static float goc_G;
		static char doigoc;
		static int direction,stopp;
		static int sensor_temp1,sensor_temp2;
		static int dem;
		
//kiem tra nut nhan button start
		if(gState.button_push)
			{
				direction=1;
				gState.button_push=0;
			}
		
///doi chieu
		levelPcChan=gConfig.pitchPower;
		
		if((sensor_head)&&(direction==1))
				{
						stopp=1;
						if(dem++>80)
							{
							direction=-1;
								
							count_L=0;
							doigoc=0;
							update_status=0;
							}
				}
		else if((sensor_end)&&(direction==-1))
				{
						stopp=1;
						if(dem++>80)
							{
								direction=0;
								count_L=0;
								doigoc=0;
								update_status=0;
								}
				}
		else{stopp =0;dem=0;}
			
//			if(dem++>3000){direction=1;stopp =1;}}else{dem=0;stopp=0;}
			
//		if((sensor_end)&&(direction=1)){
//			if(dem++>3000){ direction=0;stopp=1;}}else{dem=0;stopp=0;}
////////////////	
///chon sensor
//			sensor_temp1=data_sensor1;
			if(direction==1)
					{
					sensor_temp1=data_sensor1;
					sensor_temp2=data_sensor2;
						
					
					}
			else if(direction==-1) 
					{
					sensor_temp1=data_sensor2;
					sensor_temp2=data_sensor1;
						

						
					}
////////////			
if (!stopp)
{	
		if(sensor_temp1!=0){
			if (sensor_temp1>64){
				
				if(count_L<4)
					{
						if(update_status==0){
							update_status=1;
								if( hall_l.count >2000000000){
									hall_l.count =0;
									}
								else if(hall_r.count >2000000000){
									hall_r.count=0;
									} 
								if( hall_l.count <-2000000000){
									hall_l.count =0;
									}
								else if(hall_r.count <-2000000000){
									hall_r.count=0;
									} 
							goc_trai= hall_l.count;
							goc_phai=	hall_r.count;
							goc_G	= gAtti.psi;
							}
		///////////				
					if(direction==1)		
						{
						switch(doigoc)
						{
							case 0:
									if(((hall_l.count-goc_trai)<(-300))&&((hall_r.count-goc_phai)>(300))){
										count_L++;
										doigoc=1;
										}
									else{
										if ((hall_l.count-goc_trai)>(-300)) leftSet=-20;else leftSet=0;
										if ((hall_r.count-goc_phai)<(300)) rightSet=+20;else rightSet=0;
										}
								break;
							case 1:
									if(((hall_l.count-goc_trai)>(300))&&((hall_r.count-goc_phai)<(-300))){
										doigoc=2;
										}
									else{
										if ((hall_l.count-goc_trai)<(300)) leftSet=+20;else leftSet=0;
										if ((hall_r.count-goc_phai)>(-300)) rightSet=-20;else rightSet=0;
										}
								break;
							case 2:
									if(((hall_l.count-goc_trai)<0)&&((hall_r.count-goc_phai)>0)){
										doigoc=3;
										}
									else{
										if ((hall_l.count-goc_trai)>0) leftSet=-20;else leftSet=0;
										if ((hall_r.count-goc_phai)<0) rightSet=+20;else rightSet=0;
										}
								break;
							case 3:
									if(((hall_l.count-goc_trai)>1000)&&((hall_r.count-goc_phai)>1000)){
											doigoc=4;
										}
									else{
										if ((hall_l.count-goc_trai)<1000) leftSet	=	20;else leftSet=0;				
										if ((hall_r.count-goc_phai)<1000) rightSet	=	20;else rightSet=0;
										}
								break;
							case 4:
									if((goc_G-gAtti.psi)>3){
										leftSet	=	-levelPcChan/2;
										rightSet=	levelPcChan/2;
										}
									else if((goc_G-gAtti.psi)<-3){
										leftSet	=	levelPcChan/2;
										rightSet=	-levelPcChan/2;
										}
									else{
										update_status=0;
										doigoc=0;
										goc_trai= hall_l.count;
										goc_phai=	hall_r.count;
										leftSet	=	0;
										rightSet=	0;
									}
								break;
							default:
								break;
							}	
						}
						else if(direction==-1)		
						{
						switch(doigoc)
						{
							case 0:
									if(((hall_l.count-goc_trai)>(300))&&((hall_r.count-goc_phai)<(-300))){
										count_L++;
										doigoc=1;
										}
									else{
										if ((hall_l.count-goc_trai)<(300)) leftSet=20;else leftSet=0;
										if ((hall_r.count-goc_phai)>(-300)) rightSet=-20;else rightSet=0;
										}
								break;
							case 1:
									if(((hall_l.count-goc_trai)<(-300))&&((hall_r.count-goc_phai)>(300))){
										doigoc=2;
										}
									else{
										if ((hall_l.count-goc_trai)>(-300)) leftSet=-20;else leftSet=0;
										if ((hall_r.count-goc_phai)<(300)) rightSet=+20;else rightSet=0;
										}
								break;
							case 2:
									if(((hall_l.count-goc_trai)>0)&&((hall_r.count-goc_phai)<0)){
										doigoc=3;
										}
									else{
										if ((hall_l.count-goc_trai)<0) leftSet=+20;else leftSet=0;
										if ((hall_r.count-goc_phai)>0) rightSet=-20;else rightSet=0;
										}
								break;
							case 3:
									if(((hall_l.count-goc_trai)<-1000)&&((hall_r.count-goc_phai)<-1000)){
											doigoc=4;
										}
									else{
										if ((hall_l.count-goc_trai)>-1000) leftSet	=	-20;else leftSet=0;				
										if ((hall_r.count-goc_phai)>-1000) rightSet	=	-20;else rightSet=0;
										}
								break;
							case 4:
									if((goc_G-gAtti.psi)>3){
										leftSet	=	-levelPcChan/2;
										rightSet=	levelPcChan/2;
										}
									else if((goc_G-gAtti.psi)<-3){
										leftSet	=	levelPcChan/2;
										rightSet=	-levelPcChan/2;
										}
									else{
										update_status=0;
										doigoc=0;
										goc_trai= hall_l.count;
										goc_phai=	hall_r.count;
										leftSet	=	0;
										rightSet=	0;
									}
								break;
							default:
								break;
							}	
						}
						}
					else{leftSet	=	0;rightSet=	0;}
				}
			//data <64 và data !=0
		else{
			count_L=0;
			doigoc=0;
			update_status=0;
			if(dotinhtuong--<0)dotinhtuong=0;
			if(update_angle==1){update_angle=0;}
						leftSet		=   (levelPcChan- (sensor_temp1/1.5))*direction;//-(temp);//+( gRadio.level*data_sensor1/900);//*(hall_r.vel/hall_l.vel)
						rightSet	=   (levelPcChan+ (sensor_temp1/1.5))*direction;//-( gRadio.level*sensor_temp1/900);
			}
		}
		//data =0
			else{				
				count_L=0;			
				if(dotinhtuong++>10000)dotinhtuong=10000;
				
						if(update_angle==0){
						update_angle=1;
						angle_old=gAtti.psi;
						}		
						temp=(gAtti.psi-angle_old);			
						
						leftSet	=((dotinhtuong/10000*gConfig.pitchPower*500*hall_r.vel/hall_l.vel)+ 
											((10000-dotinhtuong)/10000*((gConfig.pitchPower*500)-temp )))*direction;
						rightSet=(((dotinhtuong)/10000)*(gConfig.pitchPower*500)+  
											((10000-dotinhtuong)/10000*((gConfig.pitchPower*500)+temp )))*direction;
					}					
	}
else{leftSet=0;rightSet=0;}//stopp=0
	}
}

void joystickProcess(void){
}

void radioProcess(void){
	if (gConfig.radioType==CONFIG_RADIO_TYPE_SBUS && gRadio.noSbusCount++ < SBUS_NO_SIGNAL_COUNT)				/*use futaba sbus*/{
		//sbusProcess();
		text_sensor();
		gRadio.isActive = 1;
	}
//	else if (gConfig.radioType	==CONFIG_RADIO_TYPE_PPM && gRadio.noPpmCount++ < PPM_NO_SIGNAL_COUNT)	/* use futaba pwm */{
//		ppmProcess();
//		gRadio.isActive = 1;
//	}
//	else if(gConfig.radioType == 	CONFIG_RADIO_TYPE_HAND)/* use gmotion */{
//		joystickProcess();
//		gRadio.isActive = 1;
//	}
	else if (gConfig.radioType==	CONFIG_RADIO_TYPE_PC)/* use simulation pc */{
		pcProcess();
		gRadio.isActive = 1;
	}
	else{
		gRadio.noSbusCount = 	SBUS_NO_SIGNAL_COUNT;
		gRadio.isActive = 		0;
	}
}

