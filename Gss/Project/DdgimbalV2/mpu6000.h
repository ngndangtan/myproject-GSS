/**
  ******************************************************************************
  * @file: 		mpu6000.h	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao va define cho mpu6000.h
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 

#include "stdint.h"
#include "stm32f10x.h"
#include "globalData.h"

/* DEFINE ----------------------------------------------------------------------------------------------------------------------*/
// MPU 6000 registers
#define MPUREG_WHOAMI 				0x75 //
#define MPUREG_SMPLRT_DIV 		0x19 //
#define MPUREG_CONFIG 				0x1A //
#define MPUREG_GYRO_CONFIG 		0x1B
#define MPUREG_ACCEL_CONFIG 	0x1C
#define MPUREG_FIFO_EN 				0x23
#define MPUREG_INT_PIN_CFG 		0x37
#define MPUREG_INT_ENABLE 		0x38
#define MPUREG_INT_STATUS 		0x3A
#define MPUREG_ACCEL_XOUT_H 	0x3B //
#define MPUREG_ACCEL_XOUT_L 	0x3C //
#define MPUREG_ACCEL_YOUT_H 	0x3D //
#define MPUREG_ACCEL_YOUT_L 	0x3E //
#define MPUREG_ACCEL_ZOUT_H 	0x3F //
#define MPUREG_ACCEL_ZOUT_L 	0x40 //
#define MPUREG_TEMP_OUT_H 		0x41//
#define MPUREG_TEMP_OUT_L 		0x42//
#define MPUREG_GYRO_XOUT_H 		0x43 //
#define MPUREG_GYRO_XOUT_L 		0x44 //
#define MPUREG_GYRO_YOUT_H 		0x45 //
#define MPUREG_GYRO_YOUT_L 		0x46 //
#define MPUREG_GYRO_ZOUT_H 		0x47 //
#define MPUREG_GYRO_ZOUT_L 		0x48 //
#define MPUREG_USER_CTRL 			0x6A //
#define MPUREG_PWR_MGMT_1 		0x6B //
#define MPUREG_PWR_MGMT_2 		0x6C //
#define MPUREG_FIFO_COUNTH 		0x72
#define MPUREG_FIFO_COUNTL 		0x73
#define MPUREG_FIFO_R_W 			0x74


// Configuration bits MPU 3000 and MPU 6000 (not revised)?
#define BIT_SLEEP 									0x40
#define BIT_H_RESET 								0x80
#define BITS_CLKSEL 								0x07
#define MPU_CLK_SEL_PLLGYROX 				0x01
#define MPU_CLK_SEL_PLLGYROZ 				0x03
#define MPU_CLK_SEL_PLLGYROY				0x02
#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18
#define BITS_FS_MASK                0x18
#define BITS_DLPF_CFG_256HZ_NOLPF2  0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07
#define BIT_INT_ANYRD_2CLEAR      	0x10
#define BIT_RAW_RDY_EN        			0x01
#define BIT_I2C_IF_DIS              0x10
#define BIT_INT_STATUS_DATA   			0x01


#define MPU6000_CS_HIGH_LEVEL				GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define MPU6000_CS_LOW_LEVEL				GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define MPU6000F_CS_HIGH_LEVEL			GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define MPU6000F_CS_LOW_LEVEL				GPIO_ResetBits(GPIOA,GPIO_Pin_15)

//checked error
#define MPU6000_SIGNAL_OK		1
#define MPU6000_SIGNAL_LOST 2

//port & pin spi of mpu6000
#define MPU6000_PIN_SCK		GPIO_Pin_13
#define MPU6000_PORT_SCK	GPIOB

#define MPU6000_PIN_MISO	GPIO_Pin_14
#define MPU6000_PORT_MISO	GPIOB

#define MPU6000_PIN_MOSI	GPIO_Pin_15
#define MPU6000_PORT_MOSI	GPIOB

#define MPU6000_PIN_CS		GPIO_Pin_12
#define MPU6000_PORT_CS		GPIOB

#define MPU6000_PIN_INT					GPIO_Pin_10
#define MPU6000_PORT_INT				GPIOB
#define MPU6000_PortSource_INT	GPIO_PortSourceGPIOB
#define MPU6000_PinSource_INT		GPIO_PinSource10
#define MPU6000_IRQn_INT				EXTI15_10_IRQn
#define MPU6000_LINE_INT				EXTI_Line10
#define MPU6000_IRQHandler			EXTI15_10_IRQHandler
#define MPU6000_SPI				SPI2
#define MPU6000_baud_rate	SPI_BaudRatePrescaler_16
#define MPU6000_RCC_SPI		RCC_APB1Periph_SPI2

/* VARIABLE --------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	u8 INT;
	u8 spiError;
	u8 spiBus;
	int16_t accx;
	int16_t accy;
	int16_t accz;
	int16_t gyrox;
	int16_t gyroy;
	int16_t gyroz;
}gsensor_spi;

/* FUCTION ---------------------------------------------------------------------------------------------------------------------*/
void MPU6000LowInit(void);
void MPU6000Init(void);
uint8_t MPU6000RegisterRead(uint8_t reg);
void MPU6000RegisterWrite(uint8_t reg, uint8_t val);
void MPU6000DataInterrupt(void);
u8 MPU6000ReadData(void);
u8 MPU6000GetError(void);


