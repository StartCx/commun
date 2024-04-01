#ifndef __I2C_CONFIG_H
#define __I2C_CONFIG_H



#include "core.h"
#include "I2C_StructLib.h"
#include "I2C_Bitbang.h"
#include "I2C_Software.h"
#include "I2C_Hardware.h"


extern I2C_M_Hardware_t I2C1_Master;
extern I2C_M_Bitbang_t  I2C2_Master;
extern I2C_M_Software_t I2C3_Master;
extern I2C_S_Bitbang_t  I2C4_Slave;

enum
{
	I2C1_BUS,
	I2C2_BUS,
	I2C3_BUS,
	I2C4_BUS,
	I2C_BUS_SUM,
};

/***********************************************************************************************/

void I2Cx_Init(uint8_t Bus);
void I2Cx_Close(uint8_t Bus);
uint8_t I2Cx_Open(uint8_t Bus);
uint8_t I2Cx_Endp(uint8_t Bus);
uint8_t I2Cx_Result(uint8_t Bus);
void I2Cx_Set(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
void I2Cx_Get(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
void I2Cx_Write(uint8_t Bus, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
void I2Cx_Read(uint8_t Bus, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
void I2Cx_Detect(uint8_t Bus, uint8_t Dev_Addr);

/***********************************************************************************************/







#endif
