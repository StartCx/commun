#include "gpio_output.h"






GPIO_Output_Device_t Led_Device = {
	.Attribute = {
		.GPIOx  	= GPIOC,
		.GPIO_Pin 	= GPIO_Pin_13,
		.LightDarkPeriod = 20,//100us一次，10步，gpio反转一次时间1ms，周期设为10，目的是为了亮度。
	},
	.Timer = {
		.TickPeroid = 10,//10*10等于100us执行一次。
	},
	.Init 	= GPIO_Out_Init,
	.Set  	= GPIO_Out_Set,
	.Driver = GPIO_Out_Driver,
	.Quit	= GPIO_Out_Quit,
};

#define GPIO_PIN_HIGH 1
#define GPIO_PIN_LOW 0


#define GPIO_EFFECT_END	0xFFFE
#define GPIO_EFFECT_AGN	0xFFFF

//1等于10ms
const unsigned short GPIO_LOW_STATE[] = 	{GPIO_PIN_LOW , 10, GPIO_EFFECT_END};//10等于100ms
const unsigned short GPIO_HIGH_STATE[]= 	{20, 10, GPIO_EFFECT_END};
const unsigned short GPIO_HPD_STATE[] = 	{20,2000, GPIO_PIN_LOW,20,GPIO_EFFECT_END};
const unsigned short GPIO_TOGGLE_STATE[] = 	{0, 700,1,700,0,700,5,700,GPIO_EFFECT_AGN,8};

const unsigned short *gpio_mode[] ={
	[GPIO_LOW] 		= GPIO_LOW_STATE,
	[GPIO_HIGH] 	= GPIO_HIGH_STATE,
	[GPIO_HPD] 		= GPIO_HPD_STATE,
	[GPIO_TOGGLE] 	= GPIO_TOGGLE_STATE,
};



void GPIO_Out_Set(GPIO_Output_Device_t *Dev, uint8_t mode)
{
	Dev->Attribute.duty = (unsigned short *)gpio_mode[mode];
	Dev->Attribute.Timer = *(Dev->Attribute.duty+1);
	Dev->Register.R15_PC  = 0;
}

void GPIO_Out_Init(GPIO_Output_Device_t *Dev)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = Dev->Attribute.GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Dev->Attribute.GPIOx, &GPIO_InitStructure);
	GPIO_SET_LOW(Dev->Attribute.GPIOx, Dev->Attribute.GPIO_Pin);
	GPIO_Out_Set(Dev, GPIO_LOW);
}

void GPIO_Out_Quit(GPIO_Output_Device_t *Dev)
{
	GPIO_Out_Set(Dev, GPIO_LOW);
}

void GPIO_Out_Driver(GPIO_Output_Device_t *Dev)
{
	enum{
		CALL_GPIO_OUTPUT_PROC_GROUP0 = 0,
		CALL_GPIO_OUTPUT_PROC_GROUP1,
		CALL_GPIO_OUTPUT_PROC_GROUP2,
		CALL_GPIO_OUTPUT_PROC_GROUP3,
		CALL_GPIO_OUTPUT_PROC_GROUP4,
		CALL_GPIO_OUTPUT_PROC_GROUP5,
		CALL_GPIO_OUTPUT_PROC_GROUP6,
		CALL_GPIO_OUTPUT_PROC_GROUP7,
		CALL_GPIO_OUTPUT_PROC_GROUP8,
		CALL_GPIO_OUTPUT_PROC_GROUP9,
		CALL_GPIO_OUTPUT_SUM
	};
	static const void *function[CALL_GPIO_OUTPUT_SUM] = {
		[CALL_GPIO_OUTPUT_PROC_GROUP0] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP0,
		[CALL_GPIO_OUTPUT_PROC_GROUP1] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP1,
		[CALL_GPIO_OUTPUT_PROC_GROUP2] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP2,
		[CALL_GPIO_OUTPUT_PROC_GROUP3] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP3,
		[CALL_GPIO_OUTPUT_PROC_GROUP4] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP4,
		[CALL_GPIO_OUTPUT_PROC_GROUP5] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP5,
		[CALL_GPIO_OUTPUT_PROC_GROUP6] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP6,
		[CALL_GPIO_OUTPUT_PROC_GROUP7] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP7,
		[CALL_GPIO_OUTPUT_PROC_GROUP8] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP8,
		[CALL_GPIO_OUTPUT_PROC_GROUP9] 	= &&CALL_GPIO_OUTPUT_PROC_GROUP9,
	};
	Dev->Register.R1_Index++;
	goto *function[Dev->Register.R15_PC];
CALL_GPIO_OUTPUT_PROC_GROUP0:
	if( Dev->Attribute.Timer){
		Dev->Attribute.Timer--;
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP5;
	}else{
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP1;
	}
	return;
CALL_GPIO_OUTPUT_PROC_GROUP1:
	if( *(Dev->Attribute.duty+2) != GPIO_EFFECT_END){
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP2;
	}else{
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP5;
	}
	return;
CALL_GPIO_OUTPUT_PROC_GROUP2:
	Dev->Attribute.duty += 2;
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP3;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP3:
	if(*(Dev->Attribute.duty) == GPIO_EFFECT_AGN){
		Dev->Attribute.duty -= *(Dev->Attribute.duty+1);
	}
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP4;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP4:
	Dev->Attribute.Timer = *(Dev->Attribute.duty+1);
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP5;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP5:
	Dev->Register.R6_Count = Dev->Attribute.Timer % Dev->Attribute.LightDarkPeriod;
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP6;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP6://10到0		//1
	if( Dev->Register.R6_Count < *Dev->Attribute.duty){
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP7;
	}else{
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP8;
	}
	return;
CALL_GPIO_OUTPUT_PROC_GROUP7:
	GPIO_SET_LOW(Dev->Attribute.GPIOx, Dev->Attribute.GPIO_Pin);
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP9;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP8:
	GPIO_SET_HIGH(Dev->Attribute.GPIOx, Dev->Attribute.GPIO_Pin);
	Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP9;
	return;
CALL_GPIO_OUTPUT_PROC_GROUP9:
	if( Dev->Register.R1_Index >= 10){//固定一个驱动时间长度，目前100us调用一次，调用10次是1ms切换一次gpio的状态。
		Dev->Register.R1_Index = 0;
		Dev->Register.R15_PC = CALL_GPIO_OUTPUT_PROC_GROUP0;
	}
	return;
}


