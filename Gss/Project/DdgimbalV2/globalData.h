/**
  ******************************************************************************
  * @file: 		globalData.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho globalData.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#ifndef  GLOBALDATA_H 
#define  GLOBALDATA_H

#include "stm32f10x.h"
#include "eeprom.h"
#include <math.h>

#define THROTTLE 2
#define ELEVATOR 1
#define AILERON 0
#define RUDDER 3
#define GEAR 4
#define VPP 5
#define AUX1   5
#define AUX2   6
#define AUX3   8
#define AUX4   7
#define PI  3.141592654
#define GRAVITY 4166.0
#define SYSTEM_ID 10
#define MAV_RX_BUFF_SIZE 100

#define ONBOARD_PARAM_COUNT    70
#define ONBOARD_PARAM_NAME_LENGTH 16
#define ANGLE2PI	PI/180.0
#define PI2ANGLE	180.0/PI
#define UBX_MAXPAYLOAD 60
#define GPS_BUFF_SIZE 200
 


enum gMAV_ENUM
{
	gMAV_SEND_MSG,
  gMAV_SEND_PARAM,
  gMAV_SEND_NONE,
};
enum
{ 
	PARAM_VERSION,
	PARAM_SERIAL_NUMBER,
	PARAM_PITCH_P,
	PARAM_PITCH_I,
	PARAM_PITCH_D,
	PARAM_ROLL_P,
	PARAM_ROLL_I,
	PARAM_ROLL_D,
	PARAM_YAW_P,
	PARAM_YAW_I,
	PARAM_YAW_D,
	PARAM_PITCH_POWER,
	PARAM_ROLL_POWER,
	PARAM_YAW_POWER,
	PARAM_PITCH_FOLLOW,
	PARAM_ROLL_FOLLOW,
	PARAM_YAW_FOLLOW,
	PARAM_PITCH_FILTER,
	PARAM_ROLL_FILTER,
	PARAM_YAW_FILTER,
	PARAM_GYRO_TRUST,
	PARAM_NPOLES_PITCH,
	PARAM_NPOLES_ROLL,
	PARAM_NPOLES_YAW,
	PARAM_DIR_MOTOR_PITCH,
	PARAM_DIR_MOTOR_ROLL,
	PARAM_DIR_MOTOR_YAW,
	PARAM_MOTOR_FREQ,
	PARAM_RADIO_TYPE,
	PARAM_GYRO_LPF,
	PARAM_TRAVEL_MIN_PITCH,
	PARAM_TRAVEL_MAX_PITCH,
	PARAM_TRAVEL_MIN_ROLL,
	PARAM_TRAVEL_MAX_ROLL,
	PARAM_TRAVEL_MIN_YAW,
	PARAM_TRAVEL_MAX_YAW,
	PARAM_RC_PITCH_LPF,
	PARAM_RC_ROLL_LPF,
	PARAM_RC_YAW_LPF,
	PARAM_SBUS_PITCH_CHAN,
	PARAM_SBUS_ROLL_CHAN,
	PARAM_SBUS_YAW_CHAN,
	PARAM_SBUS_MODE_CHAN,
	PARAM_ACCX_OFFSET,
	PARAM_ACCY_OFFSET,
	PARAM_ACCZ_OFFSET,
	PARAM_GYROX_OFFSET,
	PARAM_GYROY_OFFSET,
	PARAM_GYROZ_OFFSET,
	PARAM_USE_GPS,
	PARAM_SKIP_GYRO_CALIB,
	PARAM_RC_PITCH_TRIM,
	PARAM_RC_ROLL_TRIM,
	PARAM_RC_YAW_TRIM,
	PARAM_RC_PITCH_MODE,
	PARAM_RC_ROLL_MODE,
	PARAM_RC_YAW_MODE,
	PARAM_TILT_WINDOW,
	PARAM_PAN_WINDOW,
	PARAM_SBUS_FUNC_CHAN,
	PARAM_RC_PITCH_SPEED,
	PARAM_RC_ROLL_SPEED,
	PARAM_RC_YAW_SPEED,
	PARAM_JOY_AXIS,
	PARAM_TRIM_HOZ,
	PARAM_TRIM_TILT,
	PARAM_TRIM_PAN,
	PARAM_SBUS_YAW_SPEED_CHAN,
	PARAM_SBUS_PITCH_SPEED_CHAN,
};

enum
{ 
	gMODE_NORMAL,
  gMODE_FOLLOW_PAN,
  gMODE_FOLLOW_PAN_TILT,

}  ; 
enum
{ 
	gSTATE_CALIB_ACC,
  gSTATE_CALIB_GYRO,
  gSTATE_SENSOR_ERROR,
  gSTATE_NORMAL,
} ;
enum
{ 
	I2C_DMA_RX_WAITING,
  I2C_BUS_AVAILABLE,
}	;
enum
{ 
	//CONFIG_RADIO_TYPE_PPM 			= 0,
  CONFIG_RADIO_TYPE_SBUS 			= 0,
	//CONFIG_RADIO_TYPE_HAND 			= 2,
	CONFIG_RADIO_TYPE_PC 				= 1,
	//CONFIG_RADIO_TYPE_BUTTON		=	4,
};
enum
{ 
	CONFIG_MOTOR_FREQ_36KHZ=	0,
  CONFIG_MOTOR_FREQ_18KHZ=	1,
  CONFIG_MOTOR_FREQ_9KHZ=		2,
};
enum
{ 
	CONFIG_MOTOR_DIR_NORMAL=	1,
  CONFIG_MOTOR_DIR_REVERSE=	0,
};
enum
{ CONFIG_SKIP_GYRO_CALIB=			0,
  CONFIG_NOT_SKIP_GYRO_CALIB=	1,
};

//////////gss////////
enum
{ 
	CONFIG_RC_MODE_HAND=0,
  CONFIG_RC_MODE_LINE=1,
};
enum
{ 
	CONFIG_RC_SPEED_SLOW=0,
  CONFIG_RC_SPEED_NORMAL,
	CONFIG_RC_SPEED_FAST,
	CONFIG_RC_SPEED_SUPPER,
};
/////////////////////////


enum
{
	UDID_MASK = 0x01,
	SYS_STATUS_MASK = 0x02,
	HEART_BIT_MASK = 0x04,	
	DEBUG_MASK = 0x08,
	PPM_MASK = 0x10,
	SBUS_MASK = 0x20,
	KEY_REQUEST_MASK = 0x40,
	GPS_MASK = 0x80,
	ANGLE_MASK = 0x100,
};


enum{
	CTR_TURN_ONOFF = 0x01, 			//0000 0001
	CTR_CLEAR_MISS_STEP = 0x02,	//0000 0010
	CTR_FOLLOW_LOCK = 0x04,			//0000 0100
};

//gss//

typedef struct __global_struct
{
    float param[ONBOARD_PARAM_COUNT];
    char * param_name[ONBOARD_PARAM_COUNT]; 
    uint16_t parameter_i; // parameter index

}global_struct;

typedef struct _hall_t
{
	int8_t direct;
	int32_t count;
	float angle;
	float vel;
	uint32_t timeVel;
	uint32_t time;
	float distance;
	
	int32_t position;
	int32_t position_Oold;
	float velocity;
	float velocity_Oold;
	float accel;
	float accel_Oold;
	float acc;
	float S;
	float V;
	float A;
	
	int32_t set_p;
	int32_t set_v;
	float pwm;
	
}_hall_t;

typedef struct __gConfig
{ 
	uint8_t version;
	uint16_t serialNumber;
	
  float pitchKp;
  float pitchKi;
  float pitchKd;
	
  float rollKp;
  float rollKi;
  float rollKd;
	
  float yawKp;
  float yawKi;
  float yawKd;
	
  float pitchPower;
  float rollPower;
  float yawPower;
	
  float gyroTrust;
	
  float pitchFollow;
  float rollFollow;
  float yawFollow;
	
  float tiltFilter;
  float rollFilter;
  float panFilter;
	
  uint8_t motorFreq;
	
  uint8_t radioType;
	
  int8_t  dirMotorPitch;
  int8_t  dirMotorRoll;
  int8_t  dirMotorYaw;
	
  uint8_t nPolesPitch;
  uint8_t nPolesRoll;
  uint8_t nPolesYaw;
 
  uint8_t 	gyroLPF;
  
  
  int16_t travelMinRoll;
  int16_t travelMaxRoll;
	
  int16_t travelMinPitch;
  int16_t travelMaxPitch;
	
  int16_t travelMinYaw;
  int16_t travelMaxYaw;
	
  uint8_t rcPitchLPF;
  uint8_t rcRollLPF;
  uint8_t rcYawLPF;
	
  uint8_t sbusPitchChan;
  uint8_t sbusRollChan;
  uint8_t sbusYawChan;
  uint8_t sbusModeChan;
	uint8_t sbusYawSpeedChan;
	uint8_t sbusPitchSpeedChan;
	
	uint8_t ppmPitchChan;
  uint8_t ppmRollChan;
  uint8_t ppmYawChan;
  uint8_t ppmModeChan;
	
  int16_t accXOffset;
  int16_t accYOffset;
  int16_t accZOffset;
	
  int16_t gyroXOffset;
  int16_t gyroYOffset;
  int16_t gyroZOffset;
	
  uint8_t useGPS;
  uint8_t skipGyroCalib;   //calibration gyro at startup or not
	
  int16_t rcPitchTrim;
  int16_t rcRollTrim;
  int16_t rcYawTrim;
	
  uint8_t rcPitchMode;
  uint8_t rcRollMode;
  uint8_t rcYawMode;
	
	uint8_t tiltWindow;
	uint8_t panWindow;
	
	uint8_t sbusFuncChan;
	
	float rcPitchSpeed;
	float rcRollSpeed;
	float rcYawSpeed;
	
	uint8_t joyAxis;
	float rcTrimHoz;
	float rcTrimTilt;
	float rcTrimPan;
	
}	gConfig_t;

typedef struct __gSensor
{ 
  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;
  int16_t accX;
  int16_t accY;
  int16_t accZ;
  int16_t accXOffset;
  int16_t accYOffset;
  int16_t accZOffset;
  int16_t gyroXOffset;
  int16_t gyroYOffset;
  int16_t gyroZOffset;
  uint8_t newGyroData;
  uint8_t newAccelData;
  uint8_t gyroCalibrated;	
}  gSensor_t;
 
 typedef struct __gAtti
{ 
	float	theta;
  float phi;
  float psi;
  float thetaDot;
  float phiDot;
  float psiDot;
  float time;
	
	u8 accCorrection;
}	gAtti_t;
 

typedef struct __gMotor
{ 

  float yawPos;
  int32_t yawDamp;
  int32_t yawDrive;
  float yawPosLock;
	float panLock;	
  uint8_t enable;
  uint8_t update;	
	uint8_t updateTheta;
	uint16_t sbusUpdateTheta;
	

	uint8_t power;
	
//	float dSetTilt, dSetPan, dSetRoll;
}	 gMotor_t;
typedef struct __gRadio
{ 
	float level;
  float drive;
  float mode;

  uint8_t noSbusCount;
  int16_t sbusChan[18];
//	

//	
	uint8_t	noPcCount;
	int16_t pcChan[4];
//	
  uint8_t update;
	uint8_t isActive;
	
}	gRadio_t;

typedef struct __gState
{ uint8_t accCalibrated;
  uint8_t gyroCalibrated;
	uint8_t gyroCalibratedF;
  uint8_t sensorError;
  uint8_t normal;
  uint8_t tick;					// tick =1 every 1ms
	uint8_t numFaceAcc;
	uint8_t numFaceAccCalib;
	uint8_t numFace;
	uint8_t status;
	uint8_t statusOld;
	uint8_t active;
	
//gss
	uint8_t button_push;	
/////	
}	gState_t;


typedef struct __gMav
{ uint8_t sendType;
  uint8_t sendFinish;
  uint8_t msgIndex;
  uint8_t update;
	uint8_t updateHigh;
	uint8_t updateMedium;
	uint16_t updateLow;
	uint16_t valueChange;
} gMav_t ;

typedef struct __gBattery
{
	float cell;
	uint8_t status;
	uint8_t update,updates;
	uint8_t lowLevel;
	float volCurrent;
} gBattery_t;

typedef struct __gUblox
{
	unsigned char Class;
	unsigned char id;
	unsigned char length_hi;
	unsigned char length_lo;
	unsigned char payload_counter;
	unsigned char ck_a;
	unsigned char ck_b;

  long Longitude;
	long Lattitude;
	long Altitude;
	long Time;
	unsigned char NewData;
	unsigned char Fix;
	unsigned char NumSats;
	long velN,velNold;
	long velE,velEold;
	long velD,velDold;
	
	long Speed_3d;
	long Ground_Speed;
	long Ground_Course;
}gUblox_t;
#endif	  
