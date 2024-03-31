#include "w25qx.h"

W25qx_Driver_t W25qx = {
	.DEV_ID			= W25Q64,
	.Init   		= W25QXX_Init,
	.Read   		= W25QXX_Read,
	.Write  		= W25QXX_Write,
	.NoCheckWrite 	= W25QXX_Write_NoCheck,
	.Erase_Sector  	= W25QXX_Erase_Sector,
	.Erase_Block    = W25QXX_Erase_Block,
};


void w25qxx_driver_pc_reset(W25qx_Driver_t *W25qx_Driver)
{
	W25qx_Driver->Read_PC = 0;
	W25qx_Driver->Busy_PC = 0;
	W25qx_Driver->Enable_PC = 0;
	W25qx_Driver->Erase_PC = 0;
	W25qx_Driver->Write_Page_PC = 0;
	W25qx_Driver->Write_NoCheck_PC = 0;
	W25qx_Driver->Write_PC = 0;
	W25qx_Driver->ret = CORE_SUCCESS;
}

uint8_t W25QXX_Read(W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)   
{
	enum
	{
		W25QXX_READ_GET_PARAM,
		W25QXX_READ_SET_OPEN_BUS,
		W25QXX_READ_SET_WR_DATA,
		W25QXX_READ_WAIT_DONE,
		W25QXX_READ_PROC_SUM,
	}; 
	static const void *function[W25QXX_READ_PROC_SUM] = {
		[W25QXX_READ_GET_PARAM] 	= &&W25QXX_READ_GET_PARAM,
		[W25QXX_READ_SET_OPEN_BUS] 	= &&W25QXX_READ_SET_OPEN_BUS,
		
		[W25QXX_READ_SET_WR_DATA] 	= &&W25QXX_READ_SET_WR_DATA,
		[W25QXX_READ_WAIT_DONE] 	= &&W25QXX_READ_WAIT_DONE,
	};
	goto *function[W25qx_Driver->Read_PC];
W25QXX_READ_GET_PARAM:
	switch( W25qx_Driver->DEV_ID){
		case W25Q10: W25qx_Driver->BlockCount = 2;		break;
		case W25Q20: W25qx_Driver->BlockCount = 4;		break;
		case W25Q40: W25qx_Driver->BlockCount = 8;		break;
		case W25Q80: W25qx_Driver->BlockCount = 16;		break;
		case W25Q16: W25qx_Driver->BlockCount = 32;		break;
		case W25Q32: W25qx_Driver->BlockCount = 64;		break;
		case W25Q64: W25qx_Driver->BlockCount = 128;	break;
		case W25Q128:W25qx_Driver->BlockCount = 256;	break;
		case W25Q256:W25qx_Driver->BlockCount = 512;	break;
		case W25Q512:W25qx_Driver->BlockCount = 1024;	break;
		default: W25qx_Driver->BlockCount = 0;			break;
	}
	W25qx_Driver->MAX_ADDR =  W25qx_Driver->BlockCount*16*4096;
	
	if( pBuffer == NULL || NumByteToRead == 0 || (ReadAddr+NumByteToRead) > W25qx_Driver->MAX_ADDR){
		return CORE_DONE;
	}
	
	W25qx_Driver->Read_PC = W25QXX_READ_SET_OPEN_BUS;
	return CORE_RUNNING;
W25QXX_READ_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Read_PC = W25QXX_READ_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_READ_SET_WR_DATA:
	W25qx_Driver->Cmdbuf[0] = W25X_ReadData;
	W25qx_Driver->Cmdbuf[1] = (uint8_t)((ReadAddr)>>16);
	W25qx_Driver->Cmdbuf[2] = (uint8_t)((ReadAddr)>>8);
	W25qx_Driver->Cmdbuf[3] = (uint8_t)ReadAddr;
	W25qx_Driver->SPI_Driver->write_then_read(W25qx_Driver->SPI_Driver,W25qx_Driver->Cmdbuf, 4, pBuffer, NumByteToRead);
	W25qx_Driver->Read_PC = W25QXX_READ_WAIT_DONE;
	return CORE_RUNNING;
W25QXX_READ_WAIT_DONE:
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) != CORE_DONE){
		return CORE_RUNNING;
	}
	W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID); 	 
	W25qx_Driver->Read_PC = W25QXX_READ_GET_PARAM;
	return CORE_DONE;
}  


