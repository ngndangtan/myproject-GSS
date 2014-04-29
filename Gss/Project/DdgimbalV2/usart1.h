/**
  ******************************************************************************
  * @file: 	  usart1.h
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho usart1.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include <stdio.h>
#include "stm32f10x.h"
#include <stdio.h>
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define MAVLINK_BAUD 57600
#define MAV_RX_BUFF_SIZE 100
#define GPS_BAUD 38400
#define GPS_RX_BUFF_SIZE 100



	
void usart1Init(void);
void usart2Init(void);
int gpsReceive(void);
