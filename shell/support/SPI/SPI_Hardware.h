#ifndef __SPI_HARDWARE_H
#define __SPI_HARDWARE_H


#include "core.h"
#include "SPI_StructLib.h"


uint8_t SPI_M_Hardware_Endp(SPI_M_Hardware_t *SPI_Driver);
uint8_t SPI_M_Hardware_Open(SPI_M_Hardware_t *SPI_Driver, uint8_t CSx);
void SPI_M_Hardware_Config(SPI_M_Hardware_t *SPI_Driver);
void SPI_M_Hardware_Close(SPI_M_Hardware_t *SPI_Driver, uint8_t CSx);
void SPI_M_Hardware_WriteAndRead(SPI_M_Hardware_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
void SPI_M_Hardware_WriteThenRead(SPI_M_Hardware_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
void SPI_M_Hardware_Mode(SPI_M_Hardware_t *SPI_Driver,spi_mode_e spi_mode);


#endif
