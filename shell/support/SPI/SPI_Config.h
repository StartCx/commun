#ifndef __SPI_CONFIG_H
#define __SPI_CONFIG_H



#include "core.h"
#include "SPI_StructLib.h"
#include "SPI_Bitbang.h"
#include "SPI_Software.h"
#include "SPI_Hardware.h"



enum
{
	SPI1_M_HARDWARE = 0,
	SPI2_M_BITBANG,
	SPI3_M_SOFTWARE,
	SPI_BUS_SUM,
};


void SPIx_Init(uint8_t Bus);
void SPIx_Peripheral(uint8_t Bus);
void SPIx_Close(uint8_t Bus, uint8_t CSx);
uint8_t SPIx_Open(uint8_t Bus, uint8_t CSx);
uint8_t SPIx_Endp(uint8_t Bus);
void SPIx_WriteAndRead(uint8_t Bus, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
void SPIx_WriteThenRead(uint8_t Bus, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
void SPIx_Mode(uint8_t Bus, spi_mode_e spi_mode);


#endif
