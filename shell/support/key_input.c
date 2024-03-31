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
		.TickPeroid = 1000,//10*1000等于10ms执行一次。
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
	KeyDriver->EVENT = KEY_IDLE; //按键空闲
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
			KeyDriver->EVENT = KEY_CLICK; //单击确认
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
			KeyDriver->KeyStep = KEY_STEP_LONG_PRESS;
		}else{
			KeyDriver->EVENT = KEY_IDLE;//等待
		}
	}
	else
	{
		KeyDriver->KeyDebounce = KEY_DEBOUNCE;
		KeyDriver->EVENT = KEY_IDLE; //按键空闲
		KeyDriver->KeyStep = KEY_STEP_WAIT;
	}
	return;				
KEY_STEP_LONG_PRESS:
	if( GPIO_GET_STATE(KeyDriver->KEY_GPIOx, KeyDriver->KEY_GPIO_Pin) == Encode_KEY.KeyPressedState)
	{	
		if((KeyDriver->KeyPressLongTimer--) == 0)
		{
			KeyDriver->KeyPressLongTimer = KEY_PRESS_LONG_TIME;
			KeyDriver->EVENT = KEY_LONG_PRESS;//长按确认
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
			KeyDriver->KeyStep = KEY_STEP_CONTINUOUS_PRESS;
		}else{
			KeyDriver->EVENT = KEY_IDLE;//等待
		}
	}
	else
	{
		KeyDriver->KeyPressLongTimer = KEY_PRESS_LONG_TIME;
		KeyDriver->EVENT = KEY_CLICK_RELEASE;//单击释放
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
			KeyDriver->EVENT = KEY_LONG_PRESS_CONTINUOUS;//持续按下
			QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
		}else{
			KeyDriver->EVENT = KEY_IDLE;//等待
		}
	}
	else
	{
		KeyDriver->KeyContPressTimer = KEY_PRESS_CONTINUE_TIME;
		KeyDriver->EVENT = KEY_LONG_PRESS_RELEASE;//长按释放
		QueueDataIn(&KeyDriver->Queue, &KeyDriver->EVENT);
		KeyDriver->KeyStep = KEY_STEP_WAIT;
	}
	return;	
}






