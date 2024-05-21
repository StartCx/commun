#include "timer.h"
#include "stm32f10x_iwdg.h"



//看门狗定时时间计算公式:Tout=(预分频值*重装载值)/40 (单位：ms)
//当前pre为64，rlr为625，计算得到Tout时间为1秒（大概值）。

void IWDG_Init(void){ //初始化独立看门狗
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对寄存器IWDG_PR和IWDG_RLR的写操作
    IWDG_SetPrescaler(IWDG_Prescaler_64); //设置IWDG预分频值
    IWDG_SetReload(625); //设置IWDG重装载值
    IWDG_ReloadCounter(); //按照IWDG重装载寄存器的值重装载IWDG计数器
    IWDG_Enable(); //使能IWDG
}

void IWDG_Feed(void){ //喂狗程序
    IWDG_ReloadCounter();//固件库的喂狗函数
}

void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(TIM4); 
	
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;     // 设置预分频系数，72MHz/72 = 1MHz
    TIM_TimeBaseInitStruct.TIM_Period = 10-1; // 设置计数器上限，最大为32位无符号整数的最大值 最大是65ms。
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
	TIM_Cmd(TIM4, ENABLE); // 启动计时器			  
}


void TIM3_Configuration(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 输出比较通道4 GPIO 初始化
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
	
	// 输出比较通道 3
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}


