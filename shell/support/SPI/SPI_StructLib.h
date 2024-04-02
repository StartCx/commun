#ifndef __SPI_STRUCTLIB_H
#define __SPI_STRUCTLIB_H



#include "core.h"
#include "SPI_StructLib.h"


typedef struct SPI_M_Bitbang_t
{	
	uint8_t _CPOL;
	uint8_t _CPHA;
	
	uint8_t Mode;
	uint8_t *pTx_data;
	uint8_t *pRx_data;
	uint8_t *pRx_data_bak;
	uint16_t TxSize;
	uint16_t RxSize;
	uint16_t Size;
	
	GPIO_TypeDef	*PORT_CS[2];//端口
	uint16_t 		PIN_CS[2];	//引脚
	GPIO_TypeDef	*PORT_SCK;	//端口
	uint16_t 		PIN_SCK;	//引脚
	GPIO_TypeDef	*PORT_MISO;	//端口
	uint16_t 		PIN_MISO;	//引脚
	GPIO_TypeDef	*PORT_MOSI;	//端口
	uint16_t 		PIN_MOSI;	//引脚
	
	CPU_RegisterClass_t Register;
	
	void (*Init)(struct SPI_M_Bitbang_t *SPI_Driver);
	void (*Driver)(struct SPI_M_Bitbang_t *SPI_Driver);
	void (*Close)(struct SPI_M_Bitbang_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Bitbang_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Bitbang_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Bitbang_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Bitbang_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	
}SPI_M_Bitbang_t;


typedef struct SPI_M_Software_t
{	
	uint8_t _CPOL;
	uint8_t _CPHA;
	
	GPIO_TypeDef	*PORT_CS[2];//端口
	uint16_t 		PIN_CS[2];	//引脚
	GPIO_TypeDef	*PORT_SCK;	//端口
	uint16_t 		PIN_SCK;	//引脚
	GPIO_TypeDef	*PORT_MISO;	//端口
	uint16_t 		PIN_MISO;	//引脚
	GPIO_TypeDef	*PORT_MOSI;	//端口
	uint16_t 		PIN_MOSI;	//引脚
	
	void (*Init)(struct SPI_M_Software_t *SPI_Driver);
	void (*Driver)(struct SPI_M_Software_t *SPI_Driver);
	void (*Close)(struct SPI_M_Software_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Software_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Software_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	
}SPI_M_Software_t;


#endif
