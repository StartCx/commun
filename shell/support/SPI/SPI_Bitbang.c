#include "SPI_Bitbang.h"


enum
{
	SPI_IDLE = 0,
	SPI_WRITE_AND_READ,
	SPI_WRITE_THEN_READ,
};

enum
{
	SPI_DRIVER_PROC_READY = 0,
	SPI_DRIVER_MODE_SELECT,
	SPI_DRIVER_WRITE_THEN_READ0,
	SPI_DRIVER_WRITE_THEN_READ1,
	SPI_DRIVER_MODE_PARAM_INIT,
	SPI_DRIVER_TX_DATA_SET,
	SPI_DRIVER_LOOP_TX_DATA_MODE0,
	SPI_DRIVER_LOOP_RX_DATA_MODE0,
	SPI_DRIVER_LOOP_TX_DATA_MODE1,
	SPI_DRIVER_LOOP_RX_DATA_MODE1,
	SPI_DRIVER_LOOP_TX_DATA_MODE2,
	SPI_DRIVER_LOOP_RX_DATA_MODE2,
	SPI_DRIVER_LOOP_TX_DATA_MODE3,
	SPI_DRIVER_LOOP_RX_DATA_MODE3,
	SPI_DRIVER_MASK_SHIFT,
	SPI_DRIVER_RX_DATA_GET,
	SPI_DRIVER_SIZE_COMPARE,
	SPI_DRIVER_PROC_ENDP,
	SPI_DRIVER_SUM,
};



