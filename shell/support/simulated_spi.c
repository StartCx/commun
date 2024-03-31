#include "simulated_spi.h"


SPI_Driver_t SPI_Driver0 = {
	._CPOL = 0,
	._CPHA = 0,
	.Register.R12_Lock = CORE_UNLOCK,
	.PORT_SCK  = GPIOA,
	.PIN_SCK   = GPIO_Pin_5,
	.PORT_MISO = GPIOA,
	.PIN_MISO  = GPIO_Pin_6,
	.PORT_MOSI = GPIOA,
	.PIN_MOSI  = GPIO_Pin_7,
	.PORT_CS[0] = GPIOB,
	.PIN_CS[0]  = GPIO_Pin_9,
	.PORT_CS[1] = GPIOB,
	.PIN_CS[1]  = GPIO_Pin_4,
	
	.Init			=SPI_Configuration,
	.Driver			=SPI_Period_Driver,
	.write_and_read	=spi_write_and_read,
	.write_then_read=spi_write_then_read,
	.open			=spi_open_dev,
	.close			=spi_close_dev,
	.endp			=spi_write_read_endp,
};

uint8_t spi_open_dev(SPI_Driver_t *SPI_Driver, uint8_t ID)
{
	if( SPI_Driver->Register.R12_Lock == CORE_UNLOCK && SPI_Driver->Register.R15_PC == SPI_DRIVER_PROC_READY){
		SPI_Driver->Register.R12_Lock =  CORE_LOCK;
		GPIO_SET_LOW(SPI_Driver->PORT_CS[ID], SPI_Driver->PIN_CS[ID]);
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void spi_close_dev(SPI_Driver_t *SPI_Driver, uint8_t ID)
{
	SPI_Driver->Register.R12_Lock = CORE_UNLOCK;
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[ID], SPI_Driver->PIN_CS[ID]);
}

void spi_write_and_read(SPI_Driver_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	SPI_Driver->pTx_data = pTxData;
	SPI_Driver->pRx_data = pRxData;
	SPI_Driver->Size     = Size;
	SPI_Driver->Mode = SPI_WRITE_AND_READ;
}

void spi_write_then_read(SPI_Driver_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize)
{
	SPI_Driver->pTx_data = pTxData;
	SPI_Driver->TxSize   = TxSize;
	SPI_Driver->pRx_data = pRxData;
	SPI_Driver->RxSize   = RxSize;
	SPI_Driver->Mode = SPI_WRITE_THEN_READ;
}


uint8_t spi_write_read_endp(SPI_Driver_t *SPI_Driver)
{
	if( SPI_Driver->Register.R15_PC == SPI_DRIVER_PROC_READY && SPI_Driver->Mode == SPI_IDLE){
		return CORE_DONE;
	}else{
		return CORE_RUNNING;
	}
}




void SPI_Configuration(SPI_Driver_t *SPI_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* CS */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[0];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_Driver->PORT_CS[0], &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[1];
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_Driver->PORT_CS[1], &GPIO_InitStructure);
	
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_Driver->PORT_SCK, &GPIO_InitStructure);

	/* MISO */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SPI_Driver->PORT_MISO, &GPIO_InitStructure);

	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_Driver->PORT_MOSI, &GPIO_InitStructure);
	//状态初始化
	if( SPI_Driver->_CPOL){
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	}
	GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[0], SPI_Driver->PIN_CS[0]);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[1], SPI_Driver->PIN_CS[1]);
}



