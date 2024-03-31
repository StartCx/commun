#include "adc.h"


void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                         //ADC123_IN2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;                         //ADC123_IN2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                      //ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1���� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 //����ADC�Ĺ���ģʽ���˴�����Ϊ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                      //��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                //ADC�����ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;                            //2��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);

	/******************************************************************
	����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ʹ��ADC1,ģ��ͨ��2���������к�Ϊ1������ʱ��Ϊ55.5����
	*******************************************************************/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);

	/******************************************************************
	���ü��ģʽ(�������������)
	*******************************************************************/
	ADC_DiscModeChannelCountConfig(ADC1, 1);
	ADC_DiscModeCmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);                                             //ʹ��ADC

	ADC_ResetCalibration(ADC1);                                        //У�鸴λ
	while(ADC_GetResetCalibrationStatus(ADC1));                        //�ȴ���λ���

	ADC_StartCalibration(ADC1);                                        //��ʼADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));                             //�ȴ�У�����
}

