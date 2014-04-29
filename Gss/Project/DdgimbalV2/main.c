/**
  ******************************************************************************
  * @file: 	 main.c 
  * @author: GREMSY Team
  * @version: 1.2.0
  * @date:    30/9/2013
  * @brief:   
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/  

#include "main.h"
#include "interrupt.h"

/** DEFINE ----------------------------------------------------------------------------------------------------------------------------------------------*/
enum MOTOR_CONTROL
{
	MOTOR_OFF = 0, /* disable motor drivers | */
	MOTOR_ON = 1, /* enable motor drivers | */
	MOTOR_CONTROL_ENUM_END=2, /*  | */
};

/** FUCNTIONS -------------------------------------------------------------------------------------------------------------------------------------------*/
void delayms(__IO uint32_t num);
void statusSystem(void);
void getValuesSystem(void);
void updatesSystem(void);


/** VARIABLES -------------------------------------------------------------------------------------------------------------------------------------------*/
extern gUblox_t gUblox;
int timeProcessMain = 0;

/**
  * @brief: chuong trinh chinh de khai bao va chay cac thuat toan
  * @param: none
  * @retval: none
  */
int main(void){
	/** khai bao bien su dung trong main */
	{
	
	RCCInit(); 		/** khai bao rcc*/
	delayms(1);
 	usart1Init();	/** khai bao uart mavlink */
 	delayms(10);
 	ubloxInit();
 	delayms(10);
 	MPU6000Init();/** khai bao spi va cau hinh cho cam bien mpu6000 */
  delayms(1);	
	timerInit();	/** khai bao time & cac pwm de dieu khien dong co */
	delayms(1);
 	getParamsFromFlashAfterPowerOn();	/** khai bao va doc flash */
	delayms(1);
		buttonConfiguration();	/** cau hinh nut nhan menu */
 	delayms(1);
	adcConfiguration();			/** cau hinh adc doc pin */
	delayms(1);
	ledConfiguration();			/** cau hinh cho led */
	delayms(1);
	encorderconfiguration();
	delayms(1);
 	futabaInit();						/** khai bao uart su dung tinh nang sbus cua futaba */
	delayms(1);
	
	/** cau hinh bo system tich */
	if (SysTick_Config((uint32_t)(0xFFFFFF)) )
	{
		/* Capture error */ 
		while (1);
	}
	delayms(10);
	DMA_Cmd(DMA1_Channel5, ENABLE); /** enable dma of uart1(use mavlink)*/
	USART_Cmd(USART1, ENABLE);			/** enable uart1 */
	delayms(10);
	
	DMA_Cmd(DMA1_Channel6, ENABLE); /** enable dma of uart1(use mavlink)*/
	USART_Cmd(USART2, ENABLE);			/** enable uart1 */
	delayms(10);
	
/** khoi tao thong so ban dau */
	/** mavlink */
	gMav.sendType=gMAV_SEND_NONE;
	gMav.sendFinish=1;
	gMav.msgIndex=0;
	
 	gState.tick=0;
	/** anggle and speed angle*/
	gAtti.phi=0;
	gAtti.theta=0;
	gAtti.psi=0;
	gAtti.phiDot = 0;
	gAtti.thetaDot = 0;
	gAtti.psiDot = 0;
	
	/** sensor mpu6000 */
	mpu6000Val.INT=0;

	/** motor */
	gMotor.updateTheta =2;
	gMotor.enable=0;
	
	/** radio */
	gRadio.noSbusCount=		SBUS_NO_SIGNAL_COUNT ;
//	gRadio.noPpmCount = 	PPM_NO_SIGNAL_COUNT;
//	gRadio.noPcCount = 		PC_NO_SIGNAL_COUNT;

	/** battery */
	gBattery.lowLevel = 0;
	gBattery.status = BATT_ALARM_LOW;
	
	/** cac gia tri update */
	gSensor.newGyroData = 0;
	gRadio.update = 0;
	gMav.update = 	0;
	gMotor.update = 0;				
	gAtti.accCorrection = 1;
}
	while(1)
	{

		/** tinh van toc */
		readVel();
		gss_sensor();
		MPU6000ReadData();
		/** update gia tri sensor khi co gia tri moi */
		if (gSensor.newGyroData==1)
		{ 

			gSensor.newGyroData=0;
			/***/
			gyrox=(gyrox+0*gyrox_1)/(1+0);
			gyroy=(gyroy+0*gyroy_1)/(1+0);
			gyroz=(gyroz+gConfig.gyroLPF*gyroz_1)/(1+ gConfig.gyroLPF);
			gyrox_1=gyrox;
			gyroy_1=gyroy;
			gyroz_1=gyroz;
			
			/***/
			accx=(accx+3*accx_1)/4;
			accy=(accy+3*accy_1)/4;
			accz=(accz+3*accz_1)/4;
			accx_1=accx;
			accy_1=accy;
			accz_1=accz;
			/***/
			gSensor.gyroX=gyrox;
			gSensor.gyroY=gyroy;
			gSensor.gyroZ=gyroz;
			gSensor.accX=accx;
			gSensor.accY=accy;
			gSensor.accZ=accz;
			
			/** calib gyro */
			if(gState.gyroCalibrated == 0)
			{
				gMav.valueChange |= SYS_STATUS_MASK;
				gyroCalibrate();
				timerAtti= SysTick->VAL;
			}
			
			/** calib acc */
			if(gState.numFaceAccCalib != 0){
				gMav.valueChange |= SYS_STATUS_MASK;
				accelCalibrate();
			}
		}
		
		
		/** kich hoat dong co khi da thoa cac dieu kien sau:
				@para: khong co calib gyro
				@para: khong co calib acc
				@para: battery ko het
				@para: cho phep hoat dong dong co
		*/
		if((gState.gyroCalibrated !=0) && (gState.numFaceAccCalib == 0) && (gBattery.status == BATT_ALARM_OK)&&(gMotor.power == MOTOR_ON)){
				gState.status &= ~(STATUS_BATT_LOW);
				gMotor.enable = 1;
		}
		else{
				gMotor.enable = 0;
		}
		if (gState.tick==1)				 // tick will be set every 1ms
		{
			gState.tick=0;
			
			getValuesSystem();
			
			updatesSystem();	
		}
			/** math angle */
		if(gState.gyroCalibrated == 1 && gState.numFaceAccCalib == 0) attitudeEstimate();		
		/** control motor */
		
		motorLeft(leftSet);
		motorRight(rightSet);

		
		/** send mavlink */
		mavlinkSend();
		
		/** reciver mavlink */
		mavlinkReceive();
		
	}
}

