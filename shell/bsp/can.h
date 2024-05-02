#ifndef __CAN_H
#define __CAN_H

#include "core.h"

enum{
	CAN_10Kbits = 0,
	CAN_100Kbits,
	CAN_250Kbits,
	CAN_1Mbits,
};
//波特率=(pclk1/((1+8+7)*9)) = 36Mhz/16/9 = 250Kbits设定了一个时间单位的长度9
#define CAN_SPEED CAN_250Kbits


typedef struct CAN_Struct_t
{
	CAN_TypeDef* CANx;
	uint8_t mbox;
	uint32_t index;
	CanTxMsg Tx_Message;
	CanRxMsg RX_Message;
	uint8_t RX_Flag;
}CAN_Struct_t;


extern CAN_Struct_t CAN1_BUS;
extern uint8_t CAN_Standard_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len);
extern uint8_t CAN_Extended_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len);
extern void CAN1_Configuration(void);
extern void CAN_Recv_To_UART_Send(void);


#endif