uint8_t SPI_M_Bitbang_Open(SPI_M_Bitbang_t *SPI_Driver, uint8_t CSx)
{
	if( SPI_Driver->Register.R12_Lock == CORE_UNLOCK && SPI_Driver->Register.R15_PC == SPI_DRIVER_PROC_READY && CSx < SPI_Driver->CS_SUM){
		SPI_Driver->Register.R12_Lock =  CORE_LOCK;
		GPIO_SET_LOW(SPI_Driver->CS_Pins[CSx].GPIO_Port, SPI_Driver->CS_Pins[CSx].Pin);
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void SPI_M_Bitbang_Close(SPI_M_Bitbang_t *SPI_Driver, uint8_t CSx)
{
	SPI_Driver->Register.R12_Lock = CORE_UNLOCK;
	if( CSx < SPI_Driver->CS_SUM){
		GPIO_SET_HIGH(SPI_Driver->CS_Pins[CSx].GPIO_Port, SPI_Driver->CS_Pins[CSx].Pin);
	}
}

void SPI_M_Bitbang_WriteAndRead(SPI_M_Bitbang_t *SPI_Driver, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	SPI_Driver->pTx_data = pTxData;
	SPI_Driver->pRx_data = pRxData;
	SPI_Driver->Size     = Size;
	SPI_Driver->WR_Mode = SPI_WRITE_AND_READ;
}

void SPI_M_Bitbang_WriteThenRead(SPI_M_Bitbang_t *SPI_Driver, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize)
{
	SPI_Driver->pTx_data = pTxData;
	SPI_Driver->TxSize   = TxSize;
	SPI_Driver->pRx_data = pRxData;
	SPI_Driver->RxSize   = RxSize;
	SPI_Driver->WR_Mode = SPI_WRITE_THEN_READ;
}


uint8_t SPI_M_Bitbang_Endp(SPI_M_Bitbang_t *SPI_Driver)
{
	if( SPI_Driver->Register.R15_PC == SPI_DRIVER_PROC_READY && SPI_Driver->WR_Mode == SPI_IDLE){
		return CORE_DONE;
	}else{
		return CORE_RUNNING;
	}
}

// 切换SPI模式
void SPI_M_Bitbang_Mode(SPI_M_Bitbang_t *SPI_Driver,spi_mode_e spi_mode) {
	SPI_Driver->SPI_Mode = spi_mode;
    switch(SPI_Driver->SPI_Mode){
		case SPI_MODE_0:
			SPI_Driver->Func_Index = SPI_DRIVER_LOOP_TX_DATA_MODE0;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_1:
			SPI_Driver->Func_Index = SPI_DRIVER_LOOP_TX_DATA_MODE1;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_2:
			SPI_Driver->Func_Index = SPI_DRIVER_LOOP_TX_DATA_MODE2;
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		case SPI_MODE_3:
			SPI_Driver->Func_Index = SPI_DRIVER_LOOP_TX_DATA_MODE3;
			GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
		default:
			SPI_Driver->Func_Index = SPI_DRIVER_LOOP_TX_DATA_MODE0;
			GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
			break;
	}
}


void SPI_M_Bitbang_Config(SPI_M_Bitbang_t *SPI_Driver)
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

	GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
	SPI_M_Bitbang_Mode(SPI_Driver,SPI_Driver->SPI_Mode);
}


void SPI_M_Bitbang_Peripheral(SPI_M_Bitbang_t *SPI_Driver)
{
	static const void *function[SPI_DRIVER_SUM] = {
		[SPI_DRIVER_PROC_READY] 	= &&SPI_DRIVER_PROC_READY,
		[SPI_DRIVER_MODE_SELECT] 	= &&SPI_DRIVER_MODE_SELECT,
		
		[SPI_DRIVER_WRITE_THEN_READ0] 	= &&SPI_DRIVER_WRITE_THEN_READ0,
		[SPI_DRIVER_WRITE_THEN_READ1] 	= &&SPI_DRIVER_WRITE_THEN_READ1,
		
		[SPI_DRIVER_MODE_PARAM_INIT] 	= &&SPI_DRIVER_MODE_PARAM_INIT,
		[SPI_DRIVER_TX_DATA_SET]		= &&SPI_DRIVER_TX_DATA_SET,
		
		[SPI_DRIVER_LOOP_TX_DATA_MODE0] = &&SPI_DRIVER_LOOP_TX_DATA_MODE0,
		[SPI_DRIVER_LOOP_RX_DATA_MODE0]	= &&SPI_DRIVER_LOOP_RX_DATA_MODE0,
		
		[SPI_DRIVER_LOOP_TX_DATA_MODE1] = &&SPI_DRIVER_LOOP_TX_DATA_MODE1,
		[SPI_DRIVER_LOOP_RX_DATA_MODE1]	= &&SPI_DRIVER_LOOP_RX_DATA_MODE1,
		
		[SPI_DRIVER_LOOP_TX_DATA_MODE2] = &&SPI_DRIVER_LOOP_TX_DATA_MODE2,
		[SPI_DRIVER_LOOP_RX_DATA_MODE2]	= &&SPI_DRIVER_LOOP_RX_DATA_MODE2,
		
		[SPI_DRIVER_LOOP_TX_DATA_MODE3] = &&SPI_DRIVER_LOOP_TX_DATA_MODE3,
		[SPI_DRIVER_LOOP_RX_DATA_MODE3]	= &&SPI_DRIVER_LOOP_RX_DATA_MODE3,
		
		
		[SPI_DRIVER_MASK_SHIFT]		= &&SPI_DRIVER_MASK_SHIFT,
		[SPI_DRIVER_RX_DATA_GET]	= &&SPI_DRIVER_RX_DATA_GET,
		[SPI_DRIVER_SIZE_COMPARE]	= &&SPI_DRIVER_SIZE_COMPARE,
		[SPI_DRIVER_PROC_ENDP]		= &&SPI_DRIVER_PROC_ENDP,
	};
	
	goto *function[SPI_Driver->Register.R15_PC];
	
SPI_DRIVER_PROC_READY:
	if( SPI_Driver->WR_Mode != SPI_IDLE){
		SPI_Driver->Register.R15_PC = SPI_DRIVER_MODE_SELECT;
	}
	return;
SPI_DRIVER_MODE_SELECT:
	if( SPI_Driver->WR_Mode == SPI_WRITE_AND_READ){
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
	if( SPI_Driver->First_Bit == LSB_FIRST){
		SPI_Driver->Register.R10_Mask = 0x01;
	}else{
		SPI_Driver->Register.R10_Mask = 0x80;
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_TX_DATA_SET;
	return;
SPI_DRIVER_TX_DATA_SET:
	if( SPI_Driver->pTx_data != NULL && SPI_Driver->Register.R1_Index < SPI_Driver->Size){
		SPI_Driver->Register.R3_cout = SPI_Driver->pTx_data[SPI_Driver->Register.R1_Index];
	}else{
		SPI_Driver->Register.R3_cout = 0xFF;
	}
	SPI_Driver->Register.R15_PC = SPI_Driver->Func_Index;
	return;
/********************************************************************************************************************/
SPI_DRIVER_LOOP_TX_DATA_MODE0:
	if( SPI_Driver->Register.R3_cout & SPI_Driver->Register.R10_Mask){ //发送数据
		GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}
	GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_RX_DATA_MODE0;
	return;
SPI_DRIVER_LOOP_RX_DATA_MODE0:
	if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO) == 1){//读取状态
		SPI_Driver->Register.R2_cin |=  SPI_Driver->Register.R10_Mask;  // 将特定位设置为高
	}else{
		SPI_Driver->Register.R2_cin &= ~SPI_Driver->Register.R10_Mask;  // 将特定位设置为低
	}
	GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	SPI_Driver->Register.R15_PC = SPI_DRIVER_MASK_SHIFT;
	return;
/********************************************************************************************************************/
SPI_DRIVER_LOOP_TX_DATA_MODE1:
	GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	if( SPI_Driver->Register.R3_cout & SPI_Driver->Register.R10_Mask){ //发送数据
		GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_RX_DATA_MODE1;
	return;
SPI_DRIVER_LOOP_RX_DATA_MODE1:
	GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿 
	if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO) == 1){//读取状态
		SPI_Driver->Register.R2_cin |=  SPI_Driver->Register.R10_Mask;  // 将特定位设置为高
	}else{
		SPI_Driver->Register.R2_cin &= ~SPI_Driver->Register.R10_Mask;  // 将特定位设置为低
	}  
	SPI_Driver->Register.R15_PC = SPI_DRIVER_MASK_SHIFT;
	return;
/********************************************************************************************************************/
SPI_DRIVER_LOOP_TX_DATA_MODE2:
	if( SPI_Driver->Register.R3_cout & SPI_Driver->Register.R10_Mask){ //发送数据
		GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}
	GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_RX_DATA_MODE2;
	return;
SPI_DRIVER_LOOP_RX_DATA_MODE2:
	if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO) == 1){//读取状态
		SPI_Driver->Register.R2_cin |=  SPI_Driver->Register.R10_Mask;  // 将特定位设置为高
	}else{
		SPI_Driver->Register.R2_cin &= ~SPI_Driver->Register.R10_Mask;  // 将特定位设置为低
	}
	GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	SPI_Driver->Register.R15_PC = SPI_DRIVER_MASK_SHIFT;
	return;
	
