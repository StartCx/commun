#include "SPI_Config.h"

#define SPI1_NUM_CS 1  // SPI1��1��Ƭѡ����
#define SPI2_NUM_CS 2  // SPI2��2��Ƭѡ����
#define SPI3_NUM_CS 1  // SPI3��2��Ƭѡ����


// ����SPI1��Ƭѡ��������
SPI_CS_Pin_t SPI1_CS_Pins[SPI1_NUM_CS] = {
    {GPIOA, GPIO_Pin_15}
};

// ����SPI2��Ƭѡ��������
SPI_CS_Pin_t SPI2_CS_Pins[SPI2_NUM_CS] = {
    {GPIOB, GPIO_Pin_12},
	{GPIOA, GPIO_Pin_8},
};

// ����SPI3��Ƭѡ��������
SPI_CS_Pin_t SPI3_CS_Pins[SPI3_NUM_CS] = {
    {GPIOB, GPIO_Pin_3},
};

SPI_M_Hardware_t SPI1_Master = {
	.SPIx		= SPI1,
	.SPI_Mode	= SPI_MODE_0,
	.First_Bit	= MSB_FIRST,
	.Delay_time	= 100,
	.Lock 		= CORE_UNLOCK,
	.PORT_SCK  	= GPIOA,
	.PIN_SCK   	= GPIO_Pin_5,
	.PORT_MISO 	= GPIOA,
	.PIN_MISO  	= GPIO_Pin_6,
	.PORT_MOSI 	= GPIOA,
	.PIN_MOSI  	= GPIO_Pin_7,
	
	.CS_Pins 	= SPI1_CS_Pins,
	.CS_SUM		= SPI1_NUM_CS,
	
	.Init			= SPI_M_Hardware_Config,
	.WriteAndRead	= SPI_M_Hardware_WriteAndRead,
	.WriteThenRead	= SPI_M_Hardware_WriteThenRead,
	.Close			= SPI_M_Hardware_Close,
	.Open			= SPI_M_Hardware_Open,
	.Endp			= SPI_M_Hardware_Endp,
	.Mode			= SPI_M_Hardware_Mode,
};


SPI_M_Bitbang_t SPI2_Master = {
	.SPI_Mode	= SPI_MODE_0,
	.First_Bit	= MSB_FIRST,
	.Register.R12_Lock = CORE_UNLOCK,
	.PORT_SCK  = GPIOB,
	.PIN_SCK   = GPIO_Pin_13,
	.PORT_MISO = GPIOB,
	.PIN_MISO  = GPIO_Pin_14,
	.PORT_MOSI = GPIOB,
	.PIN_MOSI  = GPIO_Pin_15,
	
	.CS_Pins 	= SPI2_CS_Pins,
	.CS_SUM		= SPI2_NUM_CS,
	
	.Init			= SPI_M_Bitbang_Config,
	.Driver			= SPI_M_Bitbang_Peripheral,
	.WriteAndRead	= SPI_M_Bitbang_WriteAndRead,
	.WriteThenRead	= SPI_M_Bitbang_WriteThenRead,
	.Close			= SPI_M_Bitbang_Close,
	.Open			= SPI_M_Bitbang_Open,
	.Endp			= SPI_M_Bitbang_Endp,
	.Mode			= SPI_M_Bitbang_Mode,
};




SPI_M_Software_t SPI3_Master = {
	.SPI_Mode	= SPI_MODE_0,
	.First_Bit	= MSB_FIRST,
	.Delay_time	= 1,
	.Lock 		= CORE_UNLOCK,
	.PORT_SCK  	= GPIOC,
	.PIN_SCK   	= GPIO_Pin_14,
	.PORT_MISO 	= GPIOC,
	.PIN_MISO  	= GPIO_Pin_15,
	.PORT_MOSI 	= GPIOA,
	.PIN_MOSI  	= GPIO_Pin_4,
	
	.CS_Pins 	= SPI3_CS_Pins,
	.CS_SUM		= SPI3_NUM_CS,
	
	.Init			= SPI_M_Software_Config,
	.WriteAndRead	= SPI_M_Software_WriteAndRead,
	.WriteThenRead	= SPI_M_Software_WriteThenRead,
	.Close			= SPI_M_Software_Close,
	.Open			= SPI_M_Software_Open,
	.Endp			= SPI_M_Software_Endp,
	.Mode			= SPI_M_Software_Mode,
};

