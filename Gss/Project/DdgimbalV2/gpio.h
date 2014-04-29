/**
  ******************************************************************************
  * @file: 		gpio.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho gpio.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
//include ------------------------------------------------------------------------------------------------------------------------
#include "stm32f10x.h"
#include "globalData.h"

//define -------------------------------------------------------------------------------------------------------------------------
//define ppm
#define PPM_NO_SIGNAL_COUNT 			100
#define PPM_MODE_VALUE_HIGH 			100
#define PPM_MODE_VALUE_LOW 				-100
#define PPM_PITCH_SET_ANGLE_SCALE 5.0
#define PPM_ROLL_SET_ANGLE_SCALE 	5.0
#define PPM_YAW_SET_ANGLE_SCALE 	5.0
#define PPM_PITCH_SET_SPEED_SCALE 5.0
#define PPM_ROLL_SET_SPEED_SCALE 	5.0
#define PPM_YAW_SET_SPEED_SCALE 	5.0

#define PIN_PAN					GPIO_Pin_12
#define PORT_PAN				GPIOA
#define PAN_IRQn				EXTI15_10_IRQn
#define PAN_LINE				EXTI_Line4
#define PAN_IRQHandler	EXTI15_10_IRQHandler

#define PIN_TILT				GPIO_Pin_11
#define PORT_TILT				GPIOA
#define TILT_IRQn				EXTI15_10_IRQn
#define TILT_LINE				EXTI_Line5
#define TILT_IRQHandler	EXTI15_10_IRQHandler

#define PIN_ROLL				GPIO_Pin_4
#define PORT_ROLL 			GPIOB
#define ROLL_IRQn				EXTI4_IRQn
#define ROLL_LINE				EXTI_Line4
#define ROLL_IRQHandler	EXTI4_IRQHandler

#define PIN_MODE				GPIO_Pin_5
#define PORT_MODE				GPIOB
#define MODE_IRQn				EXTI5_IRQn
#define MODE_LINE				EXTI_Line5
#define MODE_IRQHandler	EXTI5_IRQHandler

//define button 
#define PIN_BUTTON				GPIO_Pin_13
#define PORT_BUTTON				GPIOA
#define BUTTON_IRQn				EXTI15_10_IRQn
#define BUTTON_LINE				EXTI_Line13
#define BUTTON_IRQHandler	EXTI15_10_IRQHandler

//define led
#define PORT_LED_STATUS	GPIOA
#define LED_STATUS			GPIO_Pin_14

#define PORT_LED_ALARM	GPIOA
#define LED_ALARM				GPIO_Pin_15

#define LED_ERROR_SPEED 	100
#define LED_STATUS_SPEED 	300
#define LED_ALARM_SPEED 	400

#define BUTTON_GAP		1000
#define BUTTON_SANPLE	50

//led
// enum{
// 	STATUS_NORMAL = 1,
// 	STATUS_GYRO_CALIB = 2,
// 	STATUS_ACC_CALIB = 3,
// 	STATUS_UPDATE_THETA = 4,
// 	STATUS_ERROR = 5,
// 	STATUS_MISS_STEP = 6,
// 	STATUS_BATT_LOW = 10,
// };

enum{
	STATUS_MODE_FOLLOW_LOCK = 0x01,	/** folow = 1, lock = 0 */
	STATUS_MISS_STEP = 0x02,				/** miss step = 1, no miss step = 0 */
	STATUS_ERROR_SENSOR = 0x04,			/** error sensor = 1, no error sensor = 0 */
	STATUS_BATT_LOW = 0x08,					/** batterry low = 1, no battery low = 0 */
	STATUS_MOTORS = 0x10,						/** motors on = 1, motor off = 0 (fimware 1.3.4)*/
};

//ppm
enum{
	PPM_PAN = 	0,
	PPM_TILT = 	1,
	PPM_ROLL = 	2,
	PPM_MODE = 	3
};

//button
enum{
	BUTTON_UPDATE_THETA = 1,
	BUTTON_LOCK_MODE	= 2,
	BUTTON_FOLLOW_MODE = 3,
};

//typedef ------------------------------------------------------------------------------------------------------------------------
typedef struct{
	u32 low;
	u32 high;
	u32 value;
	BitAction bit;
} ppmValue;

typedef struct{
	u8 cntButton;
	u8 cntValue;
	u16 cntGap;
	u8 flag1;
	u8 flag2;
	
	u8 val;
}buttonValue;

//variable -----------------------------------------------------------------------------------------------------------------------

//fuction ------------------------------------------------------------------------------------------------------------------------
void ledConfiguration(void);
void ppmConfiguration(void);
void buttonConfiguration(void);
void encorderconfiguration(void);

void ledStatus(u8 n);
void ledGps(BitAction n);

u16 getButtonPess(void);
u32 readROLL(void);
u32 readPAN(void);
u32 readTILT(void);
u32 readMODE(void);
