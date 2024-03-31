#include "simulated_uart.h"


SIM_UART_Driver_t SIM_UART_Driver = {
	.PORT_TX  = GPIOA,
	.PIN_TX   = GPIO_Pin_9,
	.PORT_RX  = GPIOA,
	.PIN_RX   = GPIO_Pin_10,
	.Timer = {
		.TickPeroid = 10,
	},
};
/*
	if( SIM_UART_Driver.Tx_Register.R8_Flag == 0 && QueueDataOut(&Shell_Device.Shell_Print_Queue, &SIM_UART_Driver.Tx_Data)){
		SIM_UART_Driver.Tx_Register.R8_Flag = 1;
	}else if( SIM_UART_Driver.Tx_Register.R8_Flag == 1 && Simul_Uart_Tx( &SIM_UART_Driver, &SIM_UART_Driver.Tx_Data, 1)){
		SIM_UART_Driver.Tx_Register.R8_Flag = 0;
	}
*/
uint8_t Simul_Uart_Tx(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t *pDat, uint16_t num)		//模拟串口发送
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
	SIM_UART_Driver->Tx_pDat = pDat;
	SIM_UART_Driver->Tx_num  = num;
	SIM_UART_Driver->Tx_Register.R1_Index = 0;
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP1;
	return 0;
SIM_UART_TX_GROUP1:
	if(SIM_UART_Driver->Tx_pDat == NULL || SIM_UART_Driver->Tx_num == 0){
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP0;
		return 1;
	}
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP2_START_BIT;
	return 0;
SIM_UART_TX_GROUP2_START_BIT:
	GPIO_SET_HIGH(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);//空闲---高电平
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP3_START_BIT;	
	return 0;
SIM_UART_TX_GROUP3_START_BIT:
	GPIO_SET_LOW(SIM_UART_Driver->PORT_TX, SIM_UART_Driver->PIN_TX);//起始位---低电平
	SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP4;
	SIM_UART_Driver->Tx_Register.R10_Mask = 0x01;
	return 0;
SIM_UART_TX_GROUP4:	
	if( SIM_UART_Driver->Tx_Register.R10_Mask != 0x100){
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
	return 0;
SIM_UART_TX_GROUP5:
	SIM_UART_Driver->Tx_Register.R1_Index++;
	if( SIM_UART_Driver->Tx_Register.R1_Index < SIM_UART_Driver->Tx_num){
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP2_START_BIT;
	}else{
		SIM_UART_Driver->Tx_Register.R15_PC = SIM_UART_TX_GROUP0;
		return 1;
	}
	return 0;  
}

uint8_t Simul_Uart_Rx(SIM_UART_Driver_t *SIM_UART_Driver)		//模拟串口发送
{
	enum
	{
		SIM_UART_RX_GROUP0,
		SIM_UART_RX_GROUP1,
		SIM_UART_RX_SUM
	};
	static const void *function[SIM_UART_RX_SUM] = {
		[SIM_UART_RX_GROUP0] 			= &&SIM_UART_RX_GROUP0,
		[SIM_UART_RX_GROUP1] 			= &&SIM_UART_RX_GROUP1,
	};
	goto *function[SIM_UART_Driver->Rx_Register.R15_PC];
	
SIM_UART_RX_GROUP0:
	if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 0){//起始位
		SIM_UART_Driver->Rx_Register.R10_Mask = 0x01;
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP1;
	}
	return 0;
SIM_UART_RX_GROUP1:
	if( SIM_UART_Driver->Rx_Register.R10_Mask != 0x100){
		if( GPIO_GET_STATE(SIM_UART_Driver->PORT_RX, SIM_UART_Driver->PIN_RX) == 1){
			SIM_UART_Driver->Rx_Register.R2_cin |= SIM_UART_Driver->Rx_Register.R10_Mask;
		}else{
			SIM_UART_Driver->Rx_Register.R2_cin &= ~SIM_UART_Driver->Rx_Register.R10_Mask;
		}
		SIM_UART_Driver->Rx_Register.R10_Mask <<= 1;
	}else{
		SIM_UART_Driver->Rx_Register.R15_PC = SIM_UART_RX_GROUP0;
		SIM_UART_Driver->Rx_Data = SIM_UART_Driver->Rx_Register.R2_cin;
		return 1;
	}
	return 0;
}

