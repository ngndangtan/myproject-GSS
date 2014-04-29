/**
  ******************************************************************************
  * @file: 		ublox_gps.c	  
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   read signal gps
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "ublox_gps.h"
#include "stm32f10x.h"

extern u8 check_a;
extern u8 check_b;
extern u8 UBX_step;
//
//
int data_sensor1,data_sensor2;
int sensor_head,sensor_end;

 
 //int gps_data_counter_1=0;
extern u8 gps_buffer[GPS_BUFF_SIZE];
extern u8 UBX_buffer[UBX_MAXPAYLOAD];
extern u8 Data_UBX[GPS_BUFF_SIZE];
extern gUblox_t gUblox;

float accN,accD,accE;
float accGps;


void ubx_checksum(u8 ubx_data)
{
	check_a += ubx_data;
	check_b += check_a; 
}
long join_4_bytes(u8 Buffer[])
{
	union long_union {
	int32_t dword;
	u8 byte[4];
	} longUnion;

	longUnion.byte[0] = *Buffer;
	longUnion.byte[1] = *(Buffer + 1);
	longUnion.byte[2] = *(Buffer + 2);
	longUnion.byte[3] = *(Buffer + 3);
	return(longUnion.dword);
}
void parse_ubx_gps(void)
{
	int j1;
//Verifing if we are in class 1, you can change this "IF" for a "Switch" in case you want to use other UBX classes.. 
//In this case all the message im using are in class 1, to know more about classes check PAGE 60 of DataSheet.
	if(gUblox.Class == 0x01) 
	{
		switch(gUblox.id) //Checking the UBX ID
		{
		case 0x02: // ID NAV - POSLLH 
			j1 = 0;
		
			gUblox.Time = join_4_bytes(&UBX_buffer[j1]); // ms Time of week
			j1 += 4;
			gUblox.Longitude = join_4_bytes(&UBX_buffer[j1]); // lon * 10000000
			j1 += 4;
			gUblox.Lattitude = join_4_bytes(&UBX_buffer[j1]); // lat * 10000000
			j1 += 4;
     		 //Altitude = j1oin_4_bytes(&UBX_buffer[j1]);  // elipsoid heigth mm
			j1 += 4;
			gUblox.Altitude =join_4_bytes(&UBX_buffer[j1]);	// MSL heigth mm
			//gUblox.Altitude /= 10.;									
			
	
			gUblox.NewData = 1;
			break;
		case 0x03: //ID NAV - STATUS 
		if((UBX_buffer[4] >= 0x03) && (UBX_buffer[5] & 0x01))				 
				gUblox.Fix = 1; // valid position				
			else
				gUblox.Fix = 0; // invalid position
			break;

		case 0x06: //ID NAV - SOL
			if((UBX_buffer[10] >= 0x03) && (UBX_buffer[11] & 0x01))
				gUblox.Fix = 1; // valid position
			else
				gUblox.Fix = 0; // invalid position				
	
				gUblox.NumSats = UBX_buffer[47];										// Number of sats...		 
			break;

		case 0x12: // ID NAV - VELNED 
		     
			gUblox.velN= join_4_bytes(&UBX_buffer[4]); // cm / s
			gUblox.velE= join_4_bytes(&UBX_buffer[8]); // cm / s
			gUblox.velD= join_4_bytes(&UBX_buffer[12]); // cm / s
			j1 = 16;
			gUblox.Speed_3d = join_4_bytes(&UBX_buffer[j1]); // cm / s
			j1 += 4;
			gUblox.Ground_Speed = join_4_bytes(&UBX_buffer[j1]); // Ground speed 2D cm / s
			gUblox.Ground_Speed/=100;							  // Ground speed 2D m / s
			j1 += 4;
			gUblox.Ground_Course = join_4_bytes(&UBX_buffer[j1]); // Heading 2D deg * 100000
			gUblox.Ground_Course /= 100000;	// Rescale heading to deg * 100
			j1 += 4;
  
			break; 
			}
		}	 
}

int gss_sensor(void)
{
	  int i1, k1, numc,i2;
	  static int gps_data_counter_1=0;
	  u8 data1,data2;
	  int gps_data_counter;
		
	  static int count;
		

		gps_data_counter=GPS_BUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel6);
	
		numc=gps_data_counter-gps_data_counter_1;
		if (numc<0) numc=GPS_BUFF_SIZE+numc;
		if (numc)
		{ 
		   for (i1=0;i1<numc;i1++)
		   {  
				 k1=i1+gps_data_counter_1;
		     if (k1>=200)   k1-=200;
				 
		     Data_UBX[i1]=gps_buffer[k1] ;
		   }
		   
			  gps_data_counter_1=gps_data_counter;

	
		
			for(i1 = 0; i1 < numc; i1++)	// Process bytes received
			{
				data1=Data_UBX[i1];
				if(data1==0xfe) count ++;else count=0;
				if(count==2) i2=0;
				
				switch(i2)
				{
					case 1:
						sensor_head=data1;
						break;
					case 2:
//						data_sensor1=data1;
							if(data1&0x01)
								{
									data_sensor1 =-128;
								}
							else if (data1&0x02){
								data_sensor1 	=-128;
								}
							else if (data1&0x04){
								data_sensor1 	=-128;
								}
							else if (data1&0x08){
								data_sensor1 	=-128;
								}
							else if (data1&0x10){
									data_sensor1 =-64;
								}
							else if (data1&0x20){
									data_sensor1 =0;
								}
							else if (data1&0x40){
									data_sensor1 =0;
								}
							else if (data1&0x80){
								data_sensor1 	=64;
								}
							else {
								data_sensor1		=255;
								}
						break;
					case 3:
//						data_sensor2=data1;
							if(data1&0x01)
								{
									data_sensor2 =-128;
								}
							else if (data1&0x02){
								data_sensor2 	=-128;
								}
							else if (data1&0x04){
								data_sensor2 	=-128;
								}
							else if (data1&0x08){
								data_sensor2 	=-128;
								}
							else if (data1&0x10){
									data_sensor2 =-64;
								}
							else if (data1&0x20){
									data_sensor2 =0;
								}
							else if (data1&0x40){
									data_sensor2 =0;
								}
							else if (data1&0x80){
								data_sensor2 	=64;
								}
							else {
								data_sensor2		=255;
								}
						break;
					case 4:
						sensor_end=data1;
						break;
					default:
						break;
				}
				i2++;
			
			}
		}
	 return 1;
}
int read_ubx_gps(void)
{
//	  int i1, k1, numc;
//	  static int gps_data_counter_1=0;
//	  u8 data1;
//	  int gps_data_counter;
//	  
//		

//		gps_data_counter=GPS_BUFF_SIZE-DMA_GetCurrDataCounter(DMA1_Channel6);
//	
//		numc=gps_data_counter-gps_data_counter_1;
//		if (numc<0) numc=GPS_BUFF_SIZE+numc;
//		if (numc)
//		{ 
//		   for (i1=0;i1<numc;i1++)
//		   {  
//				 k1=i1+gps_data_counter_1;
//		     if (k1>=GPS_BUFF_SIZE)   k1-=GPS_BUFF_SIZE;
//				 
//		     Data_UBX[i1]=gps_buffer[k1] ;
//		   }
//		   
//			  gps_data_counter_1=gps_data_counter;

//	
//		
//			for(i1 = 0; i1 < numc; i1++)	// Process bytes received
//			{
//				data1=Data_UBX[i1];
//				switch(UBX_step)		 // Normally we start from zero. This is a state machine
//				{
//				case 0:	
//					if(data1 == 0xB5)	// UBX sync char 1
//						UBX_step++;	 // OH first data packet is correct, so jump to the next step
//				break; 
//				case 1:	
//					if(data1 == 0x62)	// UBX sync char 2
//						UBX_step++;	 // ooh! The second data packet is correct, jump to the step 2
//					else 
//						UBX_step = 0;	 // Nop, is not correct so restart to step zero and try again.		 
//				break;
//				case 2:
//					gUblox.Class = data1;
//					ubx_checksum(gUblox.Class);
//					UBX_step++;
//				break;
//				case 3:
//					gUblox.id = data1;
//					ubx_checksum(gUblox.id);
//					UBX_step++;
//				break;
//				case 4:
//					gUblox.length_hi = data1;
//					ubx_checksum(gUblox.length_hi);
//					UBX_step++;
//					// We check if the payload length is valid...
//					if (gUblox.length_hi >= UBX_MAXPAYLOAD)
//					{
//					//	printf("ERR:GPS_BAD_PAYLOAD_LENGTH!!");          
//						UBX_step = 0;	 // Bad data, so restart to step zero and try again.		 
//						check_a = 0;
//						check_b = 0;
//					}
//				break;
//				case 5:
//					gUblox.length_lo = data1;
//					ubx_checksum(gUblox.length_lo);
//					UBX_step++;
//					gUblox.payload_counter = 0;
//				break;
//				case 6:				 // Payload data read...
//					if (gUblox.payload_counter < gUblox.length_hi)	// We stay in this state until we reach the payload_length
//					{
//						UBX_buffer[gUblox.payload_counter] = data1;
//						ubx_checksum(data1);
//						gUblox.payload_counter++;
//						if (gUblox.payload_counter == gUblox.length_hi)
//							UBX_step++;
//					}
//				break;
//				case 7:
//					gUblox.ck_a = data1;	 // First checksum byte
//					UBX_step++;
//				break;
//				case 8:
//					gUblox.ck_b = data1;	 // Second checksum byte
//				  // We end the GPS read...
//					if((check_a == gUblox.ck_a) && (check_b == gUblox.ck_b))	 // Verify the received checksum with the generated checksum.. 
//					{
//						// Parse the new GPS packet
//						parse_ubx_gps();
//					}
//					else
//					{
//					//	printf("ERR:GPS_CHK!!");
//					}
//        	// Variable initialization
//					UBX_step = 0;
//					check_a = 0;
//					check_b = 0;
//				break;
//				}
//			}
//		}
//	 return numc;
}

u8 gpsCorrection(void){
	if((gUblox.Fix == 1) && (gUblox.NumSats > NUM_SAT_ENABLE)){
		accN = (gUblox.velN - gUblox.velNold)/RATE_GPS;
		accD = (gUblox.velD - gUblox.velDold)/RATE_GPS;
		accE = (gUblox.velE - gUblox.velEold)/RATE_GPS;
		gUblox.velNold = gUblox.velN;
		gUblox.velDold = gUblox.velD;
		gUblox.velEold = gUblox.velE;
		
		accGps = sqrt(accN*accN + accD*accD + accE*accE);
		
		if(accGps > ACC_GPS_MIN)	return 0;
	}
	
	return 1;
}

void ubloxInit(void)
{	 
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Configure PA3 for USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Set address for DMA USART1 RX*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;
	/* Set address for DMA memory */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) gps_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = GPS_BUFF_SIZE; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/* DMA1_Channel6 configuration */
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	
	/* Enable DMA interupt */
	
	/* Enable DMA */
	DMA_Cmd(DMA1_Channel6, DISABLE);
	
	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable USART  DMA */
 	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
	/* Enable USART */
	USART_Cmd(USART2, DISABLE);
}
