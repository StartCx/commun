#include "stm32f10x.h"
#include "timer.h"
#include "usart.h"
#include "string.h"
#include "shell_task.h"
#include "I2C_Config.h"
#include "SPI_Config.h"
#include "core.h"



int main(void)
{	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 //��ADCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	 //��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	 //��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	Usart1_Init(115200);//PA9,PA10
	Led_Device.Init(&Led_Device);
	I2Cx_Init(I2C1_M_HARDWARE);
	I2Cx_Init(I2C2_M_BITBANG);
	I2Cx_Init(I2C3_M_SOFTWARE);
	I2Cx_Init(I2C4_S_BITBANG);
	
	SPIx_Init(SPI1_M_HARDWARE);
	SPIx_Init(SPI2_M_BITBANG);
	SPIx_Init(SPI3_M_SOFTWARE);
	
	TIM4_Configuration();
	Shell_Device.Init(&Shell_Device);
	
	
	while(1)
	{
		Start_CPU();
		
		if( Scheduler(&Led_Device.Timer) == 1)//100usִ��һ��
		{
			Led_Device.Driver(&Led_Device);
		}
		if( Scheduler(&Shell_Device.Timer) == 1)//100usִ��һ��
		{
			Shell_Device.Driver(&Shell_Device);
			Shell_Device.Put(&Shell_Device);
		}
		
		Stop_CPU(10);
	}
}


void USART1_IRQHandler(void)
{
	Shell_Device.Get(&Shell_Device);
}



void TIM4_IRQHandler(void)
{	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	Timer_IncTick();//10usһ��
	I2Cx_Peripheral(I2C2_M_BITBANG);
	I2Cx_Peripheral(I2C4_S_BITBANG);
	SPIx_Peripheral(SPI2_M_BITBANG);
}

