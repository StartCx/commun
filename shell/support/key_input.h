#ifndef __KEY_DRIVER_H
#define __KEY_DRIVER_H


#include "core.h"
#include "queue.h"


#define KEY_DEBOUNCE 			(5) 	//消抖次数
#define	KEY_PRESS_LONG_TIME		(200)	//长按时间2s
#define KEY_PRESS_CONTINUE_TIME	(15)	//持续长按间隔时间500ms 


// 按键检测过程
typedef enum
{
	KEY_STEP_WAIT,				//等待按键
	KEY_STEP_CLICK,				//按键按下
	KEY_STEP_LONG_PRESS,		//长按
	KEY_STEP_CONTINUOUS_PRESS,  //持续按下
	KEY_STEP_PROC_SUM
}KEY_STEP_TYPEDEF;


typedef enum
{	
	KEY_IDLE = 0,       	 	 //按键空闲
	KEY_CLICK,          		 //单击确认
	KEY_CLICK_RELEASE,           //短按释放
	KEY_LONG_PRESS,			   	 //长按确认
	KEY_LONG_PRESS_CONTINUOUS,	 //长按持续
	KEY_LONG_PRESS_RELEASE		 //长按释放
}KEY_EVENT_TYPEDEF;

typedef struct
{
	uint32_t KeyStep;			//按键检测流程
	uint32_t KeyDebounce;		//按键消抖时间
	uint8_t  EVENT;				//事件返回
	uint32_t KeyPressLongTimer;	//长按延时
	uint32_t KeyContPressTimer;	//连续长按延时
	uint8_t  KeyPressedState; 	//按下时状态
	GPIO_TypeDef	*KEY_GPIOx;	//端口
	uint16_t 		KEY_GPIO_Pin;	//引脚
	Queue64_t 		Queue;
	Timer_t			Timer;
}KeyDriver_t;

extern KeyDriver_t Encode_KEY;


void Key_Event_Update(KeyDriver_t * KeyDriver);
void Key_GPIO_Init(KeyDriver_t * KeyDriver);


#endif
