#include "key_input.h"


KeyDriver_t Encode_KEY = {
	.EVENT 				= KEY_IDLE,
	.KeyDebounce 		= KEY_DEBOUNCE,
	.KeyPressLongTimer 	= KEY_PRESS_LONG_TIME,
	.KeyContPressTimer 	= KEY_PRESS_CONTINUE_TIME,
	.KeyPressedState	= 0,
	.KEY_GPIOx 			= GPIOB,
	.KEY_GPIO_Pin 		= GPIO_Pin_0,
	.Timer = {
		.TickPeroid = 1000,//10*1000����10msִ��һ�Ρ�
	},
};


void Key_GPIO_Init(KeyDriver_t * KeyDriver)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = KeyDriver->KEY_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KeyDriver->KEY_GPIOx, &GPIO_InitStructure);
	
	QueueEmpty(&KeyDriver->Queue);
}


void Key_Event_Update(KeyDriver_t * KeyDriver)
{	
	static const void *function[KEY_STEP_PROC_SUM] = {
		[KEY_STEP_WAIT] 			= &&KEY_STEP_WAIT,
		[KEY_STEP_CLICK] 			= &&KEY_STEP_CLICK,
		[KEY_STEP_LONG_PRESS] 		= &&KEY_STEP_LONG_PRESS,
		[KEY_STEP_CONTINUOUS_PRESS] = &&KEY_STEP_CONTINUOUS_PRESS,
	};
	goto *function[KeyDriver->KeyStep];
KEY_STEP_WAIT:
	KeyDriver->EVENT = KEY_IDLE; //��������
	if( GPIO_GET_STATE(KeyDriver->KEY_GPIOx, KeyDriver->KEY_GPIO_Pin) == Encode_KEY.KeyPressedState)
	{
		KeyDriver->KeyStep = KEY_STEP_CLICK;	
	}
	return;	
KEY_STEP_CLICK:
	if( GPIO_GET_STATE(KeyDriver->KEY_GPIOx, KeyDriver->KEY_GPIO_Pin) == Encode_KEY.KeyPressedState)
	{
		if((KeyDriver->KeyDebounce--) == 0)
		{
			KeyDriver->KeyDebounce = KEY_DEBOUNCE;
			KeyDriver->EVENT = KEY_CLICK; //����ȷ��
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
			KeyDriver->KeyStep = KEY_STEP_LONG_PRESS;
		}else{
			KeyDriver->EVENT = KEY_IDLE;//�ȴ�
		}
	}
	else
	{
		KeyDriver->KeyDebounce = KEY_DEBOUNCE;
		KeyDriver->EVENT = KEY_IDLE; //��������
		KeyDriver->KeyStep = KEY_STEP_WAIT;
	}
	return;				
KEY_STEP_LONG_PRESS:
	if( GPIO_GET_STATE(KeyDriver->KEY_GPIOx, KeyDriver->KEY_GPIO_Pin) == Encode_KEY.KeyPressedState)
	{	
		if((KeyDriver->KeyPressLongTimer--) == 0)
		{
			KeyDriver->KeyPressLongTimer = KEY_PRESS_LONG_TIME;
			KeyDriver->EVENT = KEY_LONG_PRESS;//����ȷ��
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
			KeyDriver->KeyStep = KEY_STEP_CONTINUOUS_PRESS;
		}else{
			KeyDriver->EVENT = KEY_IDLE;//�ȴ�
		}
	}
	else
	{
		KeyDriver->KeyPressLongTimer = KEY_PRESS_LONG_TIME;
		KeyDriver->EVENT = KEY_CLICK_RELEASE;//�����ͷ�
		QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
		KeyDriver->KeyStep = KEY_STEP_WAIT;
	}
	return;
KEY_STEP_CONTINUOUS_PRESS:
	if( GPIO_GET_STATE(KeyDriver->KEY_GPIOx, KeyDriver->KEY_GPIO_Pin) == Encode_KEY.KeyPressedState)
	{
		if((KeyDriver->KeyContPressTimer--) == 0)
		{
			KeyDriver->KeyContPressTimer = KEY_PRESS_CONTINUE_TIME;
			KeyDriver->EVENT = KEY_LONG_PRESS_CONTINUOUS;//��������
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
		}else{
			KeyDriver->EVENT = KEY_IDLE;//�ȴ�
		}
	}
	else
	{
		KeyDriver->KeyContPressTimer = KEY_PRESS_CONTINUE_TIME;
		KeyDriver->EVENT = KEY_LONG_PRESS_RELEASE;//�����ͷ�
		QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
		KeyDriver->KeyStep = KEY_STEP_WAIT;
	}
	return;	
}






