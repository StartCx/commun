#include "stm32f10x.h"
#include "timer.h"
#include "usart.h"
#include "string.h"
#include "shell_task.h"
#include "I2C_Config.h"
#include "simulated_pwm.h"
#include "simulated_uart.h"
#include "adc.h"
#include "dht11.h"
#include "core.h"
#include "w25qx.h"
#include "encoder_input.h"
#include "IR_Receiver.h"
#include "key_input.h"
#include "servo_output.h"
#include "hc_sr04.h"
#include "DS18B20.h"
#include "I2C_Config.h"




int main(void)
{	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 //��ADCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	 //��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	
	Usart1_Init(115200);//PA9,PA10
	
	I2Cx_Init(I2C1_M_HARDWARE);
	I2Cx_Init(I2C2_M_BITBANG);
	I2Cx_Init(I2C3_M_SOFTWARE);
	I2Cx_Init(I2C4_S_BITBANG);
/*	
	for(int i =0; i < 255; i++){
		if(I2C1_Master.Detect(&I2C1_Master,i) == NO_ERROR){
			printf("0x%2x ��ַ����\r\n",i);
		}else{
			printf("0x%2x ��ַ������\r\n",i);
		}
		for(int j = 0; j< 100000;j++);
	}
	uint8_t txbuf[8];
	uint8_t rxbuf[8];
	I2C1_Master.Get(&I2C1_Master, 0xa0, 0x00,8, rxbuf, 8);
	for(int i = 0; i< 8;i++){
		printf(" 0x%x",rxbuf[i]);
	}
	printf("�����\r\n");
	
	for(int i = 0; i< 8;i++){
		txbuf[i] = rxbuf[i]+1;
	}
	I2C1_Master.Set(&I2C1_Master, 0xa0, 0x00,8, txbuf, 8);
	for(int i = 0; i< 8;i++){
		printf(" 0x%x",txbuf[i]);
	}
	printf("д���\r\n");
*/	
	TIM4_Configuration();
	Shell_Device.Init(&Shell_Device);
	Led_Device.Init(&Led_Device);
	//****************************************************************************/
	
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
	servo_output_angle_task(&Servo_Output);//���ֶ��
	PWM_Out_Put(&Simulated_PWM);
	Timer_IncTick();//10usһ��
	I2Cx_Peripheral(I2C2_M_BITBANG);
	I2Cx_Peripheral(I2C4_S_BITBANG);
}

