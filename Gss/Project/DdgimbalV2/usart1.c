/**
  ******************************************************************************
  * @file: 		usart1.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   sua dung cho cac uart1
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "usart1.h"
extern u8 mav_rx_buffer[];
extern u8 mav_tx_buffer[];

void usart1Init(void) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;	
	
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
		
		/* Configure PA9 for USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure PA10 for USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* Enable the DMA1_Channel4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;				//2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//0
  NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;  
  NVIC_Init(&NVIC_InitStructure);

 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;				//4
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;							//0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	/* Set address for DMA USART TX*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 0x04;
	
	/* Set address for DMA memory */ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)mav_tx_buffer;
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
			
	/* Clear the TC bit in the SR */
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	/* DMA1_Channel4 configuration */
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	
	/* Enable DMA interupt */
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	/* Enable DMA */
	DMA_Cmd(DMA1_Channel4, DISABLE);

	/* Set address for DMA USART1 RX*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 0x04;
	/* Set address for DMA memory */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) mav_rx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

	DMA_InitStructure.DMA_BufferSize = MAV_RX_BUFF_SIZE; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* DMA1_Channel6 configuration */
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	/* Enable DMA interupt */
	//DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	
	/* Enable DMA */
	DMA_Cmd(DMA1_Channel5, ENABLE);
    
	USART_InitStructure.USART_BaudRate = MAVLINK_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART  DMA */
	USART_DMACmd(USART1, USART_DMAReq_Tx , ENABLE);
	/* Enable USART  DMA */
	USART_DMACmd(USART1, USART_DMAReq_Rx , ENABLE);
	
	/* Enable USART */
	USART_Cmd(USART1, DISABLE);
 
}
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
	//USART_SendData(USART1,(u8)ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}
