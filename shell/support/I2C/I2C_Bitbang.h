#ifndef __I2C_BITBANG_H
#define __I2C_BITBANG_H


#include "core.h"
#include "I2C_StructLib.h"




/**********************************************MASTER*****************************************************************/
void I2C_M_Bitbang_Config(I2C_M_Bitbang_t *I2C_Driver);
void I2C_M_Bitbang_Peripheral(I2C_M_Bitbang_t *I2C_Driver);
void I2C_S_Bitbang_Config(I2C_S_Bitbang_t *I2C_Driver);
void I2C_S_Bitbang_Peripheral(I2C_S_Bitbang_t *I2C_Driver);

void I2C_M_Bitbang_Detect(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr);
void I2C_M_Bitbang_Set(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
void I2C_M_Bitbang_Get(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr,uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size);
void I2C_M_Bitbang_Write(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
void I2C_M_Bitbang_Read(I2C_M_Bitbang_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size);
void I2C_M_Bitbang_Close(I2C_M_Bitbang_t *I2C_Driver);
uint8_t I2C_M_Bitbang_Endp(I2C_M_Bitbang_t *I2C_Driver);
uint8_t I2C_M_Bitbang_Open(I2C_M_Bitbang_t *I2C_Driver);
uint8_t I2C_M_Bitbang_Result(I2C_M_Bitbang_t *I2C_Driver);
/**********************************************MASTER*****************************************************************/


#endif 
