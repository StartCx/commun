#ifndef __SPI_STRUCTLIB_H
#define __SPI_STRUCTLIB_H



#include "core.h"
#include "SPI_StructLib.h"

typedef enum
{
	SPI_MODE_0,//CPOL = 0,CPHA = 0;
	SPI_MODE_1,//CPOL = 0,CPHA = 1;
	SPI_MODE_2,//CPOL = 1,CPHA = 0;
	SPI_MODE_3,//CPOL = 1,CPHA = 1;
}spi_mode_e;

typedef struct SPI_M_Bitbang_t
{	
	spi_mode_e SPI_Mode;
	uint8_t WR_Mode;
	uint8_t *pTx_data;
	uint8_t *pRx_data;
	uint8_t *pRx_data_bak;
	uint16_t TxSize;
	uint16_t RxSize;
	uint16_t Size;
	uint8_t  Func_Index;
	
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
	void (*Mode)(struct SPI_M_Bitbang_t *SPI_Driver,spi_mode_e spi_mode);
}SPI_M_Bitbang_t;


typedef struct SPI_M_Software_t
{	
	spi_mode_e 		SPI_Mode;
	uint8_t			Lock;
	uint8_t 		Mask;
	uint16_t 		Index;
	uint16_t 		Delay_time;	//延迟时间
	uint16_t 		Delay_cnt;	//延迟计数
	uint16_t 		Rx_Data;	//接收一字节
	
	GPIO_TypeDef	*PORT_CS[2];//端口
	uint16_t 		PIN_CS[2];	//引脚
	GPIO_TypeDef	*PORT_SCK;	//端口
	uint16_t 		PIN_SCK;	//引脚
	GPIO_TypeDef	*PORT_MISO;	//端口
	uint16_t 		PIN_MISO;	//引脚
	GPIO_TypeDef	*PORT_MOSI;	//端口
	uint16_t 		PIN_MOSI;	//引脚
	
	uint8_t (*SPI_Mode_pFunc)(struct SPI_M_Software_t *SPI_Driver,uint8_t TxData);
	void (*Init)(struct SPI_M_Software_t *SPI_Driver);
	void (*Close)(struct SPI_M_Software_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Software_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Software_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	void (*Mode)(struct SPI_M_Software_t *SPI_Driver,spi_mode_e spi_mode);
}SPI_M_Software_t;


typedef struct SPI_M_Hardware_t
{	
	SPI_TypeDef* 	SPIx;
	spi_mode_e 		SPI_Mode;
	uint8_t			Lock;
	uint16_t 		Index;
	uint16_t 		Delay_time;	//延迟时间
	uint16_t 		Delay_cnt;	//延迟计数
	uint16_t 		Rx_Data;	//接收一字节
	
	GPIO_TypeDef	*PORT_CS[2];//端口
	uint16_t 		PIN_CS[2];	//引脚
	GPIO_TypeDef	*PORT_SCK;	//端口
	uint16_t 		PIN_SCK;	//引脚
	GPIO_TypeDef	*PORT_MISO;	//端口
	uint16_t 		PIN_MISO;	//引脚
	GPIO_TypeDef	*PORT_MOSI;	//端口
	uint16_t 		PIN_MOSI;	//引脚
	
	void (*Init)(struct SPI_M_Hardware_t *SPI_Driver);
	void (*Close)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Hardware_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Hardware_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	void (*Mode)(struct SPI_M_Hardware_t *SPI_Driver,spi_mode_e spi_mode);
}SPI_M_Hardware_t;


#endif