/********************************************************************************************************************/
SPI_DRIVER_LOOP_TX_DATA_MODE3:
	GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	if( SPI_Driver->Register.R3_cout & SPI_Driver->Register.R10_Mask){ //发送数据
		GPIO_SET_HIGH(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_MOSI, SPI_Driver->PIN_MOSI);
	}
	SPI_Driver->Register.R15_PC = SPI_DRIVER_LOOP_RX_DATA_MODE3;
	return;
SPI_DRIVER_LOOP_RX_DATA_MODE3:
	GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿 
	if( GPIO_GET_STATE(SPI_Driver->PORT_MISO, SPI_Driver->PIN_MISO) == 1){//读取状态
		SPI_Driver->Register.R2_cin |=  SPI_Driver->Register.R10_Mask;  // 将特定位设置为高
	}else{
		SPI_Driver->Register.R2_cin &= ~SPI_Driver->Register.R10_Mask;  // 将特定位设置为低
	}  
	SPI_Driver->Register.R15_PC = SPI_DRIVER_MASK_SHIFT;
	return;
/********************************************************************************************************************/
SPI_DRIVER_MASK_SHIFT:
	if( SPI_Driver->First_Bit == MSB_FIRST){
		SPI_Driver->Register.R10_Mask >>= 1;
		if( SPI_Driver->Register.R10_Mask > 0){
			SPI_Driver->Register.R15_PC = SPI_Driver->Func_Index;
		}else{
			SPI_Driver->Register.R15_PC = SPI_DRIVER_RX_DATA_GET;
		}
	}else{
		SPI_Driver->Register.R10_Mask <<= 1;
		if( SPI_Driver->Register.R10_Mask <= 0x80){
			SPI_Driver->Register.R15_PC = SPI_Driver->Func_Index;
		}else{
			SPI_Driver->Register.R15_PC = SPI_DRIVER_RX_DATA_GET;
		}
	}
	return;
SPI_DRIVER_RX_DATA_GET:
	if( SPI_Driver->pRx_data != NULL && SPI_Driver->Register.R1_Index < SPI_Driver->Size){
		SPI_Driver->pRx_data[SPI_Driver->Register.R1_Index] = SPI_Driver->Register.R2_cin;
	}else{
		SPI_Driver->Register.R2_cin = 0xFF;
	}
	if( SPI_Driver->First_Bit == LSB_FIRST){
		SPI_Driver->Register.R10_Mask = 0x01;
	}else{
		SPI_Driver->Register.R10_Mask = 0x80;
	}
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
	SPI_Driver->WR_Mode = SPI_IDLE;
	SPI_Driver->Register.R15_PC = SPI_DRIVER_PROC_READY;
	return;
}







