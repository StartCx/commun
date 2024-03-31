#ifndef __I2C_HARDWARE_H
#define __I2C_HARDWARE_H


#include "core.h"
#include "I2C_StructLib.h"


void I2C_M_Hardware_Config(I2C_M_Hardware_t *I2C_Driver);
void I2C_M_Hardware_Close(I2C_M_Hardware_t *I2C_Driver);
uint8_t I2C_M_Hardware_Open(I2C_M_Hardware_t *I2C_Driver);
uint8_t I2C_M_Hardware_Endp(I2C_M_Hardware_t *I2C_Driver);
uint8_t I2C_M_Hardware_Result(I2C_M_Hardware_t *I2C_Driver);
uint8_t I2C_M_Hardware_Set(I2C_M_Hardware_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Hardware_Get(I2C_M_Hardware_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Hardware_Write(I2C_M_Hardware_t *I2C_Driver,uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Hardware_Read(I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Hardware_Detect(I2C_M_Hardware_t *I2C_Driver, uint8_t _Address);


#endif
