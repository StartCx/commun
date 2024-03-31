#include "adc.h"


void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                         //ADC123_IN2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;                         //ADC123_IN2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1配置 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 //设置ADC的工作模式，此处设置为独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                      //单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                //ADC工作在单次转化模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;                            //2个通道
	ADC_Init(ADC1, &ADC_InitStructure);

	/******************************************************************
	设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	使用ADC1,模拟通道2，采样序列号为1，采样时间为55.5周期
	*******************************************************************/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);

	/******************************************************************
	设置间断模式(规则组短序列数)
	*******************************************************************/
	ADC_DiscModeChannelCountConfig(ADC1, 1);
	ADC_DiscModeCmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);                                             //使能ADC

	ADC_ResetCalibration(ADC1);                                        //校验复位
	while(ADC_GetResetCalibrationStatus(ADC1));                        //等待复位完成

	ADC_StartCalibration(ADC1);                                        //开始ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));                             //等待校验完成
}

