#include "stm32f10x.h"
#define MPU6050     0xD0
#define I2C_ACK_ATTEMPTS 10000

ErrorStatus I2CStatus(u8 DeviceID);
int I2CWriteByte(u8 DevideID,u8 RegName, u8 RegValue);
int I2CReadFromReg(u8 DevideID,u8 RegName);
int I2CWaitForEvent(uint32_t event);
void I2CSettingMPU6050(void) ;
void i2cInit(void); 
