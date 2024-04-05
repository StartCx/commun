#ifndef __SHELL_TASK_H
#define __SHELL_TASK_H


#include "core.h"
#include "shell_driver.h"
#include "gpio_output.h"
#include "static_lib.h"
#include "I2C_Config.h"
#include "SPI_Config.h"
#include "ymodem.h"
#include "dht11.h"
#include "queue.h"
#include "encoder_input.h"
#include "IR_Receiver.h"
#include "key_input.h"
#include "hc_sr04.h"
#include "servo_output.h"
#include "DS18B20.h"
#include "simulated_pwm.h"

typedef struct
{
	Cmd_List_t  *pos;
	CPU_RegisterClass_t  Register;
	uint8_t Dev_Addr;
	uint16_t Reg_Addr;
	uint8_t Reg_Size;
	uint8_t Reg_Value[512];
	uint16_t Value_Length;
	GPIO_Output_Device_t *GPIO_Output;
	KeyDriver_t *KEY;
	Servo_Output_t *Servo;
	Simulated_PWM_t *Simulated_PWM;
	uint16_t TxLen;
	uint16_t RxLen;
	uint8_t TxBuf[16];
	uint8_t RxBuf[16];
	GPIO_TypeDef	*PORT;	//¶Ë¿Ú
	uint16_t 		PIN;	//Òý½Å
}Shell_Cmd_Task_t;


extern Shell_Device_Class_t Shell_Device;



extern void shell_Cmd_Init(Shell_Device_Class_t *Shell_Device);


#endif
