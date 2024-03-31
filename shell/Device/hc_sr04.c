#include "hc_sr04.h"


HC_SR04_Driver_t HC_SR04_Dev = {
	.GPIOx_TRIG		= GPIOB,
	.GPIO_Pin_TRIG	= GPIO_Pin_9,
	.GPIOx_ECHO		= GPIOB,
	.GPIO_Pin_ECHO	= GPIO_Pin_8,
	.Timer = {
		.TickPeroid = 10,//10*10等于100us执行一次。
	},
};

void HC_SR04_Init(HC_SR04_Driver_t *HC_SR04)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
  	GPIO_InitStructure.GPIO_Pin = HC_SR04->GPIO_Pin_TRIG;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init( HC_SR04->GPIOx_TRIG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC_SR04->GPIO_Pin_ECHO;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init( HC_SR04->GPIOx_ECHO,&GPIO_InitStructure);
}


void HC_SR04_Driver(HC_SR04_Driver_t *HC_SR04)
{
	enum{
		HC_SR04_PROC0,
		HC_SR04_PROC1,
		HC_SR04_PROC2,
		HC_SR04_PROC3,
		HC_SR04_PROC4,
		HC_SR04_ENDP,
		HC_SR04_SUM,
	};
	static const void *function[HC_SR04_SUM] = {
		[HC_SR04_PROC0] 		= &&HC_SR04_PROC0,
		[HC_SR04_PROC1] 		= &&HC_SR04_PROC1,
		[HC_SR04_PROC2] 		= &&HC_SR04_PROC2,
		[HC_SR04_PROC3] 		= &&HC_SR04_PROC3,
		[HC_SR04_PROC4] 		= &&HC_SR04_PROC4,
		[HC_SR04_ENDP] 			= &&HC_SR04_ENDP,
	};
	
	goto *function[HC_SR04->Register.R15_PC];
HC_SR04_PROC0:
	GPIO_SET_HIGH(HC_SR04->GPIOx_TRIG, HC_SR04->GPIO_Pin_TRIG);
	HC_SR04->Register.R15_PC = HC_SR04_PROC1;
	return;
HC_SR04_PROC1:
	GPIO_SET_LOW(HC_SR04->GPIOx_TRIG, HC_SR04->GPIO_Pin_TRIG);
	HC_SR04->Register.R6_Count = 0;
	HC_SR04->Register.TimerTick = 100;
	HC_SR04->Register.R15_PC = HC_SR04_PROC2;
	return;
HC_SR04_PROC2:
	if( GPIO_GET_STATE(HC_SR04->GPIOx_ECHO, HC_SR04->GPIO_Pin_ECHO) == 1){
		HC_SR04->Register.R6_Count++;
		HC_SR04->Register.R15_PC = HC_SR04_PROC3;
	}else if( HC_SR04->Register.TimerTick == 0){
		HC_SR04->Register.R0_Result = 0;
		HC_SR04->Register.TimerTick = 2000; //200ms
		HC_SR04->Register.R15_PC = HC_SR04_ENDP;
	}else{
		HC_SR04->Register.TimerTick--;
	}
	return;
HC_SR04_PROC3:
	if( GPIO_GET_STATE(HC_SR04->GPIOx_ECHO, HC_SR04->GPIO_Pin_ECHO) == 1){
		HC_SR04->Register.R6_Count++;
	}else{
		HC_SR04->Register.R15_PC = HC_SR04_PROC4;
	}
	return;
HC_SR04_PROC4:
	HC_SR04->Register.R0_Result = HC_SR04->Register.R6_Count;
	HC_SR04->Register.TimerTick = 2000; //200ms
	HC_SR04->Register.R15_PC = HC_SR04_ENDP;
	return;
HC_SR04_ENDP:
	if( HC_SR04->Register.TimerTick == 0){
		HC_SR04->Register.R15_PC = HC_SR04_PROC0;
	}else{
		HC_SR04->Register.TimerTick--;
	}
	return;
}