//等待空闲
uint8_t W25QXX_Wait_Busy(W25qx_Driver_t *W25qx_Driver)   
{
	enum
	{
		W25QXX_BUSY_SET_OPEN_BUS,
		W25QXX_BUSY_SET_WR_DATA,
		W25QXX_BUSY_WAIT_DONE,
		W25QXX_BUSY_WAIT_READ_REG,
		W25QXX_BUSY_WAIT_READ_REG_DONE,
		W25QXX_BUSY_PROC_SUM
	}; 
	static const void *function[W25QXX_BUSY_PROC_SUM] = {
		[W25QXX_BUSY_SET_OPEN_BUS] 	= &&W25QXX_BUSY_SET_OPEN_BUS,
		[W25QXX_BUSY_SET_WR_DATA] 	= &&W25QXX_BUSY_SET_WR_DATA,
		[W25QXX_BUSY_WAIT_DONE] 	= &&W25QXX_BUSY_WAIT_DONE,
		[W25QXX_BUSY_WAIT_READ_REG] 	= &&W25QXX_BUSY_WAIT_READ_REG,
		[W25QXX_BUSY_WAIT_READ_REG_DONE]= &&W25QXX_BUSY_WAIT_READ_REG_DONE,
	};
	goto *function[W25qx_Driver->Busy_PC];
W25QXX_BUSY_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Busy_PC = W25QXX_BUSY_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_BUSY_SET_WR_DATA:
	W25qx_Driver->Cmdbuf[0] = W25X_ReadStatusReg1;
	W25qx_Driver->SPI_Driver->write_and_read(W25qx_Driver->SPI_Driver,W25qx_Driver->Cmdbuf, NULL, 1);
	W25qx_Driver->Busy_PC = W25QXX_BUSY_WAIT_DONE;
	return CORE_RUNNING;
W25QXX_BUSY_WAIT_DONE:
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) == CORE_DONE){
		W25qx_Driver->Busy_PC = W25QXX_BUSY_WAIT_READ_REG;
		W25qx_Driver->Busy_TIMEOUT = 3000;
	}
	return CORE_RUNNING;
W25QXX_BUSY_WAIT_READ_REG:
	W25qx_Driver->Busy_TIMEOUT--;
	W25qx_Driver->SPI_Driver->write_and_read(W25qx_Driver->SPI_Driver,NULL, &W25qx_Driver->Cmdbuf[0], 1);
	W25qx_Driver->Busy_PC = W25QXX_BUSY_WAIT_READ_REG_DONE;
	return CORE_RUNNING;
W25QXX_BUSY_WAIT_READ_REG_DONE:	
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) != CORE_DONE){
		return CORE_RUNNING;
	}
	if((W25qx_Driver->Cmdbuf[0] & 0x01 ) == 0x00){
		W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID);
		W25qx_Driver->Busy_PC = W25QXX_BUSY_SET_OPEN_BUS;
		W25qx_Driver->ret = CORE_SUCCESS;
		return CORE_DONE;
	}else if(W25qx_Driver->Busy_TIMEOUT == 0){
		W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID);
		W25qx_Driver->Busy_PC = W25QXX_BUSY_SET_OPEN_BUS;
		W25qx_Driver->ret = CORE_ERROR;
		return CORE_DONE;
	}else{
		W25qx_Driver->Busy_PC = W25QXX_BUSY_WAIT_READ_REG;
	}
	return CORE_RUNNING;
} 


//W25QXX写使能	
//将WEL置位   
uint8_t W25QXX_Write_Enable(W25qx_Driver_t *W25qx_Driver)   
{
	enum
	{
		W25QXX_ENABLE_SET_OPEN_BUS,
		W25QXX_ENABLE_SET_WR_DATA,
		W25QXX_ENABLE_WAIT_DONE,
		W25QXX_ENABLE_PROC_SUM
	}; 
	static const void *function[W25QXX_ENABLE_PROC_SUM] = {
		[W25QXX_ENABLE_SET_OPEN_BUS] 	= &&W25QXX_ENABLE_SET_OPEN_BUS,
		[W25QXX_ENABLE_SET_WR_DATA] 	= &&W25QXX_ENABLE_SET_WR_DATA,
		[W25QXX_ENABLE_WAIT_DONE] 		= &&W25QXX_ENABLE_WAIT_DONE,
	};
	goto *function[W25qx_Driver->Enable_PC];
W25QXX_ENABLE_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Enable_PC = W25QXX_ENABLE_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_ENABLE_SET_WR_DATA:
	W25qx_Driver->Cmdbuf[0] = W25X_WriteEnable;
	W25qx_Driver->SPI_Driver->write_and_read(W25qx_Driver->SPI_Driver,W25qx_Driver->Cmdbuf, NULL, 1);
	W25qx_Driver->Enable_PC = W25QXX_ENABLE_WAIT_DONE;
	return CORE_RUNNING;
W25QXX_ENABLE_WAIT_DONE:
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) != CORE_DONE){
		return CORE_RUNNING;
	}
	W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID);
	W25qx_Driver->Enable_PC = W25QXX_ENABLE_SET_OPEN_BUS;
	return CORE_DONE;    
}