void SPIx_Peripheral(uint8_t Bus)
{
	switch(Bus){
		case SPI2_M_BITBANG: SPI2_Master.Driver(&SPI2_Master);	break;
		default:break;
	}
}

void SPIx_Init(uint8_t Bus)
{
	switch(Bus){
		case SPI1_M_HARDWARE: SPI1_Master.Init(&SPI1_Master);break;
		case SPI2_M_BITBANG:  SPI2_Master.Init(&SPI2_Master);break;
		case SPI3_M_SOFTWARE: SPI3_Master.Init(&SPI3_Master);break;
		default:break;
	}
}

void SPIx_Mode(uint8_t Bus, spi_mode_e spi_mode)
{
	switch(Bus){
		case SPI1_M_HARDWARE: SPI1_Master.Mode(&SPI1_Master,spi_mode);break;
		case SPI2_M_BITBANG:  SPI2_Master.Mode(&SPI2_Master,spi_mode);break;
		case SPI3_M_SOFTWARE: SPI3_Master.Mode(&SPI3_Master,spi_mode);break;
		default:break;
	}
}


void SPIx_Close(uint8_t Bus, uint8_t CSx)
{
	switch(Bus){
		case SPI1_M_HARDWARE: SPI1_Master.Close(&SPI1_Master, CSx);break;
		case SPI2_M_BITBANG:  SPI2_Master.Close(&SPI2_Master, CSx);break;
		case SPI3_M_SOFTWARE: SPI3_Master.Close(&SPI3_Master, CSx);break;
		default:break;
	}
}

uint8_t SPIx_Open(uint8_t Bus, uint8_t CSx)
{
	switch(Bus){
		case SPI1_M_HARDWARE: return SPI1_Master.Open(&SPI1_Master, CSx);
		case SPI2_M_BITBANG:  return SPI2_Master.Open(&SPI2_Master, CSx);
		case SPI3_M_SOFTWARE: return SPI3_Master.Open(&SPI3_Master, CSx);
		default:return CORE_ERROR;
	}
}

uint8_t SPIx_Endp(uint8_t Bus)
{
	switch(Bus){
		case SPI1_M_HARDWARE: return SPI1_Master.Endp(&SPI1_Master);
		case SPI2_M_BITBANG:  return SPI2_Master.Endp(&SPI2_Master);
		case SPI3_M_SOFTWARE: return SPI3_Master.Endp(&SPI3_Master);
		default:return CORE_DONE;
	}
}


void SPIx_WriteAndRead(uint8_t Bus, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	switch(Bus){
		case SPI1_M_HARDWARE: SPI1_Master.WriteAndRead(&SPI1_Master,pTxData,pRxData,Size);break;
		case SPI2_M_BITBANG:  SPI2_Master.WriteAndRead(&SPI2_Master,pTxData,pRxData,Size);break;
		case SPI3_M_SOFTWARE: SPI3_Master.WriteAndRead(&SPI3_Master,pTxData,pRxData,Size);break;
		default:break;
	}
}

void SPIx_WriteThenRead(uint8_t Bus, uint8_t *pTxData, uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize)
{
	switch(Bus){
		case SPI1_M_HARDWARE: SPI1_Master.WriteThenRead(&SPI1_Master,pTxData,TxSize,pRxData,RxSize);break;
		case SPI2_M_BITBANG:  SPI2_Master.WriteThenRead(&SPI2_Master,pTxData,TxSize,pRxData,RxSize);break;
		case SPI3_M_SOFTWARE: SPI3_Master.WriteThenRead(&SPI3_Master,pTxData,TxSize,pRxData,RxSize);break;
		default:break;
	}
}


