#include "SPI_Software.h"



uint8_t SPI_M_Software_Open(SPI_M_Software_t *SPI_Driver, uint8_t CSx)
{
	if( SPI_Driver->Lock == CORE_UNLOCK && CSx < SPI_Driver->CS_SUM){
		SPI_Driver->Lock =  CORE_LOCK;
		GPIO_SET_LOW(SPI_Driver->CS_Pins[CSx].GPIO_Port, SPI_Driver->CS_Pins[CSx].Pin);
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void SPI_M_Software_Close(SPI_M_Software_t *SPI_Driver, uint8_t CSx)
{
	SPI_Driver->Lock = CORE_UNLOCK;
	if( CSx < SPI_Driver->CS_SUM){
		GPIO_SET_HIGH(SPI_Driver->CS_Pins[CSx].GPIO_Port, SPI_Driver->CS_Pins[CSx].Pin);
	}
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


uint8_t SPI_M_Software_WriteRead_Mode0(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	SPI_Driver->Rx_Data = 0;
	if( SPI_Driver->First_Bit == MSB_FIRST){
		goto MSB_FIRST;
	}else{
		goto LSB_FIRST;
	}
MSB_FIRST:
	for( SPI_Driver->Mask=0x80; SPI_Driver->Mask>0; SPI_Driver->Mask>>=1)
	{
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高 上升沿
		SPI_M_Software_Delay(SPI_Driver);
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
LSB_FIRST:	
	for( SPI_Driver->Mask=0x01; SPI_Driver->Mask<=0x80; SPI_Driver->Mask<<=1)
	{
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高 上升沿
		SPI_M_Software_Delay(SPI_Driver);
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
}


uint8_t SPI_M_Software_WriteRead_Mode1(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	SPI_Driver->Rx_Data = 0;
	if( SPI_Driver->First_Bit == MSB_FIRST){
		goto MSB_FIRST;
	}else{
		goto LSB_FIRST;
	}
MSB_FIRST:
	for( SPI_Driver->Mask=0x80; SPI_Driver->Mask>0; SPI_Driver->Mask>>=1)
	{
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高 上升沿
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		SPI_M_Software_Delay(SPI_Driver);
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
LSB_FIRST:		
	for( SPI_Driver->Mask=0x01; SPI_Driver->Mask<=0x80; SPI_Driver->Mask<<=1)
	{
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高 上升沿
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		SPI_M_Software_Delay(SPI_Driver);
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
}


uint8_t SPI_M_Software_WriteRead_Mode2(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	SPI_Driver->Rx_Data = 0;
	if( SPI_Driver->First_Bit == MSB_FIRST){
		goto MSB_FIRST;
	}else{
		goto LSB_FIRST;
	}
MSB_FIRST:
	for( SPI_Driver->Mask=0x80; SPI_Driver->Mask>0; SPI_Driver->Mask>>=1)
	{
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低
		SPI_M_Software_Delay(SPI_Driver);
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高
		SPI_M_Software_Delay(SPI_Driver);		
	}
	return SPI_Driver->Rx_Data;
LSB_FIRST:		
	for( SPI_Driver->Mask=0x01; SPI_Driver->Mask<=0x80; SPI_Driver->Mask<<=1)
	{
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低
		SPI_M_Software_Delay(SPI_Driver);
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高
		SPI_M_Software_Delay(SPI_Driver);		
	}
	return SPI_Driver->Rx_Data;
}


uint8_t SPI_M_Software_WriteRead_Mode3(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	SPI_Driver->Rx_Data = 0;
	if( SPI_Driver->First_Bit == MSB_FIRST){
		goto MSB_FIRST;
	}else{
		goto LSB_FIRST;
	}
MSB_FIRST:
	for( SPI_Driver->Mask=0x80; SPI_Driver->Mask>0; SPI_Driver->Mask>>=1)
	{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		SPI_M_Software_Delay(SPI_Driver);
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高   
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
LSB_FIRST:		
	for( SPI_Driver->Mask=0x01; SPI_Driver->Mask<=0x80; SPI_Driver->Mask<<=1)
	{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 低
		if( TxData & SPI_Driver->Mask){ //发送数据
			GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}else{
			GPIO_SET_LOW(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
		}
		SPI_M_Software_Delay(SPI_Driver);
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK,  SPI_Driver->PIN_SCK);//时钟 - 高   
		if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO)){
			SPI_Driver->Rx_Data |= SPI_Driver->Mask;
		}else{
			SPI_Driver->Rx_Data &= ~SPI_Driver->Mask;
		}
		SPI_M_Software_Delay(SPI_Driver);
	}
	return SPI_Driver->Rx_Data;
}

// 切换SPI模式
void SPI_M_Software_Mode(SPI_M_Software_t *SPI_Driver,spi_mode_e spi_mode) {
	SPI_Driver->SPI_Mode = spi_mode;
    switch(SPI_Driver->SPI_Mode){
		case SPI_MODE_0:
			SPI_Driver->SPI_Mode_pFunc = SPI_M_Software_WriteRead_Mode0;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_1:
			SPI_Driver->SPI_Mode_pFunc = SPI_M_Software_WriteRead_Mode1;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_2:
			SPI_Driver->SPI_Mode_pFunc = SPI_M_Software_WriteRead_Mode2;
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_3:
			SPI_Driver->SPI_Mode_pFunc = SPI_M_Software_WriteRead_Mode3;
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		default:
			SPI_Driver->SPI_Mode_pFunc = SPI_M_Software_WriteRead_Mode0;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
	}
}


void SPI_M_Software_Config(SPI_M_Software_t *SPI_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/* CS */
	for(uint8_t i = 0; i<SPI_Driver->CS_SUM;i++)
	{
		GPIO_InitStructure.GPIO_Pin = SPI_Driver->CS_Pins[i].Pin;
		GPIO_Init(SPI_Driver->CS_Pins[i].GPIO_Port, &GPIO_InitStructure);
		GPIO_SET_HIGH(SPI_Driver->CS_Pins[i].GPIO_Port, SPI_Driver->CS_Pins[i].Pin);
	}
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

	SPI_M_Software_Mode(SPI_Driver,SPI_Driver->SPI_Mode);
	
	GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
}

/****************************************************************************************************************/

uint8_t SPI_M_Software_WriteRead(SPI_M_Software_t *SPI_Driver,uint8_t TxData)
{
	if( SPI_Driver->SPI_Mode_pFunc != NULL){
		return SPI_Driver->SPI_Mode_pFunc(SPI_Driver, TxData);
	}
	return 0;
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