//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
uint8_t W25QXX_Erase_Sector(W25qx_Driver_t *W25qx_Driver,uint32_t Dst_Addr)   
{  
	enum
	{
		W25QXX_ERASE_GET_PARAM,
		W25QXX_ERASE_SET_OPEN_BUS,
		W25QXX_ERASE_SET_WR_DATA,
		W25QXX_ERASE_WAIT_DONE,
		W25QXX_ERASE_BUSY_DONE,
		W25QXX_ERASE_PROC_SUM,
	}; 
	static const void *function[W25QXX_ERASE_PROC_SUM] = {
		[W25QXX_ERASE_GET_PARAM] 	= &&W25QXX_ERASE_GET_PARAM,
		[W25QXX_ERASE_SET_OPEN_BUS] 	= &&W25QXX_ERASE_SET_OPEN_BUS,
		
		[W25QXX_ERASE_SET_WR_DATA] 	= &&W25QXX_ERASE_SET_WR_DATA,
		[W25QXX_ERASE_WAIT_DONE] 	= &&W25QXX_ERASE_WAIT_DONE,
		[W25QXX_ERASE_BUSY_DONE] 	= &&W25QXX_ERASE_BUSY_DONE,
	};
	goto *function[W25qx_Driver->Erase_PC];
W25QXX_ERASE_GET_PARAM:
	if( W25QXX_Write_Enable(W25qx_Driver) == CORE_DONE){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_SET_OPEN_BUS;
	}
	return CORE_RUNNING;
W25QXX_ERASE_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_ERASE_SET_WR_DATA:
	Dst_Addr*=4096;	
	W25qx_Driver->Cmdbuf[0] = W25X_SectorErase;
	W25qx_Driver->Cmdbuf[1] = (uint8_t)((Dst_Addr)>>16);
	W25qx_Driver->Cmdbuf[2] = (uint8_t)((Dst_Addr)>>8);
	W25qx_Driver->Cmdbuf[3] = (uint8_t)Dst_Addr;
	W25qx_Driver->SPI_Driver->write_and_read(&SPI_Driver0,W25qx_Driver->Cmdbuf, NULL,4);
	W25qx_Driver->Erase_PC = W25QXX_ERASE_WAIT_DONE;
	return CORE_RUNNING;
W25QXX_ERASE_WAIT_DONE:
	if( W25qx_Driver->SPI_Driver->endp(&SPI_Driver0) != CORE_DONE){
		return CORE_RUNNING;
	}
	W25qx_Driver->SPI_Driver->close(&SPI_Driver0,W25qx_Driver->BUS_ID); 
	W25qx_Driver->Erase_PC = W25QXX_ERASE_BUSY_DONE;
	return CORE_RUNNING;
W25QXX_ERASE_BUSY_DONE://等待擦除完成
	if( W25QXX_Wait_Busy(W25qx_Driver) == CORE_DONE ){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_GET_PARAM;
		return CORE_DONE;
	}
	return CORE_RUNNING;
} 

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
uint8_t W25QXX_Erase_Block(W25qx_Driver_t *W25qx_Driver,uint32_t Dst_Addr)   
{  
	enum
	{
		W25QXX_ERASE_Block_GET_PARAM,
		W25QXX_ERASE_Block_SET_OPEN_BUS,
		W25QXX_ERASE_Block_SET_WR_DATA,
		W25QXX_ERASE_Block_WAIT_DONE,
		W25QXX_ERASE_Block_BUSY_DONE,
		W25QXX_ERASE_Block_PROC_SUM,
	}; 
	static const void *function[W25QXX_ERASE_Block_PROC_SUM] = {
		[W25QXX_ERASE_Block_GET_PARAM] 		= &&W25QXX_ERASE_Block_GET_PARAM,
		[W25QXX_ERASE_Block_SET_OPEN_BUS] 	= &&W25QXX_ERASE_Block_SET_OPEN_BUS,
		
		[W25QXX_ERASE_Block_SET_WR_DATA] 	= &&W25QXX_ERASE_Block_SET_WR_DATA,
		[W25QXX_ERASE_Block_WAIT_DONE] 		= &&W25QXX_ERASE_Block_WAIT_DONE,
		[W25QXX_ERASE_Block_BUSY_DONE] 		= &&W25QXX_ERASE_Block_BUSY_DONE,
	};
	goto *function[W25qx_Driver->Erase_PC];
W25QXX_ERASE_Block_GET_PARAM:
	if( W25QXX_Write_Enable(W25qx_Driver) == CORE_DONE){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_Block_SET_OPEN_BUS;
	}
	return CORE_RUNNING;
W25QXX_ERASE_Block_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_Block_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_ERASE_Block_SET_WR_DATA:
	Dst_Addr*=65536;	
	W25qx_Driver->Cmdbuf[0] = W25X_BlockErase;
	W25qx_Driver->Cmdbuf[1] = (uint8_t)((Dst_Addr)>>16);
	W25qx_Driver->Cmdbuf[2] = (uint8_t)((Dst_Addr)>>8);
	W25qx_Driver->Cmdbuf[3] = (uint8_t)Dst_Addr;
	W25qx_Driver->SPI_Driver->write_and_read(&SPI_Driver0,W25qx_Driver->Cmdbuf, NULL,4);
	W25qx_Driver->Erase_PC = W25QXX_ERASE_Block_WAIT_DONE;
	return CORE_RUNNING;
W25QXX_ERASE_Block_WAIT_DONE:
	if( W25qx_Driver->SPI_Driver->endp(&SPI_Driver0) != CORE_DONE){
		return CORE_RUNNING;
	}
	W25qx_Driver->SPI_Driver->close(&SPI_Driver0,W25qx_Driver->BUS_ID); 
	W25qx_Driver->Erase_PC = W25QXX_ERASE_Block_BUSY_DONE;
	return CORE_RUNNING;
W25QXX_ERASE_Block_BUSY_DONE://等待擦除完成
	if( W25QXX_Wait_Busy(W25qx_Driver) == CORE_DONE ){
		W25qx_Driver->Erase_PC = W25QXX_ERASE_Block_GET_PARAM;
		return CORE_DONE;
	}
	return CORE_RUNNING;
} 
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
uint8_t W25QXX_Write_Page(W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	enum
	{
		W25QXX_Write_Page_GET_PARAM,
		W25QXX_Write_Page_SET_OPEN_BUS,
		W25QXX_Write_Page_SET_WR_ADDR,
		W25QXX_Write_Page_WAIT_ADDR_DONE,
		W25QXX_Write_Page_SET_WR_DATA,
		W25QXX_Write_Page_WAIT_DATA_DONE,
		W25QXX_Write_Page_BUSY_DONE,
		W25QXX_Write_Page_PROC_SUM,
	}; 
	static const void *function[W25QXX_Write_Page_PROC_SUM] = {
		[W25QXX_Write_Page_GET_PARAM] 	= &&W25QXX_Write_Page_GET_PARAM,
		[W25QXX_Write_Page_SET_OPEN_BUS]= &&W25QXX_Write_Page_SET_OPEN_BUS,
		
		[W25QXX_Write_Page_SET_WR_ADDR] = &&W25QXX_Write_Page_SET_WR_ADDR,
		[W25QXX_Write_Page_WAIT_ADDR_DONE]= &&W25QXX_Write_Page_WAIT_ADDR_DONE,
		
		[W25QXX_Write_Page_SET_WR_DATA]= &&W25QXX_Write_Page_SET_WR_DATA,
		[W25QXX_Write_Page_WAIT_DATA_DONE]= &&W25QXX_Write_Page_WAIT_DATA_DONE,
		
		[W25QXX_Write_Page_BUSY_DONE] 	= &&W25QXX_Write_Page_BUSY_DONE,
	};
	goto *function[W25qx_Driver->Write_Page_PC];
W25QXX_Write_Page_GET_PARAM:
	if( W25QXX_Write_Enable(W25qx_Driver) == CORE_DONE){
		W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_SET_OPEN_BUS;
	}
	return CORE_RUNNING;
W25QXX_Write_Page_SET_OPEN_BUS:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_SET_WR_ADDR;
	}
	return CORE_RUNNING;
