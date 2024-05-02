#include "can.h"


CAN_Struct_t CAN1_BUS = {
	.CANx	= CAN1,
};



void CAN1_Configuration(void){ //CAN��ʼ��������0��ʾ���óɹ�������������ʾʧ�ܣ�
    GPIO_InitTypeDef        GPIO_InitStructure; 
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;	 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PORTAʱ��                                                                
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);    //ʹ��CAN1ʱ��  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��IO
    //CAN��Ԫ����
    CAN_InitStructure.CAN_TTCM=DISABLE;         //��ʱ�䴥��ͨ��ģʽ  
    CAN_InitStructure.CAN_ABOM=DISABLE;         //����Զ����߹���   
    CAN_InitStructure.CAN_AWUM=DISABLE;         //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN_InitStructure.CAN_NART=ENABLE;          //��ֹ�����Զ����� 
    CAN_InitStructure.CAN_RFLM=DISABLE;         //���Ĳ�����,�µĸ��Ǿɵ�  
    CAN_InitStructure.CAN_TXFP=DISABLE;         //���ȼ��ɱ��ı�ʶ������ 
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;  //ģʽ���ã�CAN_Mode_Normal ��ͨģʽ��CAN_Mode_LoopBack �ػ�ģʽ; 
    //���ò�����
	if (CAN_SPEED == CAN_250Kbits) {
		CAN_InitStructure.CAN_Prescaler= 9;        //��Ƶϵ��(Fdiv)Ϊbrp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_8tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}else if (CAN_SPEED == CAN_10Kbits) {
		CAN_InitStructure.CAN_Prescaler= 144;        //��Ƶϵ��(Fdiv)Ϊbrp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;
	}else if (CAN_SPEED == CAN_100Kbits) {
		CAN_InitStructure.CAN_Prescaler= 15;        //��Ƶϵ��(Fdiv)Ϊbrp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}else if (CAN_SPEED == CAN_1Mbits) {
		CAN_InitStructure.CAN_Prescaler= 2;        //��Ƶϵ��(Fdiv)Ϊbrp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_10tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
    CAN_Init(CAN1, &CAN_InitStructure);         //��ʼ��CAN1 
	//���ù�����
    CAN_FilterInitStructure.CAN_FilterNumber=0; //������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   //����λģʽ
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //32λ�� 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;    //32λID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0
    CAN_FilterInit(&CAN_FilterInitStructure);           //�˲�����ʼ��

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);              //FIFO0��Ϣ�Һ��ж�����.            
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


uint8_t CAN_Standard_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len)
{
	CAN_Struct->Tx_Message.DLC	= Len; // Ҫ���͵����ݳ���
	CAN_Struct->Tx_Message.StdId= CAN_ID;
	CAN_Struct->Tx_Message.IDE	= CAN_ID_STD; 	//��׼֡
	CAN_Struct->Tx_Message.RTR	= CAN_RTR_DATA;	//����֡
	
    for(CAN_Struct->index=0;CAN_Struct->index<Len;CAN_Struct->index++){
		CAN_Struct->Tx_Message.Data[CAN_Struct->index]=Msg[CAN_Struct->index];       //д������  
	}        
    CAN_Struct->mbox = CAN_Transmit( CAN_Struct->CANx,&CAN_Struct->Tx_Message);   
    CAN_Struct->index = 0;
    while((CAN_TransmitStatus( CAN_Struct->CANx,CAN_Struct->mbox)==CAN_TxStatus_Failed)&&(CAN_Struct->index<0XFFF)){
		CAN_Struct->index++; //�ȴ����ͽ���
		if(CAN_Struct->index>=0XFFF){
			return 1;
		}
	}
    return 0;    
}


uint8_t CAN_Extended_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len)
{
	CAN_Struct->Tx_Message.DLC	= Len; // Ҫ���͵����ݳ���
	CAN_Struct->Tx_Message.ExtId= CAN_ID;		//��չID
	CAN_Struct->Tx_Message.IDE	= CAN_ID_EXT; 	//��׼֡
	CAN_Struct->Tx_Message.RTR	= CAN_RTR_DATA;	//����֡
	
    for(CAN_Struct->index=0;CAN_Struct->index<Len;CAN_Struct->index++){
		CAN_Struct->Tx_Message.Data[CAN_Struct->index]=Msg[CAN_Struct->index];       //д������  
	}        
    CAN_Struct->mbox = CAN_Transmit(CAN_Struct->CANx,&CAN_Struct->Tx_Message);   
    CAN_Struct->index = 0;
    while((CAN_TransmitStatus(CAN_Struct->CANx,CAN_Struct->mbox)==CAN_TxStatus_Failed)&&(CAN_Struct->index<0XFFF)){
		CAN_Struct->index++; //�ȴ����ͽ���
		if(CAN_Struct->index>=0XFFF){
			return 1;
		}
	}
    return 0;    
}


void CAN_Recv_To_UART_Send(void)
{
	if( CAN1_BUS.RX_Flag == 1){
		CAN1_BUS.RX_Flag = 0;
		if(CAN1_BUS.RX_Message.IDE == CAN_ID_STD){
			printf("CAN Recv: StdId=0x%x,",CAN1_BUS.RX_Message.StdId);
		}else{
			printf("CAN Recv: ExtId=0x%x,",CAN1_BUS.RX_Message.ExtId);
		}
		printf("RTR=%d,",CAN1_BUS.RX_Message.RTR);
		printf("DLC=%d,Data:",CAN1_BUS.RX_Message.DLC);
		
		for(int i = 0; i<8;i++){
			printf("0x%x ",CAN1_BUS.RX_Message.Data[i]);
		}
		printf("\r\n");
	}
}

