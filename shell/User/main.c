#include "stm32f10x.h"
#include "timer.h"
#include "usart.h"
#include "string.h"
#include "shell_task.h"
#include "I2C_Config.h"
#include "SPI_Config.h"
#include "core.h"

uint32_t Temp = 0;
uint8_t buf[5];

u32 SPI_FLASH_ReadID(uint8_t Bus, uint8_t CSx)
{
	
	uint8_t Write_Data = 0;
	/* 开始通讯：CS低电平 */
	SPIx_Open(Bus, CSx);

	/* 发送JEDEC指令，读取ID */
	Write_Data = 0x9F;
	SPIx_WriteThenRead(Bus,&Write_Data,1,buf,3);
	/* 停止通讯：CS高电平 */
	while( SPIx_Endp(Bus) != CORE_DONE);
	
	/*把数据组合起来，作为函数的返回值*/
	Temp = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	SPIx_Close(Bus, CSx);
	return Temp;
}

uint8_t ret = 0,i,imu_data[12]; 
uint16_t acc_x,acc_y,acc_z;
uint16_t gyr_x,gyr_y,gyr_z;

int main(void)
{	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	 //打开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	 //打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	 //打开时钟
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
	
//	I2Cx_Open(I2C1_M_HARDWARE);
//	I2Cx_Open(I2C2_M_BITBANG);
//	I2Cx_Open(I2C3_M_SOFTWARE);
//	I2Cx_Open(I2C4_S_BITBANG);
//	
//	SPIx_Open(SPI1_M_HARDWARE,0);
//	SPIx_Open(SPI2_M_BITBANG, 0);
//	SPIx_Open(SPI3_M_SOFTWARE,0);
	
	
	
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
	Timer_IncTick();//10us一次
	I2Cx_Peripheral(I2C2_M_BITBANG);
	I2Cx_Peripheral(I2C4_S_BITBANG);
	SPIx_Peripheral(SPI2_M_BITBANG);
}

