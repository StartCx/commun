#ifndef __SPI_SOFTWARE_H
#define __SPI_SOFTWARE_H




#include "core.h"
#include "SPI_StructLib.h"



void SPI_M_Software_Config(SPI_M_Software_t *SPI_Driver);
uint8_t SPI_M_Software_Open(SPI_M_Software_t *SPI_Driver, uint8_t CSx);
void SPI_M_Software_Close(SPI_M_Software_t *SPI_Driver, uint8_t CSx);
uint8_t SPI_M_Software_Endp(SPI_M_Software_t *SPI_Driver);
void SPI_M_Software_WriteAndRead(SPI_M_Software_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
void SPI_M_Software_WriteThenRead(SPI_M_Software_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
void SPI_M_Software_Mode(SPI_M_Software_t *SPI_Driver,spi_mode_e spi_mode);



























#endif
