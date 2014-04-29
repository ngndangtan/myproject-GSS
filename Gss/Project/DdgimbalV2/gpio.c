/**
  ******************************************************************************
  * @file: 	  gpio.c
  * @author:  GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao cac chan input, output
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

//include 
#include "gpio.h"
/** Description:
			PAN			A4
			TILT		A5
			ROLL		A11
			MODE		A12
*/
//define
//typedef
//variable
ppmValue Pan,Tilt,Roll,Mode;
//fuction
void ledConfiguration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = LED_STATUS;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(PORT_LED_STATUS,&GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = LED_ALARM;
		GPIO_Init(PORT_LED_ALARM,&GPIO_InitStructure);
}

void ppmConfiguration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = PIN_PAN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PORT_PAN,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TILT;
	GPIO_Init(PORT_TILT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ROLL;
	GPIO_Init(PORT_ROLL,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_MODE;
	GPIO_Init(PORT_MODE,&GPIO_InitStructure);
	
// PAN ---------------------------------------------------------------------------------------------------------------------
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	NVIC_InitStructure.NVIC_IRQChannel = PAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;					  //6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;									//0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	PAN_LINE;
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
// TILT ---------------------------------------------------------------------------------------------------------------------
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	NVIC_InitStructure.NVIC_IRQChannel = TILT_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;					  //5
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;									//0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	TILT_LINE;  
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
// ROLL MODE ----------------------------------------------------------------------------------------------------------------
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);
	
	NVIC_InitStructure.NVIC_IRQChannel = ROLL_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;					  //0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;									//0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	ROLL_LINE;
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	MODE_LINE;
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}


void buttonConfiguration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = PIN_BUTTON;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PORT_BUTTON,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);
}
void encorderconfiguration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init( GPIOA, &GPIO_InitStructure);

//------------------------------------------------------------------------------//
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);//define pin EXTI,buoc phai co
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
 	
 	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);
 			
 	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//mac dinh la group uu tien 4 (mics.c) nen danh tron 4bit cho preempti/sub k co bit nao
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;//co the bo?
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
 	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);
 			
 	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//mac dinh la group uu tien 4 (mics.c) nen danh tron 4bit cho preempti/sub k co bit nao
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;//co the bo?
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
 	NVIC_Init(&NVIC_InitStructure);
	}
void ledDisplayNum(u8 n, u16 speed){
	static u16 led_cnt1,led_cnt2;
	u8 a = 2*n;
	led_cnt1++;
	if(led_cnt1 > speed)
	{
		led_cnt1 = 0;
		led_cnt2++;
		if(led_cnt2 > (a+10))
		{
			led_cnt2 = 0;
		}
		else
		{
			if(led_cnt2 > a)
			{
				GPIO_WriteBit(PORT_LED_STATUS,LED_STATUS,Bit_RESET);
			}
			else
			{
				GPIO_WriteBit(PORT_LED_STATUS,LED_STATUS,(BitAction)(1 - GPIO_ReadOutputDataBit(PORT_LED_STATUS, LED_STATUS)));
			}
		}
	}
}

void ledBlink(int speed){
	static u16 led_cnt5;
	
	led_cnt5++;
	if(led_cnt5 > speed){
		led_cnt5 = 0;
		GPIO_WriteBit(PORT_LED_STATUS,LED_STATUS,(BitAction)(1 - GPIO_ReadOutputDataBit(PORT_LED_STATUS, LED_STATUS)));
	}
}

void ledStatus(u8 n){
	if(n & STATUS_BATT_LOW){
		ledBlink(100);
	}
	else if(n & STATUS_ERROR_SENSOR){
		ledBlink(1);
	}
	else if(n & STATUS_MISS_STEP){
		ledDisplayNum(1,LED_STATUS_SPEED);
	}
	else if(n & STATUS_MODE_FOLLOW_LOCK){
		ledDisplayNum(3,LED_STATUS_SPEED);
	}
	else{
		ledDisplayNum(2,LED_STATUS_SPEED);
	}
}

void ledGps(BitAction n)
{
	GPIO_WriteBit(PORT_LED_ALARM,LED_ALARM,n); 
}

buttonValue button;
u16 getButtonPess(void){
	if(button.val != 0) button.val = 0;
	
	if(GPIO_ReadInputDataBit(PORT_BUTTON,PIN_BUTTON) == 0){
		button.cntButton++;
	}
	else{
		button.flag2 = 0;
		button.cntButton = 0;
	}
	
	if(button.cntButton > BUTTON_SANPLE){
		if(button.flag1 == 0){
			button.cntValue = 0;
			button.flag1 = 1;
		}
		
		button.cntGap = 0;
		button.cntButton = BUTTON_SANPLE;
		
		if(button.flag2 == 0){
			button.flag2 = 1;
			button.cntValue++;
		}
	}
	
	if(button.flag1 == 1){
		button.cntGap++;
		if(button.cntGap > BUTTON_GAP){
			button.flag1 = 0;
			button.val = button.cntValue;
			button.cntGap = 0;
		}
	}
	
	return button.val;
}

u32 readROLL(void){
	return Roll.value/9;
}

u32 readPAN(void){
	return Pan.value/9;
}

u32 readTILT(void){
	return Tilt.value/9;
}

u32 readMODE(void){
	return Mode.value/9;
}
