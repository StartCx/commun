#ifndef __I2C_SOFTWARE_H
#define __I2C_SOFTWARE_H


#include "core.h"
#include "I2C_StructLib.h"


void I2C_M_Software_Config(I2C_M_Software_t *I2C_Driver);
void I2C_M_Software_Close(I2C_M_Software_t *I2C_Driver);
uint8_t I2C_M_Software_Open(I2C_M_Software_t *I2C_Driver);
uint8_t I2C_M_Software_Endp(I2C_M_Software_t *I2C_Driver);
uint8_t I2C_M_Software_Result(I2C_M_Software_t *I2C_Driver);
uint8_t I2C_M_Software_Set(I2C_M_Software_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Software_Get(I2C_M_Software_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Software_Write(I2C_M_Software_t *I2C_Driver,uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Software_Read(I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
uint8_t I2C_M_Software_Detect(I2C_M_Software_t *I2C_Driver, uint8_t _Address);


#endif
