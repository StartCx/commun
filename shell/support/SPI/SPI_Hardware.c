#include "SPI_Hardware.h"


void SPI_M_Hardware_Config(SPI_M_Hardware_t *SPI_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/* CS */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[0];
	GPIO_Init(SPI_Driver->PORT_CS[0], &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[1];
	GPIO_Init(SPI_Driver->PORT_CS[1], &GPIO_InitStructure);
	/* SCK */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_SCK;
	GPIO_Init(SPI_Driver->PORT_SCK, &GPIO_InitStructure);
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MOSI;
	GPIO_Init(SPI_Driver->PORT_MOSI, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MISO;
	GPIO_Init(SPI_Driver->PORT_MISO, &GPIO_InitStructure);

	GPIO_SET_HIGH(SPI_Driver->PORT_CS[0], SPI_Driver->PIN_CS[0]);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[1], SPI_Driver->PIN_CS[1]);
	
	/* SPI ģʽ���� */
	// FLASHоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_Driver->_CPOL;
	SPI_InitStructure.SPI_CPHA = SPI_Driver->_CPHA;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_Driver->SPIx , &SPI_InitStructure);
	/* ʹ�� SPI  */
	SPI_Cmd(SPI_Driver->SPIx , ENABLE);
}




uint8_t SPI_M_Hardware_Open(SPI_M_Hardware_t *SPI_Driver, uint8_t CSx)
{
	if( SPI_Driver->Lock == CORE_UNLOCK){
		SPI_Driver->Lock =  CORE_LOCK;
		GPIO_SET_LOW(SPI_Driver->PORT_CS[CSx], SPI_Driver->PIN_CS[CSx]);
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}



void SPI_M_Hardware_Close(SPI_M_Hardware_t *SPI_Driver, uint8_t CSx)
{
	SPI_Driver->Lock = CORE_UNLOCK;
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[CSx], SPI_Driver->PIN_CS[CSx]);
}

uint8_t SPI_M_Hardware_Endp(SPI_M_Hardware_t *SPI_Driver)
{
	return CORE_DONE;
}


/**************************************************************************************************************/
uint8_t SPI_M_Hardware_WriteRead(SPI_M_Hardware_t *SPI_Driver,uint8_t TxData)
{	
	SPI_Driver->Delay_cnt = SPI_Driver->Delay_time;
	while( SPI_I2S_GetFlagStatus(SPI_Driver->SPIx , SPI_I2S_FLAG_TXE) == RESET){
		if((SPI_Driver->Delay_cnt--) == 0) break;
	}
	SPI_I2S_SendData(SPI_Driver->SPIx , TxData);
	SPI_Driver->Delay_cnt = SPI_Driver->Delay_time;
	while (SPI_I2S_GetFlagStatus(SPI_Driver->SPIx , SPI_I2S_FLAG_RXNE) == RESET){
		if((SPI_Driver->Delay_cnt--) == 0) break;
	}
	return SPI_I2S_ReceiveData(SPI_Driver->SPIx );
}

void SPI_M_Hardware_WriteAndRead(SPI_M_Hardware_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	if( (pTxData == NULL && pRxData == NULL) || Size == 0){
		return;
	}
	if( pTxData != NULL && pRxData != NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Hardware_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}else if( pTxData != NULL && pRxData == NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			SPI_M_Hardware_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}else if( pTxData == NULL && pRxData != NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Hardware_WriteRead(SPI_Driver,0xFF);
		}
	}
}


void SPI_M_Hardware_WriteThenRead(SPI_M_Hardware_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize)
{
	if( pTxData != NULL && TxSize > 0){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < TxSize; SPI_Driver->Index++)
		{
			SPI_M_Hardware_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}
	if( pRxData != NULL && RxSize > 0){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < RxSize; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Hardware_WriteRead(SPI_Driver,0xFF);
		}
	}
}









