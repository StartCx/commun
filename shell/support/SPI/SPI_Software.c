#include "SPI_Software.h"



void SPI_M_Software_Config(SPI_M_Software_t *SPI_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/* CS */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[0];
	GPIO_Init(SPI_Driver->PORT_CS[0], &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[1];
	GPIO_Init(SPI_Driver->PORT_CS[1], &GPIO_InitStructure);
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_SCK;
	GPIO_Init(SPI_Driver->PORT_SCK, &GPIO_InitStructure);
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MOSI;
	GPIO_Init(SPI_Driver->PORT_MOSI, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SPI_Driver->PORT_MISO, &GPIO_InitStructure);

	//状态初始化
	if( SPI_Driver->_CPOL){
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	}
	GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[0], SPI_Driver->PIN_CS[0]);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[1], SPI_Driver->PIN_CS[1]);
}


uint8_t SPI_M_Software_Open(SPI_M_Software_t *SPI_Driver, uint8_t CSx)
{
	if( SPI_Driver->Lock == CORE_UNLOCK){
		SPI_Driver->Lock =  CORE_LOCK;
		GPIO_SET_LOW(SPI_Driver->PORT_CS[CSx], SPI_Driver->PIN_CS[CSx]);
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void SPI_M_Software_Close(SPI_M_Software_t *SPI_Driver, uint8_t CSx)
{
	SPI_Driver->Lock = CORE_UNLOCK;
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[CSx], SPI_Driver->PIN_CS[CSx]);
}

uint8_t SPI_M_Software_Endp(SPI_M_Software_t *SPI_Driver)
{
	return CORE_DONE;
}


/****************************************************************************************************************/

static void SPI_M_Software_Delay(SPI_M_Software_t *SPI_Driver)
{
	for (SPI_Driver->Delay_cnt = 0; SPI_Driver->Delay_cnt < SPI_Driver->Delay_time; SPI_Driver->Delay_cnt++);
}
//0∧0＝0，0∧1＝1， 1^0=1，1∧1＝0。
uint8_t SPI_M_Software_WriteRead(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	SPI_Driver->Rx_Data = 0;
	for( SPI_Driver->Mask=0x80; SPI_Driver->Mask>0; SPI_Driver->Mask>>=1)
	{
		if( SPI_Driver->_CPOL^SPI_Driver->_CPHA ){
			GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低
		}else{
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高 上升沿
		}
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		SPI_M_Software_Delay(SPI_Driver);
		if( SPI_Driver->_CPOL^SPI_Driver->_CPHA ){
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高   
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
		}
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= TxData & SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data |= 0x00;
		}
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
}


void SPI_M_Software_WriteAndRead(SPI_M_Software_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	if( (pTxData == NULL && pRxData == NULL) || Size == 0){
		return;
	}
	if( pTxData != NULL && pRxData != NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Software_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}else if( pTxData != NULL && pRxData == NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			SPI_M_Software_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}else if( pTxData == NULL && pRxData != NULL){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < Size; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Software_WriteRead(SPI_Driver,0xFF);
		}
	}
}


void SPI_M_Software_WriteThenRead(SPI_M_Software_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize)
{
	if( pTxData != NULL && TxSize > 0){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < TxSize; SPI_Driver->Index++)
		{
			SPI_M_Software_WriteRead(SPI_Driver,*(pTxData+SPI_Driver->Index));
		}
	}
	if( pRxData != NULL && RxSize > 0){
		for( SPI_Driver->Index = 0; SPI_Driver->Index < RxSize; SPI_Driver->Index++)
		{
			*(pRxData+SPI_Driver->Index) = SPI_M_Software_WriteRead(SPI_Driver,0xFF);
		}
	}
}