W25QXX_Write_Page_SET_WR_ADDR:
	W25qx_Driver->Cmdbuf[0] = W25X_PageProgram;
	W25qx_Driver->Cmdbuf[1] = (uint8_t)((WriteAddr)>>16);
	W25qx_Driver->Cmdbuf[2] = (uint8_t)((WriteAddr)>>8);
	W25qx_Driver->Cmdbuf[3] = (uint8_t)WriteAddr;
	W25qx_Driver->SPI_Driver->write_and_read(W25qx_Driver->SPI_Driver,W25qx_Driver->Cmdbuf, NULL, 4);
	W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_WAIT_ADDR_DONE;
	return CORE_RUNNING;
W25QXX_Write_Page_WAIT_ADDR_DONE:
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) == CORE_DONE){
		W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_SET_WR_DATA;
	}
	return CORE_RUNNING;
W25QXX_Write_Page_SET_WR_DATA:
	W25qx_Driver->SPI_Driver->write_and_read(W25qx_Driver->SPI_Driver,pBuffer, NULL, NumByteToWrite);
	W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_WAIT_DATA_DONE;
	return CORE_RUNNING;
W25QXX_Write_Page_WAIT_DATA_DONE:	
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) == CORE_DONE){
		W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID);
		W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_BUSY_DONE;
	}
	return CORE_RUNNING;
