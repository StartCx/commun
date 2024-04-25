#include "stm32f10x.h"
#include "timer.h"
#include "usart.h"
#include "string.h"
#include "shell_task.h"
#include "I2C_Config.h"
#include "SPI_Config.h"
#include "core.h"
#include "oled.h"
#include "can.h"

int flag = 0;


int main(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 	//打开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	 	//打开时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN1_Configuration();				//PA11， PA12
	Usart1_Init(115200);		//PA9 ， PA10
	Usart2_Init(115200);		//PA2 ， PA3
	Led_Device.Init(&Led_Device);//PC13
	I2Cx_Init(I2C1_M_HARDWARE);	//PB6 ， PB7
	I2Cx_Init(I2C2_M_BITBANG);	//PB10， PB11
	I2Cx_Init(I2C3_M_SOFTWARE);	//PB8 ， PB9
	I2Cx_Init(I2C4_S_BITBANG);	//PB4 ， PB5
	
	SPIx_Init(SPI1_M_HARDWARE);	//SCK PA5,  MISO PA6,  MOSI PA7,  CS0 PB0,  CS1 PB1;
	SPIx_Init(SPI2_M_BITBANG); 	//SCK PB13, MISO PB14, MOSI PB15, CS0 PB12, CS1 PA8;
	SPIx_Init(SPI3_M_SOFTWARE);	//SCK PC14, MISO PC15, MOSI PA4,  CS0 PB3,  CS1 PA15;
	ADC_Configuration();		//PA0，PA1
	TIM4_Configuration();
	Shell_Device.Init(&Shell_Device);
	
	while(1)
	{
		Start_CPU();
		
		if( Scheduler(&Led_Device.Timer) == 1)//100us执行一次
		{
			Led_Device.Driver(&Led_Device);
		}
		if( Scheduler(&Shell_Device.Timer) == 1)//100us执行一次
		{
			Shell_Device.Driver(&Shell_Device);
			Shell_Device.Put(&Shell_Device);
		}
		if( flag == 1){
			flag = 0;
			if(CAN1_BUS.RX_Message.IDE == CAN_ID_STD){
				printf("Recv: StdId=0x%x,",CAN1_BUS.RX_Message.StdId);
			}else{
				printf("Recv: ExtId=0x%x,",CAN1_BUS.RX_Message.ExtId);
			}
			printf("RTR=%d,",CAN1_BUS.RX_Message.RTR);
			printf("DLC=%d,Data:",CAN1_BUS.RX_Message.DLC);
			
			for(int i = 0; i<8;i++){
				printf("0x%x ",CAN1_BUS.RX_Message.Data[i]);
			}
			printf("\r\n");
		}
		Stop_CPU(10);
	}
}


void USART1_IRQHandler(void)
{
	if( Shell_Device.USARTx == USART1){
		Shell_Device.Get(&Shell_Device);
	}
}

void USART2_IRQHandler(void)
{
	if( Shell_Device.USARTx == USART2){
		Shell_Device.Get(&Shell_Device);
	}
}


void TIM4_IRQHandler(void)
{
	TIM_ClearFlag( TIM4, TIM_FLAG_Update);
	Timer_IncTick();//10us一次
	I2Cx_Peripheral(I2C2_M_BITBANG);
	I2Cx_Peripheral(I2C4_S_BITBANG);
	SPIx_Peripheral(SPI2_M_BITBANG);
}


void USB_LP_CAN1_RX0_IRQHandler() 
{
	if(CAN_GetITStatus(CAN1_BUS.CANx,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1_BUS.CANx,CAN_IT_FMP0); 
		CAN_Receive(CAN1_BUS.CANx,CAN_FIFO0,&CAN1_BUS.RX_Message);
		flag = 1;
	}
	
}