/** FUCNTIONS -------------------------------------------------------------------------------------------------------------------------------------------*/

/**
  * @brief: delay a few time
  * @param:
	* @num khoang thoi gian delay
  * @retval: none.
  */
void delayms(__IO uint32_t num){
	__IO uint32_t index = 0;
	/* default system clock is 72MHz */
	for(index = (72000 * num); index != 0; index--);
}

void statusSystem(void)
{
	/** checked status */
	
	/** hien thi trang thai */
	ledStatus(gState.status);
	
}

void getValuesSystem(void)
{
	/** read gia tri adc */
	getAdcBattery();
	
	if(gBattery.updates++==20){
		gBattery.updates = 0;
		/** kiem tra pin */
		gBattery.status = checkedBattery();
		if(gBattery.status == BATT_ALARM_LOW)
		{
			gState.status |= STATUS_BATT_LOW;
		}
	}

	/** checkec error sensor */
	if(MPU6000GetError() == MPU6000_SIGNAL_LOST){
		gMotor.power = MOTOR_OFF;
		gState.status |= STATUS_ERROR_SENSOR;
	}
	
	/** update radio */
	if (gRadio.update++==20)	    //50Hz
	{
			gRadio.update=0;			
			/** update heart bit */
			gMav.valueChange |= HEART_BIT_MASK;
			/** update debug */
			gMav.valueChange |= DEBUG_MASK;
	}
}

void updatesSystem(void)
{
	/** update send msg */
	if (gMav.sendType==gMAV_SEND_NONE)
	{
			if (gMav.update++==20)	//50Hz
			{
				radioProcess();
				gMav.sendType=gMAV_SEND_MSG;
				gMav.update=0;
			}
	}
}
