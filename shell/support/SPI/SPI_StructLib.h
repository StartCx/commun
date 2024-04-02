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
	
	GPIO_TypeDef	*PORT_CS[2];//�˿�
	uint16_t 		PIN_CS[2];	//����
	GPIO_TypeDef	*PORT_SCK;	//�˿�
	uint16_t 		PIN_SCK;	//����
	GPIO_TypeDef	*PORT_MISO;	//�˿�
	uint16_t 		PIN_MISO;	//����
	GPIO_TypeDef	*PORT_MOSI;	//�˿�
	uint16_t 		PIN_MOSI;	//����
	
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
	uint8_t 		_CPOL;
	uint8_t 		_CPHA;
	uint8_t			Lock;
	uint8_t 		Mask;
	uint16_t 		Index;
	uint16_t 		Delay_time;	//�ӳ�ʱ��
	uint16_t 		Delay_cnt;	//�ӳټ���
	uint16_t 		Rx_Data;	//����һ�ֽ�
	
	GPIO_TypeDef	*PORT_CS[2];//�˿�
	uint16_t 		PIN_CS[2];	//����
	GPIO_TypeDef	*PORT_SCK;	//�˿�
	uint16_t 		PIN_SCK;	//����
	GPIO_TypeDef	*PORT_MISO;	//�˿�
	uint16_t 		PIN_MISO;	//����
	GPIO_TypeDef	*PORT_MOSI;	//�˿�
	uint16_t 		PIN_MOSI;	//����
	
	void (*Init)(struct SPI_M_Software_t *SPI_Driver);
	void (*Close)(struct SPI_M_Software_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Software_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Software_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Software_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	
}SPI_M_Software_t;


typedef struct SPI_M_Hardware_t
{	
	SPI_TypeDef* 	SPIx;
	uint16_t 		_CPOL;
	uint16_t 		_CPHA;
	uint8_t			Lock;
	uint16_t 		Index;
	uint16_t 		Delay_time;	//�ӳ�ʱ��
	uint16_t 		Delay_cnt;	//�ӳټ���
	uint16_t 		Rx_Data;	//����һ�ֽ�
	
	GPIO_TypeDef	*PORT_CS[2];//�˿�
	uint16_t 		PIN_CS[2];	//����
	GPIO_TypeDef	*PORT_SCK;	//�˿�
	uint16_t 		PIN_SCK;	//����
	GPIO_TypeDef	*PORT_MISO;	//�˿�
	uint16_t 		PIN_MISO;	//����
	GPIO_TypeDef	*PORT_MOSI;	//�˿�
	uint16_t 		PIN_MOSI;	//����
	
	void (*Init)(struct SPI_M_Hardware_t *SPI_Driver);
	void (*Close)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t ID);
	uint8_t (*Open)(struct SPI_M_Hardware_t *SPI_Driver, uint8_t ID);
	uint8_t (*Endp)(struct SPI_M_Hardware_t *SPI_Driver);
	void (*WriteAndRead)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*WriteThenRead)(struct SPI_M_Hardware_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	
}SPI_M_Hardware_t;


#endif