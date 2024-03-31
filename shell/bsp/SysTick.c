#include "SysTick.h"


void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 10000	 100us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if( SysTick_Config(SystemCoreClock / 10000))	// ST3.5.0库版本//1ms
	{ 
		/* Capture error */ 
		while (1);
	}
	// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}



