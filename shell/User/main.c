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
#include "simulated_uart.h"


int main(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 	//打开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	 	//打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	CAN1_Configuration();		//PA11， PA12 		波特率 250k	优先级 2，0
	Usart1_Init(50000);			//PA9 ， PA10		波特率 50k 	优先级 1，0
	SIM_Uart_Init(&SIM_UART);	//PA2 ， PA3 		波特率 50k	

	Led_Device.Init(&Led_Device);//PC13
	I2Cx_Init(I2C1_M_HARDWARE);	//SCL PB6 ， SDA PB7 	50k		index = 0
	I2Cx_Init(I2C2_M_BITBANG);	//SCL PB10， SDA PB11	50k 	index = 1
	I2Cx_Init(I2C3_M_SOFTWARE);	//SCL PB8 ， SDA PB9	100k+ 	index = 2
	I2Cx_Init(I2C4_S_BITBANG);	//SCL PB4 ， SDA PB5	50k		
	
	SPIx_Init(SPI1_M_HARDWARE);	//SCK PA5,  MISO PA6,  MOSI PA7,  CS0 PA15; index = 0
	SPIx_Init(SPI2_M_BITBANG); 	//SCK PB13, MISO PB14, MOSI PB15, CS0 PB12; index = 1
	SPIx_Init(SPI3_M_SOFTWARE);	//SCK PC14, MISO PC15, MOSI PA4,  CS0 PB3;	index = 2
	ADC_Configuration();		//PA0，PA1
	TIM4_Configuration();		//优先级 0，0
	TIM3_Configuration();				//pwm PB0 PB1 	period 20ms	index = 1 index = 2	
	SIM_Servo_GPIO_Init(&Servo_Output0);//sim PA8		period 20ms	index = 0			
	
	Shell_Device.Init(&Shell_Device, SIMUL1_BUS);//shell选择串口
	
	IWDG_Init();
	printf("\r\n[  Shell Version: 1.5.7 ] -- [ DATE: %s ] -- [ TIME: %s ]\r\n",__DATE__,__TIME__);
	
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
		if( Scheduler(&Servo_Output0.Timer) == 1)//100us执行一次
		{
			sim_servo_output_speed_task(&Servo_Output0);
		}
		if( Scheduler(&Servo_Output1.Timer) == 1)//100us执行一次
		{
			pwm_servo_output_speed_task(&Servo_Output1);
		}
		if( Scheduler(&Servo_Output2.Timer) == 1)//100us执行一次
		{
			pwm_servo_output_speed_task(&Servo_Output2);
		}
		
		Stop_CPU(10);
	}
}


void USART1_IRQHandler(void)
{	
	if( Shell_Device.Bus == USART1_BUS){
		Shell_Device.Get(&Shell_Device);
	}else{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
	}
}


void SIM_Uart_Rx_IRQHandler(SIM_UART_Driver_t *SIM_UART_Driver)
{
	if( SIM_Uart_Rx_Peripheral(SIM_UART_Driver) == 1){ //串口接收中断
		Shell_Device.Get(&Shell_Device);
	}
}


void TIM4_IRQHandler(void)
{
	static uint8_t SIM_Uart_Cnt;
	static uint8_t SIM_I2Cx_Cnt;
	static uint16_t WDG_Cnt;
	
	SIM_Uart_Cnt++;
	if( SIM_Uart_Cnt >= 2 && Shell_Device.Bus == SIMUL1_BUS){
		SIM_Uart_Cnt = 0;
		SIM_Uart_Tx_Peripheral(&SIM_UART);
		SIM_Uart_Rx_IRQHandler(&SIM_UART);
	}
	sim_servo_output_angle_task(&Servo_Output0);
	TIM_ClearFlag( TIM4, TIM_FLAG_Update);
	Timer_IncTick();//10us一次
	SIM_I2Cx_Cnt++;
	if( SIM_I2Cx_Cnt >= 2){
		SIM_I2Cx_Cnt = 0;
		I2Cx_Peripheral(I2C2_M_BITBANG);
	}
	I2Cx_Peripheral(I2C4_S_BITBANG);
	SPIx_Peripheral(SPI2_M_BITBANG);
	
	WDG_Cnt++;
	if( WDG_Cnt > 50000){
		IWDG_Feed();
	}
}


void USB_LP_CAN1_RX0_IRQHandler() 
{
	if(CAN_GetITStatus(CAN1_BUS.CANx,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1_BUS.CANx,CAN_IT_FMP0); 
		CAN_Receive(CAN1_BUS.CANx,CAN_FIFO0,&CAN1_BUS.RX_Message);
		CAN1_BUS.RX_Flag = 1;
	}
}


//重定义fputc函数 
int fputc(int ch, FILE *f)
{    
	if( Shell_Device.Bus == USART1_BUS){
		while((USART1->SR & USART_FLAG_TXE) == RESET);	
		USART_SendData(USART1, (uint8_t) ch);
	}else if( Shell_Device.Bus == SIMUL1_BUS){
		SIM_Uart_SendByte( &SIM_UART, ch);
	}
	return (ch);
}
