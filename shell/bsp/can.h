#ifndef __CAN_H
#define __CAN_H

#include "core.h"



//����ģʽ�Ͳ�����
//������=(pclk1/((1+8+7)*9)) = 36Mhz/16/9 = 250Kbits�趨��һ��ʱ�䵥λ�ĳ���9
#define tsjw	CAN_SJW_1tq	//������Ŀ��1~4��
#define tbs1	CAN_BS1_8tq	//������Ŀ��1~16��
#define tbs2	CAN_BS2_7tq	//������Ŀ��1~8��
#define brp		9	//������Ŀ


typedef struct CAN_Struct_t
{
	CAN_TypeDef* CANx;
	uint8_t mbox;
	uint32_t index;
	CanTxMsg TxMessage;
	CanRxMsg RX_Message;
	uint8_t RX_Flag;
}CAN_Struct_t;


extern CAN_Struct_t CAN1_BUS;
extern uint8_t CAN_Standard_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len);
extern uint8_t CAN_Extended_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len);
extern void CAN1_Configuration(void);



#endif
