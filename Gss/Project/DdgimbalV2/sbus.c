/**
  ******************************************************************************
  * @file: 	  sbus.c
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   xu ly va khai bao cac sbus
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "sbus.h"

extern gRadio_t gRadio;
extern gMav_t 		gMav;
futabaStruct_t futabaData __attribute__((section(".ccm")));

int futabaDecode(void)
{
  if (futabaData.u.rawBuf[22]&0x04) {
 		if (!gRadio.noSbusCount)
			gRadio.noSbusCount = 101;
		return -1;
  }
  else {
		gRadio.sbusChan[0] = 1024	-	futabaData.u.channels.channel1 ;
		gRadio.sbusChan[1] = 1024	-	futabaData.u.channels.channel2 ;
		gRadio.sbusChan[2] = 1024 - futabaData.u.channels.channel3;
		gRadio.sbusChan[3] = 1024	- futabaData.u.channels.channel4 ;
		gRadio.sbusChan[4] = 1024 - futabaData.u.channels.channel5;
		gRadio.sbusChan[5] = 1024 - futabaData.u.channels.channel6;
		gRadio.sbusChan[6] = 1024 - futabaData.u.channels.channel7;
		gRadio.sbusChan[7] = 1024 - futabaData.u.channels.channel8;
		gRadio.sbusChan[8] = 1024 - futabaData.u.channels.channel9;
		gRadio.sbusChan[9] = 1024 - futabaData.u.channels.channel10;
		gRadio.sbusChan[10] = 1024 - futabaData.u.channels.channel11;
		gRadio.sbusChan[11] = 1024 - futabaData.u.channels.channel12;
		gRadio.sbusChan[12] = 1024 - futabaData.u.channels.channel13;
		gRadio.sbusChan[13] = 1024 - futabaData.u.channels.channel14;
		gRadio.sbusChan[14] = 1024 - futabaData.u.channels.channel15;
		gRadio.sbusChan[15] = 1024 - futabaData.u.channels.channel16;

		gRadio.sbusChan[16] = futabaData.u.rawBuf[22] & 0x01 ? 800 : -800;
		gRadio.sbusChan[17] = futabaData.u.rawBuf[22] & 0x02 ? 800 : -800;
		gRadio.noSbusCount=0;
		gMav.valueChange |= SBUS_MASK;
		return 1;
  }
}

int futabaCharIn(unsigned char c) {
  switch (futabaData.state) {
    case FUTABA_WAIT_SYNC:
			if (c == FUTABA_START_CHAR) {
					futabaData.state = FUTABA_WAIT_DATA;
					futabaData.dataCount = 0;
				
			}
		break;
    case FUTABA_WAIT_DATA:
			futabaData.u.rawBuf[futabaData.dataCount++] = c;
			if (futabaData.dataCount == 23)
					futabaData.state = FUTABA_WAIT_END;
		break;
    case FUTABA_WAIT_END:
			futabaData.state = FUTABA_WAIT_SYNC;
			if (c == FUTABA_END_CHAR) {
					
					return futabaDecode();
			}
		break;
  }
  return 0;
}

void futabaInit(void) {
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
			
	/* Configure PB11 for USART3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    memset((void *)&futabaData, 0, sizeof(futabaData));

   
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = FUTABA_BAUD;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART3, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;			   //3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				 			 //0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
