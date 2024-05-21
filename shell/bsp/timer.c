#include "timer.h"
#include "stm32f10x_iwdg.h"



//���Ź���ʱʱ����㹫ʽ:Tout=(Ԥ��Ƶֵ*��װ��ֵ)/40 (��λ��ms)
//��ǰpreΪ64��rlrΪ625������õ�Toutʱ��Ϊ1�루���ֵ����

void IWDG_Init(void){ //��ʼ���������Ź�
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
    IWDG_SetPrescaler(IWDG_Prescaler_64); //����IWDGԤ��Ƶֵ
    IWDG_SetReload(625); //����IWDG��װ��ֵ
    IWDG_ReloadCounter(); //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
    IWDG_Enable(); //ʹ��IWDG
}

void IWDG_Feed(void){ //ι������
    IWDG_ReloadCounter();//�̼����ι������
}

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


void TIM3_Configuration(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// ����Ƚ�ͨ��4 GPIO ��ʼ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period=2000-1;	
	TIM_TimeBaseStructure.TIM_Prescaler= 720-1;	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// ����Ƚ�ͨ�� 3
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}


