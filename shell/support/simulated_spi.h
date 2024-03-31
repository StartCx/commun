#ifndef __SIMULATED_SPI_H
#define __SIMULATED_SPI_H

#include "stm32f10x.h"
#include "core.h"
#include "gpio_output.h"

#define CPOL 	0		//Clock Polarity时钟极性
#define CPHA	0		//Clock Phase。 时钟相位

/* 宏定义 --------------------------------------------------------------------*/
#define PORT_SPI_CS0              GPIOB
#define PORT_SPI_CS1              GPIOB
#define PORT_SPI_SCK              GPIOA
#define PORT_SPI_MISO             GPIOA
#define PORT_SPI_MOSI             GPIOA

#define PIN_SPI_CS1               GPIO_Pin_7
#define PIN_SPI_CS0               GPIO_Pin_9
#define PIN_SPI_SCK               GPIO_Pin_5
#define PIN_SPI_MISO              GPIO_Pin_6
#define PIN_SPI_MOSI              GPIO_Pin_7



enum
{
	SPI_IDLE = 0,
	SPI_WRITE_AND_READ,
	SPI_WRITE_THEN_READ,
};

enum
{
	SPI_DRIVER_PROC_READY = 0,
	SPI_DRIVER_MODE_SELECT,
	SPI_DRIVER_WRITE_THEN_READ0,
	SPI_DRIVER_WRITE_THEN_READ1,
	SPI_DRIVER_MODE_PARAM_INIT,
	SPI_DRIVER_TX_DATA_SET,
	SPI_DRIVER_LOOP_TX_DATA,
	SPI_DRIVER_LOOP_RX_DATA,
	SPI_DRIVER_MASK_SHIFT,
	SPI_DRIVER_RX_DATA_GET,
	SPI_DRIVER_SIZE_COMPARE,
	SPI_DRIVER_PROC_ENDP,
	SPI_DRIVER_SUM,
};
	
typedef struct SPI_Driver_t
{	
	uint8_t _CPOL;
	uint8_t _CPHA;
	
	uint8_t Mode;
	uint8_t *pTx_data;
	uint8_t *pRx_data;
	uint8_t *pRx_data_bak;
	uint16_t TxSize;
	uint16_t RxSize;
	uint16_t Size;
	
	GPIO_TypeDef	*PORT_CS[2];//端口
	uint16_t 		PIN_CS[2];	//引脚
	
	GPIO_TypeDef	*PORT_SCK;	//端口
	uint16_t 		PIN_SCK;	//引脚

	GPIO_TypeDef	*PORT_MISO;	//端口
	uint16_t 		PIN_MISO;	//引脚
	
	GPIO_TypeDef	*PORT_MOSI;	//端口
	uint16_t 		PIN_MOSI;	//引脚
	
	CPU_RegisterClass_t Register;
	
	void (*Init)(struct SPI_Driver_t *Dev);
	void (*Driver)(struct SPI_Driver_t *Dev);
	void (*write_and_read)(struct SPI_Driver_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
	void (*write_then_read)(struct SPI_Driver_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
	uint8_t (*open)(struct SPI_Driver_t *Dev, uint8_t ID);
	void (*close)(struct SPI_Driver_t *Dev,uint8_t ID);
	uint8_t (*endp)(struct SPI_Driver_t *Dev);
}SPI_Driver_t;

/* 函数申明 ------------------------------------------------------------------*/


extern SPI_Driver_t SPI_Driver0;
	


void spi_write_and_read(SPI_Driver_t *SPI_Driver,uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
void spi_write_then_read(SPI_Driver_t *SPI_Driver,uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);

void SPI_Period_Driver(SPI_Driver_t *SPI_Driver);
void SPI_Configuration(SPI_Driver_t *SPI_Driver);
uint8_t spi_write_read_endp(SPI_Driver_t *SPI_Driver);
uint8_t spi_open_dev(SPI_Driver_t *SPI_Driver,uint8_t ID);
void spi_close_dev(SPI_Driver_t *SPI_Driver,uint8_t ID);

#endif
