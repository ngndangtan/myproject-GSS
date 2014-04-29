//include 
#include "ppm.h"
/** Description:
			PAN			A4
			TILT		A5
			ROLL		A11
			MODE		A12
*/
//define
//typedef
//variable
u8 led_cnt1,led_cnt2;
ppmValue Pan,Tilt,Roll,Mode;
//fuction
void ledConfiguration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = LED_STATUS|PIN_PAN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(PORT_LED,&GPIO_InitStructure);
}

void ppmConfiguration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = PIN_ROLL|PIN_PAN|PIN_TILT|PIN_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PORT_INPUT,&GPIO_InitStructure);
	
// PAN ---------------------------------------------------------------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;					  //0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	EXTI_Line4;  
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
// TILT ---------------------------------------------------------------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;					  //0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	EXTI_Line5;  
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
// ROLL MODE ----------------------------------------------------------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;					  //0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	EXTI_Line11;  
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_Line		=	EXTI_Line12;  
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void ledStatus(u8 n,u16 speed){
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
				GPIO_WriteBit(PORT_LED,LED_STATUS,Bit_RESET);
			}
			else
			{
				GPIO_WriteBit(PORT_LED,LED_STATUS,(BitAction)(1 - GPIO_ReadOutputDataBit(PORT_LED, LED_STATUS)));
			}
		}
	}
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
