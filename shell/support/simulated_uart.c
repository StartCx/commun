#include "simulated_uart.h"


SIM_UART_Driver_t SIM_UART = {
	.PORT_TX  = GPIOA,
	.PIN_TX   = GPIO_Pin_2,
	.PORT_RX  = GPIOA,
	.PIN_RX   = GPIO_Pin_3,
	.Tx_Register = {
		.R4_Status = CORE_IDLE,
	},
	.Timer = {
		.TickPeroid = 10,
	},
};


void SIM_Uart_Init(SIM_UART_Driver_t *SIM_UART_Driver) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SIM_UART_Driver->PIN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SIM_UART_Driver->PORT_TX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SIM_UART_Driver->PIN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SIM_UART_Driver->PORT_RX, &GPIO_InitStructure);
}



uint8_t SIM_UART_GetFlagStatus(SIM_UART_Driver_t *SIM_UART_Driver, Sim_Uart_State State)
{
	if( State == SIM_USART_TX_STATE){
		return SIM_UART_Driver->Tx_Register.R4_Status;
	}
	return SIM_UART_Driver->Rx_Register.R4_Status;
}

void SIM_UART_SetFlagStatus(SIM_UART_Driver_t *SIM_UART_Driver, Sim_Uart_State State, uint8_t value)
{
	if( State == SIM_USART_TX_STATE){
		SIM_UART_Driver->Tx_Register.R4_Status = value;
	}else{
		SIM_UART_Driver->Rx_Register.R4_Status = value;
	}
}

void SIM_USART_SendData(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t ch)
{
	SIM_UART_Driver->Tx_Register.R2_cin = ch;
	SIM_UART_Driver->Tx_pDat = &SIM_UART_Driver->Tx_Register.R2_cin;
	SIM_UART_Driver->Tx_num  = 1;
	SIM_UART_SetFlagStatus(SIM_UART_Driver,SIM_USART_TX_STATE, CORE_BUSY);
}

void SIM_Uart_SendByte( SIM_UART_Driver_t *SIM_UART_Driver, uint8_t ch)
{
	while( SIM_UART_GetFlagStatus( SIM_UART_Driver, SIM_USART_TX_STATE) == CORE_BUSY);
	SIM_USART_SendData( SIM_UART_Driver, ch);
}

void SIM_Uart_SendArray(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t *pDat, uint16_t num)
{
	while( SIM_UART_GetFlagStatus( SIM_UART_Driver, SIM_USART_TX_STATE) == CORE_BUSY);
	SIM_UART_Driver->Tx_pDat = pDat;
	SIM_UART_Driver->Tx_num  = num;
	SIM_UART_SetFlagStatus(SIM_UART_Driver,SIM_USART_TX_STATE, CORE_BUSY);
}

