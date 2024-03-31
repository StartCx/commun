#ifndef __KEY_DRIVER_H
#define __KEY_DRIVER_H


#include "core.h"
#include "queue.h"


#define KEY_DEBOUNCE 			(5) 	//��������
#define	KEY_PRESS_LONG_TIME		(200)	//����ʱ��2s
#define KEY_PRESS_CONTINUE_TIME	(15)	//�����������ʱ��500ms 


// ����������
typedef enum
{
	KEY_STEP_WAIT,				//�ȴ�����
	KEY_STEP_CLICK,				//��������
	KEY_STEP_LONG_PRESS,		//����
	KEY_STEP_CONTINUOUS_PRESS,  //��������
	KEY_STEP_PROC_SUM
}KEY_STEP_TYPEDEF;


typedef enum
{	
	KEY_IDLE = 0,       	 	 //��������
	KEY_CLICK,          		 //����ȷ��
	KEY_CLICK_RELEASE,           //�̰��ͷ�
	KEY_LONG_PRESS,			   	 //����ȷ��
	KEY_LONG_PRESS_CONTINUOUS,	 //��������
	KEY_LONG_PRESS_RELEASE		 //�����ͷ�
}KEY_EVENT_TYPEDEF;

typedef struct
{
	uint32_t KeyStep;			//�����������
	uint32_t KeyDebounce;		//��������ʱ��
	uint8_t  EVENT;				//�¼�����
	uint32_t KeyPressLongTimer;	//������ʱ
	uint32_t KeyContPressTimer;	//����������ʱ
	uint8_t  KeyPressedState; 	//����ʱ״̬
	GPIO_TypeDef	*KEY_GPIOx;	//�˿�
	uint16_t 		KEY_GPIO_Pin;	//����
	Queue64_t 		Queue;
	Timer_t			Timer;
}KeyDriver_t;

extern KeyDriver_t Encode_KEY;


void Key_Event_Update(KeyDriver_t * KeyDriver);
void Key_GPIO_Init(KeyDriver_t * KeyDriver);


#endif
