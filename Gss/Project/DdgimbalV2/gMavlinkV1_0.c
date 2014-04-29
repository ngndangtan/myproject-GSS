/**
  ******************************************************************************
  * @file: 		gMavlinkV1_0.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief: 	process datas mavlink
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "gMavlinkV1_0.h"
#include "mpu6000.h"
#include "gpio.h"
#include "mavlink.h"
#include "radio.h"
#include "gMath.h"
#include "motor.h"
#include "globalData.h"
#include "eeprom.h"
#include "paramDefault.h"
#include "timer.h"
#include "adc.h"

extern int16_t 		gyrox;	
extern int16_t 		gyroy;
extern int16_t 		gyroz;
extern int16_t 		accx;	
extern int16_t 		accy;
extern int16_t 		accz;
extern uint16_t 	VirtAddVarTab[];
extern uint32_t 	durationAtti;
extern uint16_t 	timerMav;
extern gConfig_t 	gConfig;
extern gSensor_t 	gSensor;
extern gAtti_t   	gAtti; 
extern gState_t  	gState;
extern gRadio_t  	gRadio;
extern u8 				mav_rx_buffer[MAV_RX_BUFF_SIZE];
extern u8 				mav_rx_tmp[MAV_RX_BUFF_SIZE];
extern gMav_t 		gMav;
extern global_struct 	global_data;
extern u16 				cntCalAcc;
extern uint16_t 	*unique;
extern u32 keycodeMcu;
extern gMotor_t gMotor;

extern int timeProcessAtti;
extern int timeProcssGimbalStabi;
extern int timeProcessMain;

extern float errorTilt,errorRoll,errorPan,pitchSpeed;
extern gBattery_t gBattery;
extern float pitchSet,rollSet, yawSet,yawSpeed;
extern adcStruct adcPan,adcTilt,adcBaterry;
extern gUblox_t gUblox;
extern float accN,accD,accE;
extern float accGps;
extern _hall_t hall_r,hall_l;

extern pid_t pidLeft,pidRight;

mavlink_message_t 		msg;
mavlink_status_t 			status;
///////////


extern int data_sensor1,data_sensor2,sensor_head,sensor_end;
extern float leftSet;
//////////
uint16_t len;
uint8_t buffer[255];		   //without number elements of array the program running crazy.

int ctrMode = 0;

 void param2Config(uint16_t index)
 {
   switch (index)
   {
			case PARAM_VERSION:
				gConfig.version= global_data.param[PARAM_VERSION];
			break;
			case PARAM_SERIAL_NUMBER:
				gConfig.serialNumber= global_data.param[PARAM_SERIAL_NUMBER];
			break;
			case PARAM_PITCH_P:
				gConfig.pitchKp=global_data.param[PARAM_PITCH_P];
			break;
			case	PARAM_PITCH_I:
				gConfig.pitchKi= 4*global_data.param[PARAM_PITCH_I];
			break;
			case PARAM_PITCH_D:
				gConfig.pitchKd=global_data.param[PARAM_PITCH_D] ;
			break;
			case PARAM_ROLL_P:
				gConfig.rollKp=global_data.param[PARAM_ROLL_P];
			break;
			case	PARAM_ROLL_I:
				gConfig.rollKi=global_data.param[PARAM_ROLL_I]	;
			break;
			case PARAM_ROLL_D:
				gConfig.rollKd=global_data.param[PARAM_ROLL_D];
			break;
			case PARAM_YAW_P:
				gConfig.yawKp=global_data.param[PARAM_YAW_P];
			break;
			case PARAM_YAW_I:
				gConfig.yawKi=4*global_data.param[PARAM_YAW_I];
			break;
			case PARAM_YAW_D:
				gConfig.yawKd=global_data.param[PARAM_YAW_D];
			break;
			case PARAM_PITCH_POWER:
				gConfig.pitchPower=global_data.param[PARAM_PITCH_POWER]/100.0;
			break;
			case PARAM_ROLL_POWER:
				gConfig.rollPower=global_data.param[PARAM_ROLL_POWER]/100.0;
			break;
			case PARAM_YAW_POWER:
				gConfig.yawPower=global_data.param[PARAM_YAW_POWER]/100.0;
			break;
			case PARAM_PITCH_FOLLOW:
				gConfig.pitchFollow=global_data.param[PARAM_PITCH_FOLLOW]/1000.0;
			break;
			case PARAM_ROLL_FOLLOW:
				gConfig.rollFollow=global_data.param[PARAM_ROLL_FOLLOW]/1000.0;
			break;
			case PARAM_YAW_FOLLOW:
				gConfig.yawFollow=global_data.param[PARAM_YAW_FOLLOW]/1000.0;
			break;
			case PARAM_PITCH_FILTER:
				gConfig.tiltFilter=global_data.param[PARAM_PITCH_FILTER]/1000.0;
			break;
			case PARAM_ROLL_FILTER:
				gConfig.rollFilter=global_data.param[PARAM_ROLL_FILTER]/1000.0;
			break;
			case PARAM_YAW_FILTER:
				gConfig.panFilter=global_data.param[PARAM_YAW_FILTER]/1000.0;
			break;
			case PARAM_GYRO_TRUST:
				gConfig.gyroTrust=0.97+0.03*global_data.param[PARAM_GYRO_TRUST]/255.0;		 // range 0.97 to 1
			break;
			case PARAM_NPOLES_PITCH:
				gConfig.nPolesPitch=global_data.param[PARAM_NPOLES_PITCH];
			break;
			case PARAM_NPOLES_ROLL:
				gConfig.nPolesRoll=global_data.param[PARAM_NPOLES_ROLL];
			break;
			case PARAM_NPOLES_YAW:
				gConfig.nPolesYaw=global_data.param[PARAM_NPOLES_YAW];
			break;
			case PARAM_DIR_MOTOR_PITCH:
			{
				if (global_data.param[PARAM_DIR_MOTOR_PITCH]==CONFIG_MOTOR_DIR_NORMAL)
					gConfig.dirMotorPitch=1; 
				else gConfig.dirMotorPitch=-1 ;
			}
			break;
			case PARAM_DIR_MOTOR_ROLL:
			{
				if (global_data.param[PARAM_DIR_MOTOR_ROLL]==CONFIG_MOTOR_DIR_NORMAL)
						gConfig.dirMotorRoll=1; else gConfig.dirMotorRoll=-1;
			}
			break;
			case PARAM_DIR_MOTOR_YAW:
			{
				if (global_data.param[PARAM_DIR_MOTOR_YAW]==1)
					gConfig.dirMotorYaw=1 ;else gConfig.dirMotorYaw=-1;
			}
			break;
			case PARAM_MOTOR_FREQ:
			{
				gConfig.motorFreq=global_data.param[PARAM_MOTOR_FREQ]	;
				if (gConfig.motorFreq==CONFIG_MOTOR_FREQ_36KHZ)
				{
					TIM1->ARR = MOTOR_FREQ_36KHZ;
					TIM3->ARR = MOTOR_FREQ_36KHZ;
					TIM4->ARR = MOTOR_FREQ_36KHZ;
				} 
				else if (gConfig.motorFreq==CONFIG_MOTOR_FREQ_18KHZ)
				{
					TIM1->ARR=MOTOR_FREQ_18KHZ;
					TIM3->ARR=MOTOR_FREQ_18KHZ;
					TIM4->ARR=MOTOR_FREQ_18KHZ;
				}
				else
				{
					TIM1->ARR=MOTOR_FREQ_9KHZ;
					TIM3->ARR=MOTOR_FREQ_9KHZ;
					TIM4->ARR=MOTOR_FREQ_9KHZ;
				}
			}
			break;
			case PARAM_RADIO_TYPE:
				gConfig.radioType=global_data.param[PARAM_RADIO_TYPE]	;
			break;
			case PARAM_GYRO_LPF:
				gConfig.gyroLPF=global_data.param[PARAM_GYRO_LPF]		 ;
			break;
			case PARAM_TRAVEL_MIN_PITCH:
				gConfig.travelMinPitch=global_data.param[PARAM_TRAVEL_MIN_PITCH]  ;
			break;
			case PARAM_TRAVEL_MAX_PITCH:
				gConfig.travelMaxPitch=global_data.param[PARAM_TRAVEL_MAX_PITCH]  ;
			break;
			case PARAM_TRAVEL_MIN_ROLL:
				gConfig.travelMinRoll=global_data.param[PARAM_TRAVEL_MIN_ROLL]  ;
			break;
			case PARAM_TRAVEL_MAX_ROLL:
				gConfig.travelMaxRoll=global_data.param[PARAM_TRAVEL_MAX_ROLL]  ;
			break;
			case PARAM_TRAVEL_MIN_YAW:
				gConfig.travelMinYaw=global_data.param[PARAM_TRAVEL_MIN_YAW]  ;
			break;
			case PARAM_TRAVEL_MAX_YAW:
				gConfig.travelMaxYaw=global_data.param[PARAM_TRAVEL_MAX_YAW]  ;
			break;
			case PARAM_RC_PITCH_LPF:
				gConfig.rcPitchLPF=global_data.param[PARAM_RC_PITCH_LPF]  ;
			break;
			case PARAM_RC_ROLL_LPF:
				gConfig.rcRollLPF=global_data.param[PARAM_RC_ROLL_LPF]  ;
			break;
			case PARAM_RC_YAW_LPF:
				gConfig.rcYawLPF=global_data.param[PARAM_RC_YAW_LPF]  ;
			break;
      case PARAM_SBUS_PITCH_CHAN:
				gConfig.sbusPitchChan=global_data.param[PARAM_SBUS_PITCH_CHAN]  ;
			break;
			case PARAM_SBUS_ROLL_CHAN:
				gConfig.sbusRollChan=global_data.param[PARAM_SBUS_ROLL_CHAN]  ;
			break;
			case PARAM_SBUS_YAW_CHAN:
				gConfig.sbusYawChan=global_data.param[PARAM_SBUS_YAW_CHAN]  ;
			break;
			case PARAM_SBUS_MODE_CHAN:
				gConfig.sbusModeChan=global_data.param[PARAM_SBUS_MODE_CHAN]  ;
			break;
			case PARAM_ACCX_OFFSET:
			{
				gConfig.accXOffset=global_data.param[ PARAM_ACCX_OFFSET]  ;
				gSensor.accXOffset=gConfig.accXOffset;
			}
			break;
      case PARAM_ACCY_OFFSET:
			{
				gConfig.accYOffset=global_data.param[ PARAM_ACCY_OFFSET]  ;
				gSensor.accYOffset=gConfig.accYOffset;
			}
			break;
			case PARAM_ACCZ_OFFSET:
			{
				gConfig.accZOffset=global_data.param[ PARAM_ACCZ_OFFSET]  ;
				gSensor.accZOffset=gConfig.accZOffset;
			}
			break;
			case PARAM_GYROX_OFFSET:
				gConfig.gyroXOffset=global_data.param[ PARAM_GYROX_OFFSET]  ;
			break;
      case PARAM_GYROY_OFFSET:
				gConfig.gyroYOffset=global_data.param[ PARAM_GYROY_OFFSET]  ;
			break;
			case PARAM_GYROZ_OFFSET:
				gConfig.gyroZOffset=global_data.param[ PARAM_GYROZ_OFFSET]  ;
			break;
			case PARAM_USE_GPS:
				gConfig.useGPS=global_data.param[ PARAM_USE_GPS]  ;
			break;
			case PARAM_SKIP_GYRO_CALIB:
				gConfig.skipGyroCalib=global_data.param[ PARAM_SKIP_GYRO_CALIB]  ;
			break;								
			case PARAM_RC_PITCH_TRIM:
				gConfig.rcPitchTrim=global_data.param[ PARAM_RC_PITCH_TRIM] - 100 ;
			break;
			case PARAM_RC_ROLL_TRIM:
				gConfig.rcRollTrim=global_data.param[ PARAM_RC_ROLL_TRIM] - 100 ;
			break;
			case PARAM_RC_YAW_TRIM:
				gConfig.rcYawTrim=global_data.param[ PARAM_RC_YAW_TRIM] - 100 ;
			break;
			case PARAM_RC_PITCH_MODE:
				gConfig.rcPitchMode=global_data.param[ PARAM_RC_PITCH_MODE]  ;
			break;
			case PARAM_RC_ROLL_MODE:
				gConfig.rcRollMode=global_data.param[ PARAM_RC_ROLL_MODE]  ;
			break;
			case PARAM_RC_YAW_MODE:
				gConfig.rcYawMode=global_data.param[ PARAM_RC_YAW_MODE]  ;
			break;
			case PARAM_TILT_WINDOW:
				gConfig.tiltWindow = 	global_data.param[PARAM_TILT_WINDOW];
			break;
			case PARAM_PAN_WINDOW:
				gConfig.panWindow	=		global_data.param[PARAM_PAN_WINDOW];
			break;
			
			case PARAM_SBUS_FUNC_CHAN:
				gConfig.sbusFuncChan	=		global_data.param[PARAM_SBUS_FUNC_CHAN];
			break;
			case PARAM_RC_PITCH_SPEED:
				gConfig.rcPitchSpeed	=	(100 - global_data.param[PARAM_RC_PITCH_SPEED])/10.0 + 1;
			break;
			case PARAM_RC_ROLL_SPEED:
				gConfig.rcRollSpeed	=		(100 - global_data.param[PARAM_RC_ROLL_SPEED])/10.0  + 1;
			break;
			case PARAM_RC_YAW_SPEED:
				gConfig.rcYawSpeed	=		(100 - global_data.param[PARAM_RC_YAW_SPEED])/10.0   + 1;
			break;
			case PARAM_JOY_AXIS:
				gConfig.joyAxis = global_data.param[PARAM_JOY_AXIS];
			break;
			case PARAM_TRIM_HOZ:
				gConfig.rcTrimHoz = (global_data.param[PARAM_TRIM_HOZ] - 100)/10.0;
			break;
			case PARAM_TRIM_TILT:
				gConfig.rcTrimTilt = (global_data.param[PARAM_TRIM_TILT] - 100)/10.0;
			break;
			case PARAM_TRIM_PAN:
				gConfig.rcTrimPan = (global_data.param[PARAM_TRIM_PAN] - 100)/10.0;
			break;
			case PARAM_SBUS_YAW_SPEED_CHAN:
				gConfig.sbusYawSpeedChan = global_data.param[PARAM_SBUS_YAW_SPEED_CHAN];
			break;
			case PARAM_SBUS_PITCH_SPEED_CHAN:
				gConfig.sbusPitchSpeedChan = global_data.param[PARAM_SBUS_PITCH_SPEED_CHAN];
			break;
	 }
 }

void mavlinkSend(void){
  if (gMav.sendFinish)
  {
		 if (gMav.sendType==gMAV_SEND_MSG)
     {			 
				switch(gMav.msgIndex)
				{
						case 0:
						{
 							if(gMav.valueChange & HEART_BIT_MASK)
							{
								gMav.valueChange &= (~HEART_BIT_MASK);
								mavlink_msg_heartbeat_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg, MAV_TYPE_FIXED_WING, MAV_AUTOPILOT_GENERIC, MAV_MODE_AUTO_ARMED, 0, MAV_STATE_ACTIVE);
								len = mavlink_msg_to_send_buffer(buffer, &msg);
								_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex=1;
						}
						break;
	    			case 1:
						{
							mavlink_msg_raw_imu_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,0, accx, accy, accz, gyrox, gyroy, gyroz,errorTilt*100,errorRoll*100,errorPan*100);
							len = mavlink_msg_to_send_buffer(buffer, &msg);
							_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
						
							gMav.msgIndex=2;
						}
						break;
						case 2:
						{
							mavlink_msg_attitude_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg, 0,gAtti.theta*3.14159/180.0,gAtti.phi*3.14159/180.0, gAtti.psi*3.14159/180.0, gAtti.thetaDot*3.14159/180.0, gAtti.phiDot*3.14159/180.0, gAtti.psiDot*3.14159/180.0);
							len = mavlink_msg_to_send_buffer(buffer, &msg);
							_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							gMav.msgIndex=3;
						}
						break;
						case 3:
						{
 							if(gMav.valueChange & SYS_STATUS_MASK)
							{
								 gMav.valueChange &= (~SYS_STATUS_MASK);
									mavlink_msg_system_status_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg, gBattery.volCurrent, gState.numFace + gState.gyroCalibrated - 1, 0, gState.status, 0); // chua co ve tinh, con trong status2
									len = mavlink_msg_to_send_buffer(buffer, &msg);
									_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex = 4;
						}
						break;
						case 4:
						{
// 							if(gMav.valueChange & PPM_MASK)
//							{
//									gMav.valueChange &= (~PPM_MASK);
//									mavlink_msg_ppm_chan_values_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,gRadio.ppmChan[PPM_TILT],gRadio.ppmChan[PPM_PAN],gRadio.ppmChan[PPM_ROLL],gRadio.ppmChan[PPM_MODE]);
//									len = mavlink_msg_to_send_buffer(buffer, &msg);
//									_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
//							}
							gMav.msgIndex = 5;
						}
						break;
						case 5:
						{
 							if(gMav.valueChange & SBUS_MASK)
							{
									gMav.valueChange &= (~SBUS_MASK);
									mavlink_msg_sbus_chan_values_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,gRadio.sbusChan[0],gRadio.sbusChan[1],gRadio.sbusChan[2],
													gRadio.sbusChan[3],gRadio.sbusChan[4],gRadio.sbusChan[5],gRadio.sbusChan[6],gRadio.sbusChan[7],gRadio.sbusChan[8],
													gRadio.sbusChan[9],gRadio.sbusChan[10],gRadio.sbusChan[11],gRadio.sbusChan[12],gRadio.sbusChan[13],gRadio.sbusChan[14],
													gRadio.sbusChan[15],gRadio.sbusChan[16],gRadio.sbusChan[17]);
									len = mavlink_msg_to_send_buffer(buffer, &msg);
									_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex = 6;
						}
						break;
						case 6:
						{
 							if(gMav.valueChange & DEBUG_MASK)
							{
								gMav.valueChange &= (~DEBUG_MASK);
//								mavlink_msg_debug_values_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg, (hall_r.vel/hall_l.vel), pidRight.out, hall_r.distance, hall_l.distance,0, hall_r.count, hall_l.count,data_sensor1);
								mavlink_msg_debug_values_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg, (hall_r.vel/hall_l.vel), hall_l.count, hall_r.count, hall_l.distance,sensor_end, sensor_head, data_sensor2,data_sensor1);
								len = mavlink_msg_to_send_buffer(buffer, &msg);
								_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
  						gMav.msgIndex = 7;
						}
						break;
						//working on debug code unique id
						case 7:
						{
 							if((gMav.valueChange & UDID_MASK) == 1)   //check bit udid of value changed that trigger this code
							{
								gMav.valueChange &= (~UDID_MASK);			//reset bit udid of value changed that trigger this code
								mavlink_msg_unique_id_values_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,GSTABI,unique[0],unique[1],unique[2],unique[3],unique[4],unique[5]);
								len = mavlink_msg_to_send_buffer(buffer, &msg);
								_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex = 8;
						}
						break;
						case 8:
						{
							if(gMav.valueChange & KEY_REQUEST_MASK){
								gMav.valueChange &= (~KEY_REQUEST_MASK);
								mavlink_msg_keycode_request_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,GSTABI);
								len = mavlink_msg_to_send_buffer(buffer, &msg);
								_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex = 9;
						}
						break;
						case 9:{
							if(gMav.valueChange & GPS_MASK)
							{
								gMav.valueChange &= (~GPS_MASK);
								mavlink_msg_gps_raw_int_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,gUblox.Time,gUblox.Fix,gUblox.Lattitude,gUblox.Longitude,gUblox.Altitude,0,0,0,0,gUblox.NumSats);
								len = mavlink_msg_to_send_buffer(buffer, &msg);
								_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
							}
							gMav.msgIndex = 10;
						}
						break;
						default:
						{
							gMav.msgIndex = 0;  gMav.sendType=gMAV_SEND_NONE;
						}
						break;
	      }
   }
	 
	 if (gMav.sendType==gMAV_SEND_PARAM)
	 {
			if(global_data.parameter_i < ONBOARD_PARAM_COUNT)
			{
				mavlink_msg_param_value_pack(SYSTEM_ID, MAV_COMP_ID_gGIMBAL, &msg,(const char*) global_data.param_name[global_data.parameter_i],global_data.param[global_data.parameter_i],MAVLINK_TYPE_FLOAT,ONBOARD_PARAM_COUNT,global_data.parameter_i );
				len = mavlink_msg_to_send_buffer(buffer, &msg);
				_mavlink_send_uart(MAVLINK_COMM_0,(const char*) buffer, len);
				global_data.parameter_i++;
			}
			else
			{
				gMav.update = 0;
				gMav.sendType=gMAV_SEND_NONE;
			}
		}
	}
}

void getParamsDefault(void){	
  global_data.param_name[PARAM_VERSION]= 					"VERSION";
  global_data.param_name[PARAM_SERIAL_NUMBER]= 		"SERIAL_NUMBER";
  global_data.param_name[PARAM_PITCH_P]= 					"PITCH_P";
  global_data.param_name[PARAM_PITCH_I]= 					"PITCH_I";
  global_data.param_name[PARAM_PITCH_D]= 					"PITCH_D" ;
	global_data.param_name[PARAM_ROLL_P]=						"ROLL_P";
	global_data.param_name[PARAM_ROLL_I]=	 					"ROLL_I";
	global_data.param_name[PARAM_ROLL_D]=						"ROLL_D";
  global_data.param_name[PARAM_YAW_P]=						"YAW_P";
	global_data.param_name[PARAM_YAW_I]=						"YAW_I";
	global_data.param_name[PARAM_YAW_D]	=						"YAW_D";
  global_data.param_name[PARAM_PITCH_POWER]=			"PITCH_POWER";
	global_data.param_name[PARAM_ROLL_POWER]=	 			"ROLL_POWER";
	global_data.param_name[PARAM_YAW_POWER]	=	 			"YAW_POWER";
  global_data.param_name[PARAM_PITCH_FOLLOW]=	  	"PITCH_FOLLOW";
	global_data.param_name[PARAM_ROLL_FOLLOW]=	 		"ROLL_FOLLOW";
	global_data.param_name[PARAM_YAW_FOLLOW]=	 			"YAW_FOLLOW";
	global_data.param_name[PARAM_PITCH_FILTER]=	  	"PITCH_FILTER";
	global_data.param_name[PARAM_ROLL_FILTER]=	 		"ROLL_FILTER";
	global_data.param_name[PARAM_YAW_FILTER]=	 			"YAW_FILTER";
	global_data.param_name[PARAM_GYRO_TRUST]= 			"GYRO_TRUST";
  global_data.param_name[PARAM_NPOLES_PITCH]=  		"NPOLES_PITCH";
	global_data.param_name[PARAM_NPOLES_ROLL]=			"NPOLES_ROLL";
	global_data.param_name[PARAM_NPOLES_YAW]=				"NPOLES_YAW";
  global_data.param_name[PARAM_DIR_MOTOR_PITCH] = "DIR_MOTOR_PITCH";
	global_data.param_name[PARAM_DIR_MOTOR_ROLL]  = "DIR_MOTOR_ROLL";
	global_data.param_name[PARAM_DIR_MOTOR_YAW]	  =	"DIR_MOTOR_YAW";
	global_data.param_name[PARAM_MOTOR_FREQ]	 = 		"MOTOR_FREQ";
	global_data.param_name[PARAM_RADIO_TYPE]		 = 	"RADIO_TYPE";
	global_data.param_name[PARAM_GYRO_LPF]		 = 		"GYRO_LPF";
	global_data.param_name[PARAM_TRAVEL_MIN_PITCH]=	"TRAVEL_MIN_PIT";
	global_data.param_name[PARAM_TRAVEL_MAX_PITCH]= "TRAVEL_MAX_PIT";
	global_data.param_name[PARAM_TRAVEL_MIN_ROLL]=  "TRAVEL_MIN_ROLL";
	global_data.param_name[PARAM_TRAVEL_MAX_ROLL]=  "TRAVEL_MAX_ROLL";
	global_data.param_name[PARAM_TRAVEL_MIN_YAW]=  	"TRAVEL_MIN_YAW";
	global_data.param_name[PARAM_TRAVEL_MAX_YAW]=  	"TRAVEL_MAX_YAW";
  global_data.param_name[PARAM_RC_PITCH_LPF]=			"RC_PITCH_LPF";
	global_data.param_name[PARAM_RC_ROLL_LPF]=			"RC_ROLL_LPF";
	global_data.param_name[PARAM_RC_YAW_LPF]=  			"RC_YAW_LPF";
	global_data.param_name[PARAM_SBUS_PITCH_CHAN]=	"SBUS_PITCH_CHAN";
	global_data.param_name[PARAM_SBUS_ROLL_CHAN]=		"SBUS_ROLL_CHAN";
	global_data.param_name[PARAM_SBUS_YAW_CHAN]=		"SBUS_YAW_CHAN";
	global_data.param_name[PARAM_SBUS_MODE_CHAN]=		"SBUS_MODE_CHAN";
	global_data.param_name[PARAM_ACCX_OFFSET]=			"ACCX_OFFSET";
  global_data.param_name[PARAM_ACCY_OFFSET]=			"ACCY_OFFSET";
	global_data.param_name[PARAM_ACCZ_OFFSET]=			"ACCZ_OFFSET";
	global_data.param_name[PARAM_GYROX_OFFSET]=			"GYROX_OFFSET";
  global_data.param_name[PARAM_GYROY_OFFSET]=			"GYROY_OFFSET";
	global_data.param_name[PARAM_GYROZ_OFFSET]=			"GYROZ_OFFSET";
	global_data.param_name[PARAM_USE_GPS]=					"USE_GPS";
	global_data.param_name[PARAM_SKIP_GYRO_CALIB]=	"SKIP_GYRO_CALIB";
	global_data.param_name[PARAM_RC_PITCH_TRIM]=		"RC_PITCH_TRIM";
	global_data.param_name[PARAM_RC_ROLL_TRIM]=			"RC_ROLL_TRIM";
	global_data.param_name[PARAM_RC_YAW_TRIM]=			"RC_YAW_TRIM";
	global_data.param_name[PARAM_RC_PITCH_MODE]=		"RC_PITCH_MODE";
	global_data.param_name[PARAM_RC_ROLL_MODE]=			"RC_ROLL_MODE";
	global_data.param_name[PARAM_RC_YAW_MODE]=			"RC_YAW_MODE";
	global_data.param_name[PARAM_TILT_WINDOW]=			"TILT_WINDOW";
	global_data.param_name[PARAM_PAN_WINDOW]=				"PAN_WINDOW";
	
	global_data.param_name[PARAM_SBUS_FUNC_CHAN]=		"SBUS_FUNC_CHAN";
	global_data.param_name[PARAM_RC_PITCH_SPEED]=		"RC_PITCH_SPEED";
	global_data.param_name[PARAM_RC_ROLL_SPEED]=		"RC_ROLL_SPEED";
	global_data.param_name[PARAM_RC_YAW_SPEED]=			"RC_YAW_SPEED";
	global_data.param_name[PARAM_JOY_AXIS]=					"JOY_AXIS";
	global_data.param_name[PARAM_TRIM_HOZ]=					"TRIM_HOZ";
	global_data.param_name[PARAM_TRIM_TILT]=				"TRIM_TILT";
	global_data.param_name[PARAM_TRIM_PAN]=					"TRIM_PAN";
	global_data.param_name[PARAM_SBUS_YAW_SPEED_CHAN]=	"SBUS_YAW_SPEED_CHAN";
	global_data.param_name[PARAM_SBUS_PITCH_SPEED_CHAN]=	"SBUS_PITCH_SPEED_CHAN";
	

	global_data.param[PARAM_VERSION]= 			DEFAULT_VERSION;
  global_data.param[PARAM_SERIAL_NUMBER]= DEFAULT_SERIAL_NUMBER;
  global_data.param[PARAM_PITCH_P]= 			DEFAULT_PITCH_P;
  global_data.param[PARAM_PITCH_I]= 			DEFAULT_PITCH_I;
  global_data.param[PARAM_PITCH_D]= 			DEFAULT_PITCH_D;
	global_data.param[PARAM_ROLL_P]=				DEFAULT_ROLL_P;
	global_data.param[PARAM_ROLL_I]=				DEFAULT_ROLL_I;
	global_data.param[PARAM_ROLL_D]=				DEFAULT_ROLL_D;
  global_data.param[PARAM_YAW_P]=					DEFAULT_YAW_P;
	global_data.param[PARAM_YAW_I]=					DEFAULT_YAW_I;
	global_data.param[PARAM_YAW_D]	=				DEFAULT_YAW_D;
  global_data.param[PARAM_PITCH_POWER]=		DEFAULT_PITCH_POWER;
	global_data.param[PARAM_ROLL_POWER]=	 	DEFAULT_ROLL_POWER;
	global_data.param[PARAM_YAW_POWER]	=	 	DEFAULT_YAW_POWER;
  global_data.param[PARAM_PITCH_FOLLOW]=	DEFAULT_PITCH_FOLLOW;
	global_data.param[PARAM_ROLL_FOLLOW]=	 	DEFAULT_ROLL_FOLLOW;
	global_data.param[PARAM_YAW_FOLLOW]=	 	DEFAULT_YAW_FOLLOW;
	global_data.param[PARAM_PITCH_FILTER]=	DEFAULT_PITCH_FILTER;
	global_data.param[PARAM_ROLL_FILTER]=	 	DEFAULT_ROLL_FILTER;
	global_data.param[PARAM_YAW_FILTER]=	 	DEFAULT_YAW_FILTER;
	global_data.param[PARAM_GYRO_TRUST]= 		DEFAULT_GYRO_TRUST;
  global_data.param[PARAM_NPOLES_PITCH]=  DEFAULT_NPOLES_PITCH;
	global_data.param[PARAM_NPOLES_ROLL]=		DEFAULT_NPOLES_ROLL;
	global_data.param[PARAM_NPOLES_YAW]=		DEFAULT_NPOLES_YAW;
  global_data.param[PARAM_DIR_MOTOR_PITCH] = 	DEFAULT_DIR_MOTOR_PITCH;
	global_data.param[PARAM_DIR_MOTOR_ROLL]  = 	DEFAULT_DIR_MOTOR_ROLL;
	global_data.param[PARAM_DIR_MOTOR_YAW]	 = 	DEFAULT_DIR_MOTOR_YAW;
	global_data.param[PARAM_MOTOR_FREQ]	= 			DEFAULT_MOTOR_FREQ	;
	global_data.param[PARAM_RADIO_TYPE]	= 			DEFAULT_RADIO_TYPE;
	global_data.param[PARAM_GYRO_LPF]		= 			DEFAULT_GYRO_LPF ;
	global_data.param[PARAM_TRAVEL_MIN_PITCH]=	DEFAULT_TRAVEL_MIN_PITCH ;
	global_data.param[PARAM_TRAVEL_MAX_PITCH]= 	DEFAULT_TRAVEL_MAX_PITCH;
	global_data.param[PARAM_TRAVEL_MIN_ROLL]= 	DEFAULT_TRAVEL_MIN_ROLL;
	global_data.param[PARAM_TRAVEL_MAX_ROLL]=  	DEFAULT_TRAVEL_MAX_ROLL;
	global_data.param[PARAM_TRAVEL_MIN_YAW]=  	DEFAULT_TRAVEL_MIN_YAW;
	global_data.param[PARAM_TRAVEL_MAX_YAW]=  	DEFAULT_TRAVEL_MAX_YAW;
  global_data.param[PARAM_RC_PITCH_LPF]=			DEFAULT_RC_PITCH_LPF;
	global_data.param[PARAM_RC_ROLL_LPF]=				DEFAULT_RC_ROLL_LPF;
	global_data.param[PARAM_RC_YAW_LPF]=  			DEFAULT_RC_YAW_LPF;
	global_data.param[PARAM_SBUS_PITCH_CHAN]=		DEFAULT_SBUS_PITCH_CHAN;
	global_data.param[PARAM_SBUS_ROLL_CHAN]=		DEFAULT_SBUS_ROLL_CHAN;
	global_data.param[PARAM_SBUS_YAW_CHAN]=			DEFAULT_SBUS_YAW_CHAN;
	global_data.param[PARAM_SBUS_MODE_CHAN]=		DEFAULT_SBUS_MODE_CHAN;
	global_data.param[PARAM_ACCX_OFFSET]=				DEFAULT_ACCX_OFFSET;
  global_data.param[PARAM_ACCY_OFFSET]=				DEFAULT_ACCY_OFFSET;
	global_data.param[PARAM_ACCZ_OFFSET]=				DEFAULT_ACCZ_OFFSET;
	global_data.param[PARAM_GYROX_OFFSET]=			DEFAULT_GYROX_OFFSET;
  global_data.param[PARAM_GYROY_OFFSET]=			DEFAULT_GYROY_OFFSET;
	global_data.param[PARAM_GYROZ_OFFSET]=			DEFAULT_GYROZ_OFFSET;
	global_data.param[PARAM_USE_GPS]=						DEFAULT_USE_GPS;
	global_data.param[PARAM_SKIP_GYRO_CALIB]=		DEFAULT_SKIP_GYRO_CALIB;
	global_data.param[PARAM_RC_PITCH_TRIM]=			DEFAULT_RC_PITCH_TRIM;
	global_data.param[PARAM_RC_ROLL_TRIM]=			DEFAULT_RC_ROLL_TRIM;
	global_data.param[PARAM_RC_YAW_TRIM]=				DEFAULT_RC_YAW_TRIM;
	global_data.param[PARAM_RC_PITCH_MODE]=			DEFAULT_RC_PITCH_MODE;
	global_data.param[PARAM_RC_ROLL_MODE]=			DEFAULT_RC_ROLL_MODE;
	global_data.param[PARAM_RC_YAW_MODE]=				DEFAULT_RC_YAW_MODE;
	global_data.param[PARAM_TILT_WINDOW]=				DEFAULT_TILT_WINDOW;
	global_data.param[PARAM_PAN_WINDOW]=				DEFAULT_PAN_WINDOW;	
	
	global_data.param[PARAM_SBUS_FUNC_CHAN] = DEFAULT_SBUS_FUNC;
	global_data.param[PARAM_RC_PITCH_SPEED] = DEFAULT_RC_PITCH_SPEED;
	global_data.param[PARAM_RC_ROLL_SPEED]  = DEFAULT_RC_ROLL_SPEED;
	global_data.param[PARAM_RC_YAW_SPEED]   = DEFAULT_RC_YAW_SPEED;
	global_data.param[PARAM_JOY_AXIS] = 	DEFAULT_JOY_AXIS;
	global_data.param[PARAM_TRIM_HOZ] = 	DEFAULT_TRIM_HOZ;
	global_data.param[PARAM_TRIM_TILT] = 	DEFAULT_TRIM_TILT;
	global_data.param[PARAM_TRIM_PAN] = 	DEFAULT_TRIM_PAN;
	
	global_data.param[PARAM_SBUS_YAW_SPEED_CHAN] = 	DEFAULT_SBUS_YAW_SPEED_CHAN;
	global_data.param[PARAM_SBUS_PITCH_SPEED_CHAN] = DEFAULT_SBUS_PITCH_SPEED_CHAN;
}

void getParamsFromFlashAfterPowerOn(void){  
	uint16_t i;
  int16_t param_value;
   for (i=0;i<ONBOARD_PARAM_COUNT;i++)
	 {   
		 VirtAddVarTab[i]=i+1;
   }
	 
   FLASH_Unlock();
   EE_Init();
    
   getParamsDefault();
   for (i=0;i<ONBOARD_PARAM_COUNT;i++)
   {   
			 if(!EE_ReadVariable(VirtAddVarTab[i],(uint16_t *)&param_value))
			 global_data.param[i] = (float)param_value;
			 param2Config(i);
   }	
  if (gConfig.skipGyroCalib==CONFIG_SKIP_GYRO_CALIB)
	{
		gState.gyroCalibrated=1;
		gSensor.gyroXOffset=global_data.param[PARAM_GYROX_OFFSET];
		gSensor.gyroYOffset=global_data.param[PARAM_GYROY_OFFSET];
		gSensor.gyroZOffset=global_data.param[PARAM_GYROZ_OFFSET];
	} 
	else{
		gState.gyroCalibrated=0;
	}
   	
    
    global_data.parameter_i = 0;
}

void handle_mavlink_message(mavlink_channel_t chan, mavlink_message_t* msg){
	char* key;
	uint16_t i;
    //uint16_t m_parameter_i = 0;
	
	switch (msg->msgid)
	{
		case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
		{
			// Start sending parameters
			global_data.parameter_i=0;
			gMav.sendType=gMAV_SEND_PARAM;
		}
		break;
		case MAVLINK_MSG_ID_PARAM_SET:
		{
				mavlink_param_set_t set;
				mavlink_msg_param_set_decode(msg, &set);
				
				key = (char*) set.param_id;
				for (i = 0; i < ONBOARD_PARAM_COUNT; i++)
				{		
					if (!strcmp(key, global_data.param_name[i]))  //two strings are equal
					{					
						// Only write and emit changes if there is actually a difference
						// AND only write if new value is NOT "not-a-number"
						// AND is NOT infinity
							
						if (global_data.param[i] != set.param_value	&& !isnan(set.param_value)	&& !isinf(set.param_value))
						{
							global_data.param[i] = set.param_value;
							EE_WriteVariable(VirtAddVarTab[i], (int16_t)set.param_value);
							param2Config(i);
						}
						
					}
				}
		}
		break;
		case MAVLINK_MSG_ID_RC_SIMULATION:
		{			
			gRadio.pcChan[PC_TILT_CHANNEL] = 	mavlink_msg_rc_simulation_get_tilt(msg);
			gRadio.pcChan[PC_ROLL_CHANNEL] = 	mavlink_msg_rc_simulation_get_roll(msg);
			gRadio.pcChan[PC_PAN_CHANNEL] = 	mavlink_msg_rc_simulation_get_pan(msg);
			gRadio.noPcCount=0;
		}
		break;
		case MAVLINK_MSG_ID_IMU_CALIB_REQUEST:
		{
			mavlink_imu_calib_request_t calibRequest;
			calibRequest.calib_type = mavlink_msg_imu_calib_request_get_calib_type(msg);
			
			if(calibRequest.calib_type == 0){ // calib acc
				gState.numFaceAcc = mavlink_msg_imu_calib_request_get_acc_calib_mode(msg);

				if(gState.numFaceAcc == 0) 	{gState.numFaceAccCalib = 1;gState.numFace = 1;}
				else												gState.numFaceAccCalib++;
				
				if(gState.numFaceAccCalib > 7) gState.numFaceAccCalib = 7;
				
				cntCalAcc = 0;
			}
			else if(calibRequest.calib_type == 1){ // calib gyro
				gState.gyroCalibrated = 0;
			}
		}
		break;
		case MAVLINK_MSG_ID_UNIQUE_ID_REQUEST:
		{
			if(mavlink_msg_unique_id_request_get_device_name1(msg) == GSTABI)	gMav.valueChange |= UDID_MASK;
		}
		break;

		case MAVLINK_MSG_ID_MOTORS_CONTROL:
		{
			ctrMode = mavlink_msg_motors_control_get_command(msg);
			
			if(ctrMode & CTR_TURN_ONOFF){
				gMotor.power = 1;
			}
			else if(!(ctrMode & CTR_TURN_ONOFF)){
				gMotor.power = 0;
			}
			
			if(ctrMode & CTR_CLEAR_MISS_STEP){
				gMotor.updateTheta = 0;
			}
		}
		break;

	}
}

int mavlinkReceive(void)
{
	int i, k, numc;
	static int data_counter_1=0;
	int data_counter;
	
	data_counter=MAV_RX_BUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel5);

	numc=data_counter-data_counter_1;
	if (numc<0) numc=MAV_RX_BUFF_SIZE+numc;
	if (numc)
	{ 
		 
		 
		for (i=0;i<numc;i++)
		{  
			k=i+data_counter_1;
			if (k>=MAV_RX_BUFF_SIZE)   k-=MAV_RX_BUFF_SIZE;
			mav_rx_tmp[i]=mav_rx_buffer[k] ;
		}
		 
		data_counter_1=data_counter;
		for(i = 0; i < numc; i++)	// Process bytes received
		{
			
			if(mavlink_parse_char(MAVLINK_COMM_0,mav_rx_tmp[i],&msg,&status))
			{
				handle_mavlink_message(MAVLINK_COMM_0, &msg);	
			}
		}
	}
	 return numc;
}