void SIM_Uart_Tx_Peripheral(SIM_UART_Driver_t *SIM_UART_Driver)		//模拟串口发送
{
	enum
	{
		SIM_UART_TX_GROUP0,
		SIM_UART_TX_GROUP1,
		SIM_UART_TX_GROUP2_START_BIT,
		SIM_UART_TX_GROUP3_START_BIT,
		SIM_UART_TX_GROUP4,
		SIM_UART_TX_GROUP5,
		SIM_UART_TX_SUM
	};
	static const void *function[SIM_UART_TX_SUM] = {
		[SIM_UART_TX_GROUP0] 			= &&SIM_UART_TX_GROUP0,
		[SIM_UART_TX_GROUP1] 			= &&SIM_UART_TX_GROUP1,
		[SIM_UART_TX_GROUP2_START_BIT] = &&SIM_UART_TX_GROUP2_START_BIT,
		[SIM_UART_TX_GROUP3_START_BIT] = &&SIM_UART_TX_GROUP3_START_BIT,
		[SIM_UART_TX_GROUP4] 			= &&SIM_UART_TX_GROUP4,
		[SIM_UART_TX_GROUP5] 			= &&SIM_UART_TX_GROUP5,
	};
	goto *function[SIM_UART_Driver->Tx_Register.R15_PC];

SIM_UART_TX_GROUP0:
	if( SIM_UART_Driver->Tx_Register.R4_Status == CORE_BUSY){
		SIM_UART_Driver->Tx_Register.R1_Index = 0;
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP1;
	}
	return;
SIM_UART_TX_GROUP1:
	if(SIM_UART_Driver->Tx_pDat == NULL || SIM_UART_Driver->Tx_num == 0){
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP0;
		SIM_UART_Driver->Tx_Register.R4_Status = CORE_IDLE;
	}else{
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP2_START_BIT;
	}
	return;
SIM_UART_TX_GROUP2_START_BIT:
	GPIO_SET_HIGH(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);//空闲---高电平
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP3_START_BIT;	
	return;
SIM_UART_TX_GROUP3_START_BIT:
	GPIO_SET_LOW(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);//起始位---低电平
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP4;
	SIM_UART_Driver->Tx_Register.R10_Mask = 0x01;
	return;
SIM_UART_TX_GROUP4:	
	if( SIM_UART_Driver->Tx_Register.R10_Mask < 0x100){
		if( SIM_UART_Driver->Tx_pDat[SIM_UART_Driver->Tx_Register.R1_Index] & SIM_UART_Driver->Tx_Register.R10_Mask){
			GPIO_SET_HIGH(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);
		}else{
			GPIO_SET_LOW(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);
		}
		SIM_UART_Driver->Tx_Register.R10_Mask <<= 1;
	}else{
		GPIO_SET_HIGH(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP5;
	}
	return;
SIM_UART_TX_GROUP5:
	SIM_UART_Driver->Tx_Register.R1_Index++;
	if( SIM_UART_Driver->Tx_Register.R1_Index < SIM_UART_Driver->Tx_num){
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP2_START_BIT;
	}else{
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP0;
		SIM_UART_Driver->Tx_Register.R4_Status = CORE_IDLE;
	}
	return;
}


uint8_t SIM_Uart_Rx_Peripheral(SIM_UART_Driver_t *SIM_UART_Driver)		//模拟串口接收
{
	enum
	{
		SIM_UART_RX_INIT,
		SIM_UART_RX_GROUP0,
		SIM_UART_RX_GROUP1,
		SIM_UART_RX_GROUP2,
		SIM_UART_RX_SUM
	};
	static const void *function[SIM_UART_RX_SUM] = {
		[SIM_UART_RX_INIT] 			= &&SIM_UART_RX_INIT,
		[SIM_UART_RX_GROUP0] 		= &&SIM_UART_RX_GROUP0,
		[SIM_UART_RX_GROUP1] 		= &&SIM_UART_RX_GROUP1,
		[SIM_UART_RX_GROUP2] 		= &&SIM_UART_RX_GROUP2,
	};
	goto *function[SIM_UART_Driver->Rx_Register.R15_PC];

SIM_UART_RX_INIT:
	if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 1){
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP0;
	}
	return 0;
SIM_UART_RX_GROUP0:
	if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 0){//起始位
		SIM_UART_Driver->Rx_Register.R10_Mask = 0x01;
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP1;
	}
	return 0;
SIM_UART_RX_GROUP1:																//数据位
	if( SIM_UART_Driver->Rx_Register.R10_Mask < 0x100){
		if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 1){
			SIM_UART_Driver->Rx_Register.R2_cin |= SIM_UART_Driver->Rx_Register.R10_Mask;
		}else{
			SIM_UART_Driver->Rx_Register.R2_cin &= ~SIM_UART_Driver->Rx_Register.R10_Mask;
		}
		SIM_UART_Driver->Rx_Register.R10_Mask <<= 1;
	}else{
		goto SIM_UART_RX_GROUP2;
	}
	return 0;
SIM_UART_RX_GROUP2:
	if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 1){//停止位
		SIM_UART_Driver->Rx_Data = SIM_UART_Driver->Rx_Register.R2_cin;
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP0;
		return 1;
	}else{
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP0;
	}
	return 0;
}



