#ifndef __SIMULATED_UART_H
#define __SIMULATED_UART_H


#include "core.h"

typedef struct SIM_UART_Driver_t
{
	uint16_t Tx_num;
	uint8_t *Tx_pDat;
	
	uint8_t Rx_Data;
	uint8_t Tx_Data;
	
	GPIO_TypeDef	*PORT_TX;//�˿�
	uint16_t 		PIN_TX;	//����
	
	GPIO_TypeDef	*PORT_RX;//�˿�
	uint16_t 		PIN_RX;	//����
	
	Timer_t	 Timer;
	CPU_RegisterClass_t Tx_Register;
	CPU_RegisterClass_t Rx_Register;
}SIM_UART_Driver_t;

extern SIM_UART_Driver_t SIM_UART_Driver;

uint8_t Simul_Uart_Tx(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t *pDat, uint16_t num);		//ģ�⴮�ڷ���
uint8_t Simul_Uart_Rx(SIM_UART_Driver_t *SIM_UART_Driver);					//ģ�⴮�ڽ���


#endif 
