#include "SPI_Software.h"











void SPI_M_Software_Config(SPI_M_Software_t *SPI_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/* CS */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[0];
	GPIO_Init(SPI_Driver->PORT_CS[0], &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_CS[1];
	GPIO_Init(SPI_Driver->PORT_CS[1], &GPIO_InitStructure);
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_SCK;
	GPIO_Init(SPI_Driver->PORT_SCK, &GPIO_InitStructure);
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MOSI;
	GPIO_Init(SPI_Driver->PORT_MOSI, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Pin = SPI_Driver->PIN_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SPI_Driver->PORT_MISO, &GPIO_InitStructure);

	//状态初始化
	if( SPI_Driver->_CPOL){
		GPIO_SET_HIGH(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿
	}else{
		GPIO_SET_LOW(SPI_Driver->PORT_SCK, SPI_Driver->PIN_SCK);//时钟 - 低 下降沿   
	}
	GPIO_SET_HIGH(SPI_Driver->PORT_MOSI,  SPI_Driver->PIN_MOSI);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[0], SPI_Driver->PIN_CS[0]);
	GPIO_SET_HIGH(SPI_Driver->PORT_CS[1], SPI_Driver->PIN_CS[1]);
}



void SPI_Delay(void)
{
	uint16_t cnt = 5;
	while(cnt--);
}



void SPI_Delay_Write_Read_Byte(SPI_M_Software_t *SPI_Driver,uint8_t *TxData)
{
	uint8_t RxData = 0;

	for(uint8_t mask=0x80; mask>0; mask>>=1)
	{
#if CPOL^CPHA
		SPI_SCK_HIGH;      //时钟 - 高
#else
		SPI_SCK_LOW;       //时钟 - 低 下降沿
#endif
		if(TxData & mask){ //发送数据
			SPI_MOSI_HIGH;
		}else{
			SPI_MOSI_LOW;
		}
		SPI_Delay();
#if CPOL^CPHA
		SPI_SCK_LOW;       //时钟 - 低
#else
		SPI_SCK_HIGH;      //时钟 - 高 上升沿
#endif
		if(SPI_MISO_READ){
			RxData |= mask;
		}else{
			RxData |= 0x00;
		}
		SPI_Delay();
	}	
#if CPOL
	SPI_SCK_HIGH;      //时钟 - 高
#else
	SPI_SCK_LOW;       //时钟 - 低 下降沿
#endif
	return RxData;
}


