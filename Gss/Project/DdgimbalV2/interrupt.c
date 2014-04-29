/**
  ******************************************************************************
  * @file: 		interrupt.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   chua cac ham ngat
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "interrupt.h"
#include "motor.h"
#include "sbus.h"
#include "globalData.h"
#include "mpu6000.h"
#include "attitude.h"

extern u16 mpu6000CntCheckedInt;
//gss////////
extern _hall_t hall_r,hall_l;
extern gMav_t gMav;
extern gState_t gState;
extern gsensor_spi mpu6000Val;

void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5)==SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}		
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4)==SET)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);
		gMav.sendFinish=1;
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}		
}

void USART3_IRQHandler(void)
{
	u8  c;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
			c =USART_ReceiveData(USART3);
			futabaCharIn(c);
			USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
	}
}

void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		gState.tick=1;
	}
}

void MPU6000_IRQHandler(void)
{
	/*intterupt sensor mpu6000 --------------------------------------------------------------------------------------------------------------*/
	if( EXTI_GetITStatus(MPU6000_LINE_INT)==SET)	
	{
		static u8 tmp=0;
		if (tmp==0) {tmp=1;} 
		else{
			mpu6000Val.INT=1;
			mpu6000CntCheckedInt = 0;
		}
		EXTI_ClearITPendingBit(MPU6000_LINE_INT);
	}
	/*intterupt button*/
	if( EXTI_GetITStatus(EXTI_Line13)==SET)	
	{

		gState.button_push=1;
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

void 	EXTI9_5_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line7)==SET)	
	{
		hall_r.timeVel = getTime(&hall_r.time);
		hall_r.time = SysTick->VAL;
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)
			hall_r.direct=-1;
		else
			hall_r.direct=1;
		
		hall_r.count+= hall_r.direct;
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	if( EXTI_GetITStatus(EXTI_Line6)==SET)
	{
		hall_l.timeVel = getTime(&hall_l.time);
		hall_l.time = SysTick->VAL;
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==1)
			hall_l.direct=1;
		else
			hall_l.direct=-1;
		
		hall_l.count+= hall_l.direct;
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

void readVel(void){
	hall_r.vel = 162000.0/(float)hall_r.timeVel;//1.0/((float)hall_r.timeVel/9000000.0)xung/s -> (xung/s)/880.0(vong/s) 
																							//-> (vong/s)*60(vong/phut) -> (vong/phut)/5(vong banh xe/phut)
																							//-> (vong banh xe/phut)*2*PI*R(cm/phut)-> (cm/phut)/10^2(m/phut);
	hall_l.vel = 162000.0/(float)hall_l.timeVel;
	
	hall_r.distance = ((float)hall_r.count*0.0003); //count -> count/880.0(vong) -> vong/5(vong banh xe)
																									//-> vong banh xe*2*PI*R(cm) -> cm/100(m);
	hall_l.distance = ((float)hall_l.count*0.0003);
}