W25QXX_Write_Page_BUSY_DONE://等待擦除完成
	if( W25QXX_Wait_Busy(W25qx_Driver) == CORE_DONE ){
		W25qx_Driver->Write_Page_PC = W25QXX_Write_Page_GET_PARAM;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
uint8_t W25QXX_Write_NoCheck(W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{
	enum
	{
		W25QXX_Write_NoCheck_GET_PARAM,
		W25QXX_Write_NoCheck_Write_Page,
		W25QXX_Write_NoCheck_REMAIN_CMP,
		W25QXX_Write_NoCheck_PROC_SUM,
	}; 
	static const void *function[W25QXX_Write_NoCheck_PROC_SUM] = {
		[W25QXX_Write_NoCheck_GET_PARAM] 	= &&W25QXX_Write_NoCheck_GET_PARAM,
		[W25QXX_Write_NoCheck_Write_Page]= &&W25QXX_Write_NoCheck_Write_Page,
		[W25QXX_Write_NoCheck_REMAIN_CMP] = &&W25QXX_Write_NoCheck_REMAIN_CMP,
	};
	goto *function[W25qx_Driver->Write_NoCheck_PC];
W25QXX_Write_NoCheck_GET_PARAM:
	W25qx_Driver->NoCheck_pBuffer = pBuffer;
	W25qx_Driver->NoCheck_WriteAddr = WriteAddr;
	W25qx_Driver->NoCheck_NumByteToWrite = NumByteToWrite;
	W25qx_Driver->pageremain=256-W25qx_Driver->NoCheck_WriteAddr%256; //单页剩余的字节数
	if(W25qx_Driver->NoCheck_NumByteToWrite<=W25qx_Driver->pageremain){
		W25qx_Driver->pageremain=W25qx_Driver->NoCheck_NumByteToWrite;//不大于256个字节
	}
	W25qx_Driver->Write_NoCheck_PC = W25QXX_Write_NoCheck_Write_Page;
	return CORE_RUNNING;
W25QXX_Write_NoCheck_Write_Page:
	if( W25QXX_Write_Page(W25qx_Driver,W25qx_Driver->NoCheck_pBuffer,W25qx_Driver->NoCheck_WriteAddr,W25qx_Driver->pageremain) == CORE_DONE){
		W25qx_Driver->Write_NoCheck_PC = W25QXX_Write_NoCheck_REMAIN_CMP;
	}
	return CORE_RUNNING;
W25QXX_Write_NoCheck_REMAIN_CMP:
	if(W25qx_Driver->NoCheck_NumByteToWrite==W25qx_Driver->pageremain){//写入结束了
		W25qx_Driver->Write_NoCheck_PC = W25QXX_Write_NoCheck_GET_PARAM;
		return CORE_DONE;
	}else{ //NumByteToWrite>pageremain
		W25qx_Driver->NoCheck_pBuffer += W25qx_Driver->pageremain;
		W25qx_Driver->NoCheck_WriteAddr += W25qx_Driver->pageremain;	
		W25qx_Driver->NoCheck_NumByteToWrite-=W25qx_Driver->pageremain;			  //减去已经写入了的字节数
		if(W25qx_Driver->NoCheck_NumByteToWrite>256){
			W25qx_Driver->pageremain=256; //一次可以写入256个字节
		}else{
			W25qx_Driver->pageremain=W25qx_Driver->NoCheck_NumByteToWrite; 	  //不够256个字节了
		}
	}
	W25qx_Driver->Write_NoCheck_PC = W25QXX_Write_NoCheck_Write_Page;
	return CORE_RUNNING;
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
uint8_t W25QXX_Write(W25qx_Driver_t *W25qx_Driver,uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	enum{
		W25QXX_Write_GET_MAX_ADDR,
		W25QXX_Write_GET_PARAM,
		W25QXX_Write_SECTOR_READ,
		
		W25QXX_Write_SECTOR_READ_CMP,
		W25QXX_Write_ERASE_DATA_CMP,
		W25QXX_Write_NO_ERASE_DATA,
		W25QXX_Write_ERASE_DATA,
		W25QXX_Write_DATA_COPY,
		W25QXX_Write_NoCheck_REMAIN,
		W25QXX_Write_DONE_CMP,
		W25QXX_Write_SUM,
	};
	static const void *function[W25QXX_Write_SUM] = {
		[W25QXX_Write_GET_MAX_ADDR] 	= &&W25QXX_Write_GET_MAX_ADDR,
		[W25QXX_Write_GET_PARAM] 		= &&W25QXX_Write_GET_PARAM,
		[W25QXX_Write_SECTOR_READ]		= &&W25QXX_Write_SECTOR_READ,
		[W25QXX_Write_SECTOR_READ_CMP] 	= &&W25QXX_Write_SECTOR_READ_CMP,
		[W25QXX_Write_ERASE_DATA_CMP] 	= &&W25QXX_Write_ERASE_DATA_CMP,
		[W25QXX_Write_NO_ERASE_DATA]	= &&W25QXX_Write_NO_ERASE_DATA,
		[W25QXX_Write_ERASE_DATA] 		= &&W25QXX_Write_ERASE_DATA,
		[W25QXX_Write_DATA_COPY] 		= &&W25QXX_Write_DATA_COPY,
		[W25QXX_Write_NoCheck_REMAIN]	= &&W25QXX_Write_NoCheck_REMAIN,
		[W25QXX_Write_DONE_CMP] 		= &&W25QXX_Write_DONE_CMP,
	};
	goto *function[W25qx_Driver->Write_PC];
W25QXX_Write_GET_MAX_ADDR:
	switch( W25qx_Driver->DEV_ID){
		case W25Q10: W25qx_Driver->BlockCount = 2;		break;
		case W25Q20: W25qx_Driver->BlockCount = 4;		break;
		case W25Q40: W25qx_Driver->BlockCount = 8;		break;
		case W25Q80: W25qx_Driver->BlockCount = 16;		break;
		case W25Q16: W25qx_Driver->BlockCount = 32;		break;
		case W25Q32: W25qx_Driver->BlockCount = 64;		break;
		case W25Q64: W25qx_Driver->BlockCount = 128;	break;
		case W25Q128:W25qx_Driver->BlockCount = 256;	break;
		case W25Q256:W25qx_Driver->BlockCount = 512;	break;
		case W25Q512:W25qx_Driver->BlockCount = 1024;	break;
		default: W25qx_Driver->BlockCount = 0;			break;
	}
	W25qx_Driver->MAX_ADDR =  W25qx_Driver->BlockCount*16*4096;
	
	if( pBuffer == NULL || NumByteToWrite == 0 || (WriteAddr+NumByteToWrite) > W25qx_Driver->MAX_ADDR){
		return CORE_DONE;
	}
	W25qx_Driver->Write_PC = W25QXX_Write_GET_PARAM;
	return CORE_RUNNING;
W25QXX_Write_GET_PARAM:
	W25qx_Driver->WriteAddr 		= WriteAddr;
	W25qx_Driver->pBuffer   		= pBuffer;
	W25qx_Driver->NumByteToWrite 	= NumByteToWrite;
	W25qx_Driver->secpos	   		= W25qx_Driver->WriteAddr/4096;//扇区地址  
	W25qx_Driver->secoff	   		= W25qx_Driver->WriteAddr%4096;//在扇区内的偏移
	W25qx_Driver->secremain 		= 4096 - W25qx_Driver->secoff;
	if( W25qx_Driver->NumByteToWrite <= W25qx_Driver->secremain){
		W25qx_Driver->secremain = W25qx_Driver->NumByteToWrite;
	}
	W25qx_Driver->Write_PC = W25QXX_Write_SECTOR_READ;
	return CORE_RUNNING;
W25QXX_Write_SECTOR_READ:
	if( W25QXX_Read(W25qx_Driver, W25qx_Driver->buf,W25qx_Driver->secpos*4096,4096) == CORE_DONE){//读出整个扇区的内容
		W25qx_Driver->i = 0;
		W25qx_Driver->Write_PC = W25QXX_Write_SECTOR_READ_CMP;
	}
	return CORE_RUNNING;
W25QXX_Write_SECTOR_READ_CMP:
	if( W25qx_Driver->i < W25qx_Driver->secremain){
		if(W25qx_Driver->buf[W25qx_Driver->secoff+W25qx_Driver->i]!=0XFF){
			W25qx_Driver->Write_PC = W25QXX_Write_ERASE_DATA_CMP;//需要擦除
		}else{
			W25qx_Driver->i++;		
		}
	}else{//循环判断结束
		W25qx_Driver->Write_PC = W25QXX_Write_ERASE_DATA_CMP;
	}
	return CORE_RUNNING;
W25QXX_Write_ERASE_DATA_CMP:
	if( W25qx_Driver->i < W25qx_Driver->secremain){ //需要擦除
		W25qx_Driver->Write_PC = W25QXX_Write_ERASE_DATA;//需要擦除
	}else{ //不需要擦除
		W25qx_Driver->Write_PC = W25QXX_Write_NO_ERASE_DATA;//不需要擦除
	}
	return CORE_RUNNING;
W25QXX_Write_NO_ERASE_DATA://写已经擦除了的,直接写入扇区剩余区间.
	if(W25QXX_Write_NoCheck(W25qx_Driver, W25qx_Driver->pBuffer,W25qx_Driver->WriteAddr,W25qx_Driver->secremain) == CORE_DONE){
		W25qx_Driver->Write_PC = W25QXX_Write_DONE_CMP;
	}
	return CORE_RUNNING;
W25QXX_Write_ERASE_DATA:
	if(W25QXX_Erase_Sector(W25qx_Driver, W25qx_Driver->secpos) == CORE_DONE){//擦除这个扇区
		W25qx_Driver->i = 0;
		W25qx_Driver->Write_PC = W25QXX_Write_DATA_COPY;
	}
	return CORE_RUNNING;
W25QXX_Write_DATA_COPY://复制
	if( W25qx_Driver->i < W25qx_Driver->secremain){
		W25qx_Driver->buf[W25qx_Driver->i+W25qx_Driver->secoff]=W25qx_Driver->pBuffer[W25qx_Driver->i];
		W25qx_Driver->i++;		
	}else{
		W25qx_Driver->Write_PC = W25QXX_Write_NoCheck_REMAIN;
	}
	return CORE_RUNNING;
W25QXX_Write_NoCheck_REMAIN:
	if( W25QXX_Write_NoCheck(W25qx_Driver, W25qx_Driver->buf,W25qx_Driver->secpos*4096,4096) == CORE_DONE){//写入整个扇区 
		W25qx_Driver->Write_PC = W25QXX_Write_DONE_CMP;
	}
	return CORE_RUNNING;
W25QXX_Write_DONE_CMP:
	if(W25qx_Driver->NumByteToWrite==W25qx_Driver->secremain){//写入结束了
		W25qx_Driver->Write_PC = W25QXX_Write_GET_MAX_ADDR;
		return CORE_DONE;
	}else{
		W25qx_Driver->secpos++;//扇区地址增1
		W25qx_Driver->secoff=0;//偏移位置为0 	 

		W25qx_Driver->pBuffer += W25qx_Driver->secremain;  //指针偏移
		W25qx_Driver->WriteAddr+=W25qx_Driver->secremain;//写地址偏移	   
		W25qx_Driver->NumByteToWrite-=W25qx_Driver->secremain;//字节数递减
		if(W25qx_Driver->NumByteToWrite>4096){
			W25qx_Driver->secremain=4096;	//下一个扇区还是写不完
		}else {
			W25qx_Driver->secremain=W25qx_Driver->NumByteToWrite;			//下一个扇区可以写完了
		}
	}
	W25qx_Driver->Write_PC = W25QXX_Write_SECTOR_READ;
	return CORE_RUNNING;
}


uint8_t w25qxx_ID_Manufacturer(W25qx_Driver_t *W25qx_Driver)
{
	enum
	{
		W25QXX_READ_ID_PARAM,
		W25QXX_READ_ID_Page,
		W25QXX_READ_ID_CMP,
		W25QXX_READ_ID_SUM,
	}; 
	static const void *function[W25QXX_READ_ID_SUM] = {
		[W25QXX_READ_ID_PARAM] 	= &&W25QXX_READ_ID_PARAM,
		[W25QXX_READ_ID_Page]	= &&W25QXX_READ_ID_Page,
		[W25QXX_READ_ID_CMP] 	= &&W25QXX_READ_ID_CMP,
	};
	goto *function[W25qx_Driver->DEV_ID_PC];
W25QXX_READ_ID_PARAM:
	if( W25qx_Driver->SPI_Driver->open(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID) == CORE_SUCCESS){
		W25qx_Driver->DEV_ID_PC = W25QXX_READ_ID_Page;
	}
	return CORE_RUNNING;
W25QXX_READ_ID_Page:
	W25qx_Driver->Cmdbuf[0] = W25X_JedecDeviceID;
	W25qx_Driver->SPI_Driver->write_then_read(W25qx_Driver->SPI_Driver,W25qx_Driver->Cmdbuf, 1, W25qx_Driver->buf, 3);
	W25qx_Driver->DEV_ID_PC = W25QXX_READ_ID_CMP;
	return CORE_RUNNING;
W25QXX_READ_ID_CMP:
	if( W25qx_Driver->SPI_Driver->endp(W25qx_Driver->SPI_Driver) != CORE_DONE){
		return CORE_RUNNING;
	}
	W25qx_Driver->DEV_ID_Value = W25qx_Driver->buf[0]<<16 | W25qx_Driver->buf[1]<<8 | W25qx_Driver->buf[2];
	switch(W25qx_Driver->DEV_ID_Value)
	{
		case W25Q64: 	W25qx_Driver->DEV_ID = W25Q64; break;
		default: 		W25qx_Driver->DEV_ID = W25Q64; break;
	};
	
	W25qx_Driver->SPI_Driver->close(W25qx_Driver->SPI_Driver,W25qx_Driver->BUS_ID); 	 
	W25qx_Driver->DEV_ID_PC = W25QXX_READ_ID_PARAM;
	return CORE_DONE;
}

void W25QXX_Init(W25qx_Driver_t *W25qx_Driver)
{
	W25qx_Driver->SPI_Driver  = &SPI_Driver0;
	W25qx_Driver->BUS_ID 	  = 0;
	w25qxx_driver_pc_reset(W25qx_Driver);
	while( w25qxx_ID_Manufacturer(W25qx_Driver) != CORE_DONE);
}



void w25qxx_ymodem_write(uint32_t addr, uint8_t *buffer, uint32_t length)
{
	if( W25qx.ret == CORE_ERROR){
		w25qxx_driver_pc_reset(&W25qx);
		return;
	}
	while(W25QXX_Write_NoCheck(&W25qx,buffer,addr,length) != CORE_DONE)
	{
		if( W25qx.ret == CORE_ERROR)
		{
			w25qxx_driver_pc_reset(&W25qx);
			return;
		}
	}
}

void w25qxx_ymodem_erase(uint32_t addr, uint32_t ByteLen)
{
	int start_page = 0;
	int end_page = 0;
	int start_block = 0;
	int end_block = 0;
	W25qx.ret = CORE_SUCCESS;
	start_page = addr/4096;
	end_page = (ByteLen+addr)/4096;
	
	start_block = addr/65536;
	end_block = (ByteLen+addr)/65536;
	
	if( start_block == end_block){//说明在一个块，一个块有16个扇区
		for( int i = start_page; i <= end_page; i++){
			while( W25qx.Erase_Sector(&W25qx,i) != CORE_DONE)
			{
				if( W25qx.ret == CORE_ERROR)
				{
					w25qxx_driver_pc_reset(&W25qx);
					return;
				}
			}
		}
	}else{//不在一个块，两个块中或者多个块中
		//首先先将零散扇区内存擦除
		for( int i = start_page; i <= (start_block+1)*16; i++){
			while( W25qx.Erase_Sector(&W25qx,i) != CORE_DONE)
			{
				if( W25qx.ret == CORE_ERROR)
				{
					w25qxx_driver_pc_reset(&W25qx);
					return;
				}
			}
		}
		//开始擦除块内存
		start_block = start_block+1;
		if( start_block != end_block){
			for( int i = start_block; i < end_block; i++){
				while( W25qx.Erase_Block(&W25qx,i) != CORE_DONE)
				{
					if( W25qx.ret == CORE_ERROR)
					{
						w25qxx_driver_pc_reset(&W25qx);
						return;
					}
				}
			}
		}
		
		//最后将零散扇区内存擦除
		for( int i = end_block*16; i <= end_page; i++){
			while( W25qx.Erase_Sector(&W25qx,i) != CORE_DONE)
			{
				if( W25qx.ret == CORE_ERROR)
				{
					w25qxx_driver_pc_reset(&W25qx);
					return;
				}
			}
		}
	}
}




