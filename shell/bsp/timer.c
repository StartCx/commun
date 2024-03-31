#include "timer.h"


void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(TIM4); 
	
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;     // ����Ԥ��Ƶϵ����72MHz/72 = 1MHz
    TIM_TimeBaseInitStruct.TIM_Period = 10-1; // ���ü��������ޣ����Ϊ32λ�޷������������ֵ �����65ms��
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);					  
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);		
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE); // ������ʱ��			  
}



