#include "core.h"

uint32_t Cur_Usage_Cpu;
uint32_t Cur_Usage_Max_Cpu;
uint32_t TimerTickCount = 0;
uint32_t CPUTickCount = 0;


void Timer_IncTick(void)
{
	CPUTickCount++;
	TimerTickCount++;
}


void Restart_Scheduler_Timer(struct Timer_t *p)
{
	p->TickPrev = TimerTickCount;
}

uint8_t Scheduler(struct Timer_t *p)
{	
	if( p->TickPrev > TimerTickCount){
		p->TickTime = 0xFFFF-(p->TickPrev-TimerTickCount);
	}else{
		p->TickTime = TimerTickCount-p->TickPrev;
	}

	if( p->TickTime >= p->TickPeroid){
		p->TickPrev = TimerTickCount;
		return 1;
	}
	return 0;
}

void Start_CPU(void)
{
	CPUTickCount = 0;
}


void Stop_CPU(uint32_t Time)
{
	Cur_Usage_Cpu = CPUTickCount;
	if( Cur_Usage_Cpu > Cur_Usage_Max_Cpu){
		Cur_Usage_Max_Cpu = Cur_Usage_Cpu;
	}
//	while(CPUTickCount < Time);
}


inline uint32_t get_init_fp()
{
	static uint32_t *fp = (uint32_t *)0x00000000;
	return (*fp);
}

void GPIO_ToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  if( GPIOx->IDR & GPIO_Pin){
	  GPIO_SET_LOW(GPIOx, GPIO_Pin);
  }else{
	  GPIO_SET_HIGH(GPIOx, GPIO_Pin);
  }
}
