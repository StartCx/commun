#include "encoder_input.h"



Encoder_Input_Device_t Encoder = {
	.A_GPIOx 	= GPIOB,
	.A_GPIO_Pin = GPIO_Pin_10,
	
	.B_GPIOx 	= GPIOB,
	.B_GPIO_Pin = GPIO_Pin_11,
	.Timer = {
		.TickPeroid = 10,//1*100等于100us执行一次。
	},
};


void ENCODE_GPIO_Init(Encoder_Input_Device_t *Encoder)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Encoder->A_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Encoder->A_GPIOx, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Encoder->B_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Encoder->B_GPIOx, &GPIO_InitStructure);
}


void encode_count_read(Encoder_Input_Device_t *Encoder)
{
	enum
	{
		A_B_READ_INIT_STATE,
		A_B_READ_INIT_STATE_1,
		A_B_READ_INIT_STATE_2,
		A_LOW_B_LOW_STATE,
		A_HIGH_B_LOW_STATE,
		A_HIGH_B_HIGH_STATE,
		A_LOW_B_HIGH_STATE,
		AB_SUM,
	};
	
	static const void *function[AB_SUM] = {
		[A_B_READ_INIT_STATE] 		= &&A_B_READ_INIT_STATE,
		[A_B_READ_INIT_STATE_1] 	= &&A_B_READ_INIT_STATE_1,
		[A_B_READ_INIT_STATE_2] 	= &&A_B_READ_INIT_STATE_2,
		[A_LOW_B_LOW_STATE] 		= &&A_LOW_B_LOW_STATE,
		[A_HIGH_B_LOW_STATE] 		= &&A_HIGH_B_LOW_STATE,
		[A_HIGH_B_HIGH_STATE] 		= &&A_HIGH_B_HIGH_STATE,
		[A_LOW_B_HIGH_STATE] 		= &&A_LOW_B_HIGH_STATE,
	};
	goto *function[Encoder->Register.R15_PC];
A_B_READ_INIT_STATE:
	if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 0){
		Encoder->Register.R15_PC = A_B_READ_INIT_STATE_1;
	}else{
		Encoder->Register.R15_PC = A_B_READ_INIT_STATE_2;
	}
	return;
A_B_READ_INIT_STATE_1:
	if( GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->Register.R15_PC = A_LOW_B_LOW_STATE;
	}else{
		Encoder->Register.R15_PC = A_LOW_B_HIGH_STATE;
	}
	return;
A_B_READ_INIT_STATE_2:
	if( GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->Register.R15_PC = A_HIGH_B_LOW_STATE;
	}else{
		Encoder->Register.R15_PC = A_HIGH_B_HIGH_STATE;
	}
	return;
A_LOW_B_LOW_STATE:
	if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 1 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->cur_encode++;
		Encoder->Register.R15_PC = A_HIGH_B_LOW_STATE;
	}else if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 0 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 1){
		Encoder->cur_encode--;
		Encoder->Register.R15_PC = A_LOW_B_HIGH_STATE;
	}
	return;
A_HIGH_B_LOW_STATE:
	if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 1 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 1){
		Encoder->cur_encode++;
		Encoder->Register.R15_PC = A_HIGH_B_HIGH_STATE;
	}else if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 0 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->cur_encode--;
		Encoder->Register.R15_PC = A_LOW_B_LOW_STATE;
	}
	return;
A_HIGH_B_HIGH_STATE:
	if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 0 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 1){
		Encoder->cur_encode++;
		Encoder->Register.R15_PC = A_LOW_B_HIGH_STATE;
	}else if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 1 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->cur_encode--;
		Encoder->Register.R15_PC = A_HIGH_B_LOW_STATE;
	}
	return;
A_LOW_B_HIGH_STATE:
	if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 0 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 0){
		Encoder->cur_encode++;
		Encoder->Register.R15_PC = A_LOW_B_LOW_STATE;
	}else if( GPIO_GET_STATE(Encoder->A_GPIOx, Encoder->A_GPIO_Pin) == 1 && GPIO_GET_STATE(Encoder->B_GPIOx, Encoder->B_GPIO_Pin) == 1){
		Encoder->cur_encode--;
		Encoder->Register.R15_PC = A_HIGH_B_HIGH_STATE;
	}
	return;
}


