/**
  ******************************************************************************
  * @file: 	 	ublox_gps.h 
  * @author: 	GREMSY Team
  * @version: 1.2.0
  * @date:    2/11/2013
  * @brief:   khai bao cho file ublox_gps.c
  ******************************************************************************
	
   <h2><center> GREMSY Co.,Ltd </center></h2>
*/ 
#include "globalData.h"

#define ACC_GPS_MIN 		1.3
#define NUM_SAT_ENABLE 	3
#define RATE_GPS				10.0

long join_4_bytes(unsigned char Buffer[]);
void parse_ubx_gps(void);
int read_ubx_gps(void);
void ubloxInit(void);
u8 gpsCorrection(void);

//
int gss_sensor(void);