void SPI_Period_Driver(SPI_Driver_t *SPI_Driver)
{
	static const void *function[SPI_DRIVER_SUM] = {
		[SPI_DRIVER_PROC_READY] 	= &&SPI_DRIVER_PROC_READY,
		[SPI_DRIVER_MODE_SELECT] 	= &&SPI_DRIVER_MODE_SELECT,
		
		[SPI_DRIVER_WRITE_THEN_READ0] 	= &&SPI_DRIVER_WRITE_THEN_READ0,
		[SPI_DRIVER_WRITE_THEN_READ1] 	= &&SPI_DRIVER_WRITE_THEN_READ1,
		
		[SPI_DRIVER_MODE_PARAM_INIT] 	= &&SPI_DRIVER_MODE_PARAM_INIT,
		[SPI_DRIVER_TX_DATA_SET]	= &&SPI_DRIVER_TX_DATA_SET,
		[SPI_DRIVER_LOOP_TX_DATA] 	= &&SPI_DRIVER_LOOP_TX_DATA,
		[SPI_DRIVER_LOOP_RX_DATA]	= &&SPI_DRIVER_LOOP_RX_DATA,
		[SPI_DRIVER_MASK_SHIFT]		= &&SPI_DRIVER_MASK_SHIFT,
		[SPI_DRIVER_RX_DATA_GET]	= &&SPI_DRIVER_RX_DATA_GET,
		[SPI_DRIVER_SIZE_COMPARE]	= &&SPI_DRIVER_SIZE_COMPARE,
		[SPI_DRIVER_PROC_ENDP]		= &&SPI_DRIVER_PROC_ENDP,
	};
	
	goto *function[SPI_Driver->Register.R15_PC];
	
SPI_DRIVER_PROC_READY:
	if( SPI_Driver->Mode != SPI_IDLE){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_MODE_SELECT;
	}
	return;
SPI_DRIVER_MODE_SELECT:
	if( SPI_Driver->Mode == SPI_WRITE_AND_READ){
		SPI_Driver->Register.R14_LR = SPI_DRIVER_PROC_ENDP;
		SPI_Driver->Register.R15_PC = SPI_DRIVER_MODE_PARAM_INIT;
	}else{
		SPI_Driver->Register.R15_PC = SPI_DRIVER_WRITE_THEN_READ0;
	}
	return;
SPI_DRIVER_WRITE_THEN_READ0:
	SPI_Driver->pRx_data_bak = SPI_Driver->pRx_data;
	if( SPI_Driver->TxSize == 0 || SPI_Driver->pTx_data == NULL){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_WRITE_THEN_READ1;
	}else{
		SPI_Driver->pRx_data = NULL;
		SPI_Driver->Size = SPI_Driver->TxSize;
		SPI_Driver->Register.R14_LR = SPI_DRIVER_WRITE_THEN_READ1;
		SPI_Driver->Register.R15_PC = SPI_DRIVER_MODE_PARAM_INIT;
	}
	return;
SPI_DRIVER_WRITE_THEN_READ1:
	if( SPI_Driver->RxSize == 0 || SPI_Driver->pRx_data_bak == NULL){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_PROC_ENDP;
	}else{
		SPI_Driver->Size = SPI_Driver->RxSize;
		SPI_Driver->pTx_data = NULL;
		SPI_Driver->pRx_data = SPI_Driver->pRx_data_bak;
		SPI_Driver->Register.R14_LR = SPI_DRIVER_PROC_ENDP;
		SPI_Driver->Register.R15_PC = SPI_DRIVER_MODE_PARAM_INIT;
	}
	return;
SPI_DRIVER_MODE_PARAM_INIT:
	SPI_Driver->Register.R1_Index = 0;
	SPI_Driver->Register.R10_Mask = 0x80;
	SPI_Driver->Register.R15_PC = SPI_DRIVER_TX_DATA_SET;
	return;
SPI_DRIVER_TX_DATA_SET:
	if( SPI_Driver->pTx_data != NULL && SPI_Driver->Register.R1_Index < SPI_Driver->Size){
		SPI_Driver->Register.R3_cout = SPI_Driver->pTx_data[SPI_Driver->Register.R1_Index];
	}else{
		SPI_Driver->Register.R3_cout = 0xFF;
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_TX_DATA;
	return;
SPI_DRIVER_LOOP_TX_DATA:
	if( (SPI_Driver->_CPOL^SPI_Driver->_CPHA) == 1){
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	}
	if( SPI_Driver->Register.R3_cout & SPI_Driver->Register.R10_Mask){ //发送数据
		GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_RX_DATA;
	return;
SPI_DRIVER_LOOP_RX_DATA:
	if( (SPI_Driver->_CPOL^SPI_Driver->_CPHA) == 1){
		GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	}else{
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	}
	if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO) == 1){//读取状态
		SPI_Driver->Register.R2_cin |=  SPI_Driver->Register.R10_Mask;  // 将特定位设置为高
	}else{
		SPI_Driver->Register.R2_cin &= ~SPI_Driver->Register.R10_Mask;  // 将特定位设置为低
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_MASK_SHIFT;
	return;
SPI_DRIVER_MASK_SHIFT:
	SPI_Driver->Register.R10_Mask >>= 1;
	if( SPI_Driver->Register.R10_Mask > 0){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_TX_DATA;
	}else{
		SPI_Driver->Register.R15_PC = SPI_DRIVER_RX_DATA_GET;
	}
	return;
SPI_DRIVER_RX_DATA_GET:
	if( SPI_Driver->pRx_data != NULL && SPI_Driver->Register.R1_Index < SPI_Driver->Size){
		SPI_Driver->pRx_data[SPI_Driver->Register.R1_Index] = SPI_Driver->Register.R2_cin;
	}else{
		SPI_Driver->Register.R2_cin = 0xFF;
	}
	SPI_Driver->Register.R10_Mask = 0x80;
	SPI_Driver->Register.R15_PC = SPI_DRIVER_SIZE_COMPARE;
	return;
SPI_DRIVER_SIZE_COMPARE:
	SPI_Driver->Register.R1_Index++;
	if( SPI_Driver->Register.R1_Index < SPI_Driver->Size){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_TX_DATA_SET;
	}else{
		SPI_Driver->Register.R15_PC = SPI_Driver->Register.R14_LR;
	}
	return;
SPI_DRIVER_PROC_ENDP:	
	SPI_Driver->Mode = SPI_IDLE;
	SPI_Driver->Register.R15_PC = SPI_DRIVER_PROC_READY;
	return;
}







