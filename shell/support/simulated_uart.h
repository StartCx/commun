#ifndef __SIMULATED_UART_H
#define __SIMULATED_UART_H


#include "core.h"


typedef enum
{
	SIM_USART_RX_STATE,
	SIM_USART_TX_STATE,
}Sim_Uart_State;

typedef struct SIM_UART_Driver_t
{
	uint16_t Tx_num;
	uint8_t *Tx_pDat;
	
	uint8_t Rx_Data;
	uint8_t Tx_Data;
	
	GPIO_TypeDef	*PORT_TX;//端口
	uint16_t 		PIN_TX;	//引脚
	
	GPIO_TypeDef	*PORT_RX;//端口
	uint16_t 		PIN_RX;	//引脚
	
	Timer_t	 Timer;
	CPU_RegisterClass_t Tx_Register;
	CPU_RegisterClass_t Rx_Register;
}SIM_UART_Driver_t;

extern SIM_UART_Driver_t SIM_UART;
extern void SIM_Uart_Init(SIM_UART_Driver_t *SIM_UART_Driver);
void SIM_USART_SendData(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t ch);
void SIM_Uart_SendByte( SIM_UART_Driver_t *SIM_UART_Driver, uint8_t ch);
extern void SIM_Uart_SendArray(SIM_UART_Driver_t *SIM_UART_Driver, uint8_t *pDat, uint16_t num);
extern void SIM_Uart_Tx_Peripheral(SIM_UART_Driver_t *SIM_UART_Driver);	
extern uint8_t SIM_Uart_Rx_Peripheral(SIM_UART_Driver_t *SIM_UART_Driver);			
extern void SIM_Uart_Rx_IRQHandler(SIM_UART_Driver_t *SIM_UART_Driver);

extern uint8_t SIM_UART_GetFlagStatus(SIM_UART_Driver_t *SIM_UART_Driver, Sim_Uart_State State);
extern void SIM_UART_SetFlagStatus(SIM_UART_Driver_t *SIM_UART_Driver, Sim_Uart_State State, uint8_t value);

extern void SIM_Uart_Rx_IRQHandler(SIM_UART_Driver_t *SIM_UART_Driver);


#endif 
