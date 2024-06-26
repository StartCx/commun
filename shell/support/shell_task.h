#ifndef __SHELL_TASK_H
#define __SHELL_TASK_H


#include "core.h"
#include "can.h"
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
#include "w25qx.h"
#include "at24cxx.h"
#include "oled.h"
#include "timer.h"

typedef struct
{
	Cmd_List_t  *pos;
	CPU_RegisterClass_t  Register;
	//I2C
	uint8_t 	Dev_Addr;
	uint16_t 	Reg_Addr;
	uint8_t 	Reg_Size;
	uint8_t 	Reg_Value[512];
	uint16_t 	Value_Length;
	//SPI
	uint16_t 	TxLen;
	uint16_t 	RxLen;
	uint8_t 	TxBuf[16];
	uint8_t 	RxBuf[16];
	
	//lib
	sscanf_t 	sscanf_driver;
	fmt_t 		fmt_driver;
	
	GPIO_Output_Device_t *GPIO_Output;
	GPIO_TypeDef	*PORT;	//�˿�
	uint16_t 		PIN;	//����
}Shell_Cmd_Task_t;


extern Shell_Core_Class_t Shell_Device;

extern void shell_Cmd_Init(Shell_Core_Class_t *Shell_Device);


#endif



//Shell_Device->Bus = Bus;
//if( Shell_Device->Bus == USART1_BUS){
//	Shell_Device->USARTx= USART1;
//	Shell_Device->Get  	= shell_Getchar_IT;
//	Shell_Device->Put  	= shell_Putchar;
//}else if( Shell_Device->Bus == SIMUL1_BUS){
//	Shell_Device->SIM_UART_Driver = &SIM_UART;
//	Shell_Device->Get  	= shell_SIM_Uart_Getchar;
//	Shell_Device->Put  	= shell_SIM_Uart_Putchar;
//}
