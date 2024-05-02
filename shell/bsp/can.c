#include "can.h"


CAN_Struct_t CAN1_BUS = {
	.CANx	= CAN1,
};



void CAN1_Configuration(void){ //CAN初始化（返回0表示设置成功，返回其他表示失败）
    GPIO_InitTypeDef        GPIO_InitStructure; 
    CAN_InitTypeDef         CAN_InitStructure;
    CAN_FilterInitTypeDef   CAN_FilterInitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;	 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟                                                                
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);    //使能CAN1时钟  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM=DISABLE;         //非时间触发通信模式  
    CAN_InitStructure.CAN_ABOM=DISABLE;         //软件自动离线管理   
    CAN_InitStructure.CAN_AWUM=DISABLE;         //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART=ENABLE;          //禁止报文自动传送 
    CAN_InitStructure.CAN_RFLM=DISABLE;         //报文不锁定,新的覆盖旧的  
    CAN_InitStructure.CAN_TXFP=DISABLE;         //优先级由报文标识符决定 
    CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;  //模式设置：CAN_Mode_Normal 普通模式，CAN_Mode_LoopBack 回环模式; 
    //设置波特率
	if (CAN_SPEED == CAN_250Kbits) {
		CAN_InitStructure.CAN_Prescaler= 9;        //分频系数(Fdiv)为brp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_8tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}else if (CAN_SPEED == CAN_10Kbits) {
		CAN_InitStructure.CAN_Prescaler= 144;        //分频系数(Fdiv)为brp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;
	}else if (CAN_SPEED == CAN_100Kbits) {
		CAN_InitStructure.CAN_Prescaler= 15;        //分频系数(Fdiv)为brp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}else if (CAN_SPEED == CAN_1Mbits) {
		CAN_InitStructure.CAN_Prescaler= 2;        //分频系数(Fdiv)为brp+1  
		CAN_InitStructure.CAN_BS1=CAN_BS1_10tq; 
		CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	}
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
    CAN_Init(CAN1, &CAN_InitStructure);         //初始化CAN1 
	//设置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber=0; //过滤器0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;   //屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;  //32位宽 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;    //32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0
    CAN_FilterInit(&CAN_FilterInitStructure);           //滤波器初始化

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);              //FIFO0消息挂号中断允许.            
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


uint8_t CAN_Standard_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len)
{
	CAN_Struct->Tx_Message.DLC	= Len; // 要发送的数据长度
	CAN_Struct->Tx_Message.StdId= CAN_ID;
	CAN_Struct->Tx_Message.IDE	= CAN_ID_STD; 	//标准帧
	CAN_Struct->Tx_Message.RTR	= CAN_RTR_DATA;	//数据帧
	
    for(CAN_Struct->index=0;CAN_Struct->index<Len;CAN_Struct->index++){
		CAN_Struct->Tx_Message.Data[CAN_Struct->index]=Msg[CAN_Struct->index];       //写入数据  
	}        
    CAN_Struct->mbox = CAN_Transmit( CAN_Struct->CANx,&CAN_Struct->Tx_Message);   
    CAN_Struct->index = 0;
    while((CAN_TransmitStatus( CAN_Struct->CANx,CAN_Struct->mbox)==CAN_TxStatus_Failed)&&(CAN_Struct->index<0XFFF)){
		CAN_Struct->index++; //等待发送结束
		if(CAN_Struct->index>=0XFFF){
			return 1;
		}
	}
    return 0;    
}


uint8_t CAN_Extended_Send_Msg(CAN_Struct_t *CAN_Struct, uint32_t CAN_ID, uint8_t *Msg,uint8_t Len)
{
	CAN_Struct->Tx_Message.DLC	= Len; // 要发送的数据长度
	CAN_Struct->Tx_Message.ExtId= CAN_ID;		//拓展ID
	CAN_Struct->Tx_Message.IDE	= CAN_ID_EXT; 	//标准帧
	CAN_Struct->Tx_Message.RTR	= CAN_RTR_DATA;	//数据帧
	
    for(CAN_Struct->index=0;CAN_Struct->index<Len;CAN_Struct->index++){
		CAN_Struct->Tx_Message.Data[CAN_Struct->index]=Msg[CAN_Struct->index];       //写入数据  
	}        
    CAN_Struct->mbox = CAN_Transmit(CAN_Struct->CANx,&CAN_Struct->Tx_Message);   
    CAN_Struct->index = 0;
    while((CAN_TransmitStatus(CAN_Struct->CANx,CAN_Struct->mbox)==CAN_TxStatus_Failed)&&(CAN_Struct->index<0XFFF)){
		CAN_Struct->index++; //等待发送结束
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

