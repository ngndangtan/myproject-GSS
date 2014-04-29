
//include ------------------------------------------------------------------------------------------------------------------------
#include "stm32f10x.h"
#include "globalData.h"

//define -------------------------------------------------------------------------------------------------------------------------
#define PORT_LED		GPIOA
#define LED_STATUS	GPIO_Pin_8

#define PORT_INPUT	GPIOA
#define PIN_PAN			GPIO_Pin_4
#define PIN_TILT		GPIO_Pin_5
#define PIN_MODE		GPIO_Pin_12
#define PIN_ROLL		GPIO_Pin_11

#define PPM_NO_SIGNAL_COUNT 100
#define PPM_MODE_VALUE_HIGH 400
#define PPM_MODE_VALUE_LOW -400
#define PPM_PITCH_SET_ANGLE_SCALE 10
#define PPM_ROLL_SET_ANGLE_SCALE 	10
#define PPM_YAW_SET_ANGLE_SCALE 	10
#define PPM_PITCH_SET_SPEED_SCALE 10
#define PPM_ROLL_SET_SPEED_SCALE 	10
#define PPM_YAW_SET_SPEED_SCALE 	10

#define LED_ERROR_SPEED 	100
#define LED_STATUS_SPEED 	200

enum{
	STATUS_NORMAL = 1,
	STATUS_GYRO_CALIB = 2,
	STATUS_ACC_CALIB = 3,
	STATUS_ERRO = 4,
};

//typedef ------------------------------------------------------------------------------------------------------------------------
typedef struct{
	u32 low;
	u32 high;
	u32 value;
	BitAction bit;
} ppmValue;
//variable -----------------------------------------------------------------------------------------------------------------------

//fuction ------------------------------------------------------------------------------------------------------------------------
void ledConfiguration(void);
void ledStatus(u8 n,u16 speed);
void ppmConfiguration(void);
u32 readROLL(void);
u32 readPAN(void);
u32 readTILT(void);
u32 readMODE(void);
