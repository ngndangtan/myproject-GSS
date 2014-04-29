#include "mpu6050.h"
#include "stm32f10x.h"
extern u8 i2c_rx_buffer[30];
ErrorStatus I2CStatus(u8 DeviceID)
{
	u32 I2C_TimeOut = 0x3FFFF;

	/* Clear the I2C1 AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);

	/* Enable I2C1 acknowledgement if it is already disabled by other function */
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	/*----- Transmission Phase -----*/

	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on I2C1 EV5 and clear it */
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
{			 
}

	/* Send slave address for write */
	I2C_Send7bitAddress(I2C1, DeviceID, I2C_Direction_Transmitter);

	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut)/* EV6 */
	{
		I2C_TimeOut--;
	}

	if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF) != 0x0)
	{
		return ERROR;
	}
	else
	{
		return SUCCESS;
	}
}


int I2CWriteByte(u8 deviceID,u8 RegName,u8 RegValue)
{
	/*----- Transmission Phase -----*/
	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on I2C1 EV5 and clear it */

   if (!I2CWaitForEvent(I2C_EVENT_MASTER_MODE_SELECT))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}
	/* Send STIMU_3000 slave address for write */
	I2C_Send7bitAddress(I2C1, deviceID, I2C_Direction_Transmitter);

	/* Test on I2C1 EV6 and clear it */

	 if (!I2CWaitForEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}
	/* Send the configuration register data pointer */
	I2C_SendData(I2C1, RegName);

	/* Test on I2C1 EV8 and clear it */

	    if (!I2CWaitForEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}
	/* Send I2C1 data */
	I2C_SendData(I2C1, RegValue);

	/* Test on I2C1 EV8 and clear it */

	 if (!I2CWaitForEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(I2C1, ENABLE);   
	return 1;  
}

 
int I2CReadFromReg(u8 DeviceID, u8 RegName)
 	{	
 	/* Enable I2C1 acknowledgement if it is already disabled by other function */
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	/*----- Transmission Phase -----*/
	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on I2C1 EV5 and clear it */
	
	if (!I2CWaitForEvent(I2C_EVENT_MASTER_MODE_SELECT))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}	
	/* Send  slave address for write */
	I2C_Send7bitAddress(I2C1, DeviceID, I2C_Direction_Transmitter);

	/* Test on I2C1 EV6 and clear it */

	if (!I2CWaitForEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}	
			 
	/* Send the specified register data pointer */
	I2C_SendData(I2C1, RegName);

	/* Test on I2C1 EV8 and clear it */
	   
	 if (!I2CWaitForEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}	

	/*----- Reception Phase -----*/
	/* Send Re-STRAT condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Test on EV5 and clear it */

if (!I2CWaitForEvent( I2C_EVENT_MASTER_MODE_SELECT))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}		  
	   

	/* Send  slave address for read */
	I2C_Send7bitAddress(I2C1, DeviceID, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */

	if (!I2CWaitForEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    { I2C_GenerateSTOP(I2C1, ENABLE);
	  return 0;
	}	
	return 1;
 	}

  int I2CWaitForEvent( uint32_t event )
{
	 int32_t attempts;
	 
	 attempts = 0;
	 while(!I2C_CheckEvent(I2C1, event))
	 {
		  attempts++;
		  if( attempts > I2C_ACK_ATTEMPTS )
		  {
				return 0;
		  }		  
	 }
	 
	 return 1;
}

void I2CSettingMPU6050(void)
{
	 if(I2CStatus(MPU6050)== SUCCESS)
	{	
	       
		   
		  I2CWriteByte(MPU6050,0x6A,0x00);  
			I2CWriteByte(MPU6050,0x6B,0x03);  //  PLL with Z axis gyro ref
			
			I2CWriteByte(MPU6050,0x37,0x10);  // config interrupt pin INT
			I2CWriteByte(MPU6050,0x38,0x01); //enable data ready interrupt
			I2CWriteByte(MPU6050,0x1A,0x00);	  // filter 	 DLPF disable 0
			I2CWriteByte(MPU6050,0x19,0x07); // set sampling rate to 1khz	 7
			I2CWriteByte(MPU6050,0x1B,0x18); //	 gyro range 2000d/s	0x18, 1000d/s	0x10
			I2CWriteByte(MPU6050,0x1C,0x10); //	 acc range 8g
		  	 
	} 
}
void i2cInit (void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef   I2C_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	   	/* Description:
		  PB5 --> INT1  
		  PB8 --> SDA1 
		  PB9 --> SCL1 

	    */
	
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;					  //0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   
    NVIC_Init(&NVIC_InitStructure);
		
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;					   //1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* confiugre PB5 as INT1 for MPU6050 */
		GPIO_EXTILineConfig( GPIO_PortSourceGPIOA, GPIO_PinSource13);
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
												
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource13);

	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	/* Configure I2C1 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	
	I2C_DeInit(I2C1);

	/* I2C1 Init */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2 ;
	I2C_InitStructure.I2C_OwnAddress1 = 0xD0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;	//348000
	I2C_Init(I2C1, &I2C_InitStructure);

	/* I2C1 Init */
	I2C_Cmd(I2C1, ENABLE);
   	I2C_DMACmd(I2C1, ENABLE);
	I2C_DMALastTransferCmd(I2C1, ENABLE);

	 	/* Set address for DMA I2C RX*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)i2c_rx_buffer;
		
	/* Set address for DMA memory */
	DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;		//I2C_DR
		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	   // read from peripheral
	DMA_InitStructure.DMA_BufferSize = 14; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
				
	/* Clear the TC bit in the SR */
			
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
		
	/* Enable DMA interupt */
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
		
	/* Enable DMA */
	DMA_Cmd(DMA1_Channel7, ENABLE);



   EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
   EXTI_InitStructure.EXTI_Line=EXTI_Line13;  
   EXTI_InitStructure.EXTI_LineCmd=ENABLE;
   EXTI_Init(&EXTI_InitStructure);

}	

