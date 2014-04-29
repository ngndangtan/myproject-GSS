/**
  ******************************************************************************
  * @file: 		adc.c	  
  * @author: 	GREMSY Team
  * @version: v1.1.3
  * @date:    2/11/2013
  * @brief:   config va sua dung adc
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/

//include ------------------------------------------------------------------------------------------------------------------------
#include "adc.h"
#include "globalData.h"
//defien -------------------------------------------------------------------------------------------------------------------------
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
//typedef ------------------------------------------------------------------------------------------------------------------------

//variable -----------------------------------------------------------------------------------------------------------------------
__IO uint16_t ADCConvertedValue;
extern gBattery_t gBattery;
extern gMav_t gMav;
adcStruct adcPan,adcTilt,adcBaterry;

//Fuction ------------------------------------------------------------------------------------------------------------------------
void adcConfiguration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* PCLK2 is the APB2 clock */
	/* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	/* Enable ADC1 clock so that we can talk to it */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	/* Put everything back to power-on defaults */ 
	ADC_DeInit(ADC1);

  /* Configure PA.00 (ADC Channel0) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = PIN_BATT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(PORT_BATT, &GPIO_InitStructure);
	
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel1 configuration */
	ADC_RegularChannelConfig(ADC1, CHAN_ADC_BATT, 1, ADC_SampleTime_71Cycles5);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
	/** Start the conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
}

u16 readAdc(void)
{
		/* Get the conversion value */
		return ADC_GetConversionValue(ADC1);
}

u16 readAdcMulti(u8 channel){
	/* Get the conversion value */
	int adcValue = ADC_GetConversionValue(ADC1);
	/* ADC1 regular channel1 configuration */
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_71Cycles5);
	/** Start the conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
	
	return adcValue;
}

u8 AdcIndexChannel;
void getAllAdc(void){
// 		adcBaterry.value = readAdc();

// 		adcBaterry.value = (10*adcBaterry.valueOld + adcBaterry.value)/11;
// 		adcBaterry.valueOld = adcBaterry.value;
	if(AdcIndexChannel == BATTERY_CHANNEL){
		AdcIndexChannel = PAN_CHANNEL;
 		adcBaterry.value = readAdcMulti(CHAN_ADC_PAN);

		adcBaterry.value = (10*adcBaterry.valueOld + adcBaterry.value)/11;
		adcBaterry.valueOld = adcBaterry.value;
	}
	else if(AdcIndexChannel == PAN_CHANNEL){
		AdcIndexChannel = TILT_CHANNEl;
		adcPan.value = readAdcMulti(CHAN_ADC_TILT);
		adcPan.value = (10*adcPan.valueOld + adcPan.value)/11;
		adcPan.valueOld = adcPan.value;
	}
	else if(AdcIndexChannel == TILT_CHANNEl){
		AdcIndexChannel = BATTERY_CHANNEL;

		adcTilt.value = readAdcMulti(CHAN_ADC_BATT);
		adcTilt.value = (10*adcTilt.valueOld + adcTilt.value)/11;
		adcTilt.valueOld = adcTilt.value;
	}
}

void getAdcBattery(void){
		adcBaterry.value = readAdc();

		adcBaterry.value = (10*adcBaterry.valueOld + adcBaterry.value)/11;
		adcBaterry.valueOld = adcBaterry.value;	
}

uint8_t checkedBattery(void){
	float volCurrent = adcBaterry.value*BATT_VOL_SCALE;
	
	if((u16)(volCurrent*100) != (u16)(gBattery.volCurrent*100)){
		gBattery.volCurrent = volCurrent;
		gMav.valueChange |= SYS_STATUS_MASK;
		
		if(gBattery.volCurrent < (BATT_3_CELL*BATT_CELL_MIN - 4)){
			gBattery.cell = BATT_NO_CELL;
		}
		else if((gBattery.volCurrent >= (BATT_3_CELL*BATT_CELL_MIN)) && (gBattery.volCurrent <= (BATT_3_CELL*BATT_CELL_MAX))){
			gBattery.cell = BATT_3_CELL;
		}
		else if((gBattery.volCurrent >= (BATT_4_CELL*BATT_CELL_MIN)) && (gBattery.volCurrent <= (BATT_4_CELL*BATT_CELL_MAX))){
			gBattery.cell = BATT_4_CELL;
		}
		else if((gBattery.volCurrent >= (BATT_5_CELL*BATT_CELL_MIN)) && (gBattery.volCurrent <= (BATT_5_CELL*BATT_CELL_MAX))){
			gBattery.cell = BATT_5_CELL;
		}
		else if((gBattery.volCurrent >= (BATT_6_CELL*BATT_CELL_MIN))){
			gBattery.cell = BATT_6_CELL;
		}

		if(gBattery.cell != 2){
			if(gBattery.volCurrent <= gBattery.cell*BATT_CELL_ALARM){
				gBattery.update++;
				if(gBattery.update > 10){
					gBattery.update = 10;
					gBattery.lowLevel = BATT_ALARM_LOW;
				}
			}
			else{
				if(gBattery.lowLevel == BATT_ALARM_PC) gBattery.lowLevel = BATT_ALARM_OK;
				gBattery.update = 0;
			}
		}
		else{
			gBattery.lowLevel = BATT_ALARM_PC;
		}
	}
	return gBattery.lowLevel;
}

