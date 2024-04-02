#include "SPI_Config.h"


SPI_M_Bitbang_t SPI1_Master = {
	._CPOL = 0,
	._CPHA = 0,
	.Register.R12_Lock = CORE_UNLOCK,
	.PORT_SCK  = GPIOA,
	.PIN_SCK   = GPIO_Pin_5,
	.PORT_MISO = GPIOA,
	.PIN_MISO  = GPIO_Pin_6,
	.PORT_MOSI = GPIOA,
	.PIN_MOSI  = GPIO_Pin_7,
	.PORT_CS[0] = GPIOB,
	.PIN_CS[0]  = GPIO_Pin_9,
	.PORT_CS[1] = GPIOB,
	.PIN_CS[1]  = GPIO_Pin_4,
	
	.Init			= SPI_M_Bitbang_Config,
	.Driver			= SPI_M_Bitbang_Peripheral,
	.WriteAndRead	= SPI_M_Bitbang_WriteAndRead,
	.WriteThenRead	= SPI_M_Bitbang_WriteThenRead,
	.Close			= SPI_M_Bitbang_Close,
	.Open			= SPI_M_Bitbang_Open,
	.Endp			= SPI_M_Bitbang_Endp,
};


//void I2Cx_Peripheral(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C2_M_BITBANG: I2C2_Master.Driver(&I2C2_Master);	break;
//		case I2C4_S_BITBANG: I2C4_Slave.Driver(&I2C4_Slave);	break;
//		default:break;
//	}
//}

//void I2Cx_Init(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Init(&I2C1_Master);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Init(&I2C2_Master);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Init(&I2C3_Master);break;
//		case I2C4_S_BITBANG:  I2C4_Slave.Init(&I2C4_Slave);  break;
//		default:break;
//	}
//}


//void SPIx_Close(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Close(&I2C1_Master);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Close(&I2C2_Master);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Close(&I2C3_Master);break;
//		default:break;
//	}
//}

//uint8_t SPIx_Open(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: return I2C1_Master.Open(&I2C1_Master);
//		case I2C2_M_BITBANG:  return I2C2_Master.Open(&I2C2_Master);
//		case I2C3_M_SOFTWARE: return I2C3_Master.Open(&I2C3_Master);
//		default:return CORE_SUCCESS;
//	}
//}

//uint8_t SPIx_Endp(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: return I2C1_Master.Endp(&I2C1_Master);
//		case I2C2_M_BITBANG:  return I2C2_Master.Endp(&I2C2_Master);
//		case I2C3_M_SOFTWARE: return I2C3_Master.Endp(&I2C3_Master);
//		default:return CORE_SUCCESS;
//	}
//}

//uint8_t SPIx_Result(uint8_t Bus)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: return I2C1_Master.Result(&I2C1_Master);
//		case I2C2_M_BITBANG:  return I2C2_Master.Result(&I2C2_Master);
//		case I2C3_M_SOFTWARE: return I2C3_Master.Result(&I2C3_Master);
//		default:return CORE_SUCCESS;
//	}
//}

//void SPIx_Set(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Set(&I2C1_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		case I2C2_M_BITBANG: I2C2_Master.Set(&I2C2_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Set(&I2C3_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		default:break;
//	}
//}

//void SPIx_Get(uint8_t Bus, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Get(&I2C1_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Get(&I2C2_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Get(&I2C3_Master,Dev_Addr,Reg_Addr,Reg_Size,value,Size);break;
//		default:break;
//	}
//}

//void SPIx_Write(uint8_t Bus, uint8_t Dev_Addr, uint8_t *value, uint16_t Size)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Write(&I2C1_Master,Dev_Addr,value,Size);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Write(&I2C2_Master,Dev_Addr,value,Size);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Write(&I2C3_Master,Dev_Addr,value,Size);break;
//		default:break;
//	}
//}


//void SPIx_Read(uint8_t Bus, uint8_t Dev_Addr, uint8_t *value, uint16_t Size)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Read(&I2C1_Master,Dev_Addr,value,Size);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Read(&I2C2_Master,Dev_Addr,value,Size);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Read(&I2C3_Master,Dev_Addr,value,Size);break;
//		default:break;
//	}
//}

//void I2Cx_Detect(uint8_t Bus, uint8_t Dev_Addr)
//{
//	switch(Bus){
//		case I2C1_M_HARDWARE: I2C1_Master.Detect(&I2C1_Master,Dev_Addr);break;
//		case I2C2_M_BITBANG:  I2C2_Master.Detect(&I2C2_Master,Dev_Addr);break;
//		case I2C3_M_SOFTWARE: I2C3_Master.Detect(&I2C3_Master,Dev_Addr);break;
//		default:break;
//	}
//}

