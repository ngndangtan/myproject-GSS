/**
  ******************************************************************************
  * @file: 	  mpu6000.c
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "mpu6000.h"
#include "gpio.h"

/* PRAVITE DEFINE ----------------------------------------------------------------------------------------------------------------*/


/* PRAVITE VARIABLE --------------------------------------------------------------------------------------------------------------*/
gsensor_spi mpu6000Val;
u16 spi_rx_buffer[7];

extern int16_t gyrox;	
extern int16_t gyroy;	  
extern int16_t gyroz;
extern int16_t accx;	
extern int16_t accy;
extern int16_t accz;
extern gSensor_t gSensor;

// int32_t timeProcessSpi;
extern void delayms(__IO uint32_t num);
u16 mpu6000CntCheckedInt,mpu6000FCntCheckedInt;
/* PRAVITE FUCTION ---------------------------------------------------------------------------------------------------------------*/
/**SPI2 =========================================================================================================================*/
void mpu6000_begin(){
	MPU6000_CS_LOW_LEVEL;
//	delayms(1);
}

void mpu6000_end(){
	MPU6000_CS_HIGH_LEVEL;
//	delayms(1);
}

void MPU6000LowInit(void){
	/* VARIABLE */
	GPIO_InitTypeDef 	GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*RCC Configuration */
	RCC_APB1PeriphClockCmd(MPU6000_RCC_SPI, ENABLE);
	
	/* GPIO Configuration */
	/**
			PB13------SCK
			PB14------MISO
			PB15------MOSI
			
			PB12------NSS
			PC13-------INT
	*/
	GPIO_InitStructure.GPIO_Pin = MPU6000_PIN_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MPU6000_PORT_SCK, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = MPU6000_PIN_MOSI;
	GPIO_Init(MPU6000_PORT_MOSI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MPU6000_PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(MPU6000_PORT_MISO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MPU6000_PIN_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MPU6000_PORT_CS, &GPIO_InitStructure);
	
	/* SPI2 Configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = MPU6000_baud_rate;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(MPU6000_SPI, &SPI_InitStructure);
	
	/* Enable SPI2 */
	SPI_Cmd(MPU6000_SPI,ENABLE);
	
	//ngat nhan
	GPIO_InitStructure.GPIO_Pin = MPU6000_PIN_INT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MPU6000_PORT_INT, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(MPU6000_PortSource_INT, MPU6000_PinSource_INT);
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger	=	EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Line		=	MPU6000_LINE_INT;
  EXTI_InitStructure.EXTI_LineCmd	=	ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = MPU6000_IRQn_INT;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;					  //0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   
	NVIC_Init(&NVIC_InitStructure);
}

void MPU6000Init(void){
		//MPU6000 chip select setup
		MPU6000LowInit();
		delayms(10);
    MPU6000_CS_HIGH_LEVEL;
    delayms(10);
    // Chip reset
    MPU6000RegisterWrite(MPUREG_PWR_MGMT_1, BIT_H_RESET);
    delayms(10);
    // Wake up device and select GyroZ clock (better performance)
    MPU6000RegisterWrite(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);
    delayms(10);
    // Disable I2C bus (recommended on datasheet)
    MPU6000RegisterWrite(MPUREG_USER_CTRL, BIT_I2C_IF_DIS);
    delayms(10);
    // SAMPLE RATE
    MPU6000RegisterWrite(MPUREG_SMPLRT_DIV,0x07);
    delayms(10);
		/* Oscillator set*/
		MPU6000RegisterWrite(MPUREG_PWR_MGMT_1, MPU_CLK_SEL_PLLGYROZ);//  PLL with Z axis gyro ref
		delayms(10);
    // FS & NODLPF   FS=500º/s, DLPF = 256HZ (low pass filter)
    MPU6000RegisterWrite(MPUREG_CONFIG, BITS_DLPF_CFG_256HZ_NOLPF2);
    delayms(10);
    MPU6000RegisterWrite(MPUREG_GYRO_CONFIG,BITS_FS_1000DPS);  // Gyro scale 1000º/s
    delayms(10);
    MPU6000RegisterWrite(MPUREG_ACCEL_CONFIG,0x10);           // Accel scele 4g (4096LSB/g)
    delayms(10);
    // INT CFG => Interrupt on Data Ready
    MPU6000RegisterWrite(MPUREG_INT_ENABLE,BIT_RAW_RDY_EN);         // INT: Raw data ready
    delayms(10);
    MPU6000RegisterWrite(MPUREG_INT_PIN_CFG,BIT_INT_ANYRD_2CLEAR);  // INT: Clear on any read
    delayms(10);
}

u8 spi_mpu6000_rw(u8 out){
	SPI_I2S_SendData( MPU6000_SPI, out);
	while (SPI_I2S_GetFlagStatus(MPU6000_SPI, SPI_I2S_FLAG_RXNE) == RESET) {};
	return  SPI_I2S_ReceiveData(MPU6000_SPI);
}

uint8_t MPU6000RegisterRead(uint8_t reg){
  uint8_t return_value;
	mpu6000_begin();
	spi_mpu6000_rw(reg|0x80);
	return_value = spi_mpu6000_rw(0);
	mpu6000_end();	
  return return_value;
}

void MPU6000RegisterWrite(uint8_t reg, uint8_t val){
	mpu6000_begin();
	spi_mpu6000_rw(reg);
	spi_mpu6000_rw(val);
	mpu6000_end();
}

void MPU6000DataInterrupt(void){
	// tell the timer routine that there is data to be read
	mpu6000Val.INT = 1;
}

static int16_t spi_mpu6000_transfer16(void)
{
        uint8_t byte_H, byte_L;
        byte_H = spi_mpu6000_rw(0);
        byte_L = spi_mpu6000_rw(0);
        return (((int16_t)byte_H)<<8) | byte_L;
}

u8 MPU6000ReadData(void){
	if(mpu6000Val.INT == 1){
		u8 addr = 0;
		
		mpu6000Val.INT = 0;
		gSensor.newGyroData = 1;
		
		mpu6000_begin();
		addr = MPUREG_ACCEL_XOUT_H | 0x80;
		
		spi_mpu6000_rw(addr);
		accy = -spi_mpu6000_transfer16(); //-
		accx = -spi_mpu6000_transfer16(); //-
		accz = -spi_mpu6000_transfer16();
// 		accx = -spi_mpu6000_transfer16(); //-
// 		accy = spi_mpu6000_transfer16(); //-
// 		accz = -spi_mpu6000_transfer16();
		spi_mpu6000_transfer16();
		gyroy = -spi_mpu6000_transfer16(); //-
		gyrox = -spi_mpu6000_transfer16(); //-
		gyroz = -spi_mpu6000_transfer16();
// 		gyrox = -spi_mpu6000_transfer16(); //-
// 		gyroy = spi_mpu6000_transfer16(); //-
// 		gyroz = -spi_mpu6000_transfer16();
		mpu6000_end();
	}
	return 0;
}

u8 MPU6000GetError(void){
	if(mpu6000CntCheckedInt++ > 100){
		return MPU6000_SIGNAL_LOST;
	}
	
	return MPU6000_SIGNAL_OK;
}

