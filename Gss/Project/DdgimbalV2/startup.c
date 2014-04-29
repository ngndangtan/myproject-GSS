/**
  ******************************************************************************
  * @file: 		startup.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao cac chuc nang ban dau
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "startup.h"

extern uint8_t mav_tx_buffer[];

extern 	u8 i2c_rx_buffer[];
 extern unsigned char mav_rx_buffer[MAV_RX_BUFF_SIZE];

void RCCInit(void)
{	
	ErrorStatus HSEStartUpStatus;
	/* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
	initialize the PLL and update the SystemFrequency variable. */

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
   		/* Enable Prefetch Buffer */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    	/* Flash 2 wait state */
    	FLASH_SetLatency(FLASH_Latency_2);

    	/* HCLK = SYSCLK */
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);

    	/* PCLK2 = HCLK */
    	RCC_PCLK2Config(RCC_HCLK_Div1);

    	/* PCLK1 = HCLK/4 */
    	RCC_PCLK1Config(RCC_HCLK_Div4);

    	/* PLLCLK = 8MHz * 9 = 72 MHz */
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    	/* Enable PLL */
    	RCC_PLLCmd(ENABLE);

    	/* Wait till PLL is ready */
    	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    	{}
	
	    /* Select PLL as system clock source */
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	    /* Wait till PLL is used as system clock source */
	    while (RCC_GetSYSCLKSource() != 0x08)
	    {}
	}
  
	/* Enable GPIOA, GPIOB and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  	
  /* TIM2,TIM3, TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
  	
	/* Enable UART1 clock, if using USART2 or USART3 ... we must use RCC_APB1PeriphClockCmd(RCC_APB1Periph_USARTx, ENABLE) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  

  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/*turn off ocs */
	RCC_LSEConfig(RCC_LSE_OFF);
	
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);
	TIM_DeInit(TIM3);
	TIM_DeInit(TIM4);
}

