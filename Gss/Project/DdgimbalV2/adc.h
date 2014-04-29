/**
  ******************************************************************************
  * @file: 		adc.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho adc.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

//include ------------------------------------------------------------------------------------------------------------------------
#include "stm32f10x.h"

//defien -------------------------------------------------------------------------------------------------------------------------
#define BATT_CELL_MIN		3.5
#define BATT_CELL_MAX 	4.2
#define BATT_CELL_ALARM	3.6

#define BATT_VOL_SCALE	56.76/9011.2

#define BATT_ALARM_OK		0
#define BATT_ALARM_LOW	1
#define BATT_ALARM_PC		2

#define BATT_NO_CELL		2
#define BATT_3_CELL			3
#define BATT_4_CELL			4
#define BATT_5_CELL			5
#define BATT_6_CELL			6

#define BATTERY_CHANNEL	0
#define PAN_CHANNEL			1
#define TILT_CHANNEl		2

#define JOYSTICK_PITCH_SET_SPEED_SCALE 15;
#define JOYSTICK_PAN_SET_SPEED_SCALE 10;

#define PIN_BATT 			GPIO_Pin_4
#define PORT_BATT			GPIOA
#define CHAN_ADC_BATT	ADC_Channel_4

#define PIN_ADC_PAN		GPIO_Pin_1
#define PORT_ADC_PAN	GPIOA
#define CHAN_ADC_PAN	ADC_Channel_1

#define PIN_ADC_TILT	GPIO_Pin_0
#define PORT_ADC_TILT	GPIOA
#define CHAN_ADC_TILT	ADC_Channel_0

//typedef ------------------------------------------------------------------------------------------------------------------------
typedef struct{
	u32 sample;
	u16 value,valueOld;
	u8 	cntSample;
}adcStruct;
//variable -----------------------------------------------------------------------------------------------------------------------

//Fuction ------------------------------------------------------------------------------------------------------------------------
void adcConfiguration(void);
u16 readAdc(void);
uint8_t checkedBattery(void);
u16 readAdcMulti(u8 channel);
void getAllAdc(void);
void getAdcBattery(void);

