#ifndef _W25Qx_H
#define _W25Qx_H

#include "core.h"
#include "SPI_Config.h"


typedef enum
{
	W25Q10,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64 = 0XEF4017,
	W25Q128,
	W25Q256,
	W25Q512,
	W25QXX,
}W25QXX_ID_t;


#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

typedef struct W25qx_Driver_t
{
	uint8_t ret;
	uint8_t Bus;
	uint8_t BUS_ID;
	W25QXX_ID_t DEV_ID;
	uint16_t BlockCount;
	uint32_t MAX_ADDR;
/*********************************************************/
	uint8_t Read_PC;//指令地址
	uint8_t Busy_PC;
	uint32_t Busy_TIMEOUT;
	uint8_t Enable_PC;
	uint8_t Erase_PC;
	uint8_t Write_Page_PC;
	uint8_t DEV_ID_PC;//指令地址
	uint32_t DEV_ID_Value;
/*********************************************************/
	uint8_t Write_NoCheck_PC;
	uint16_t pageremain;
	uint32_t NoCheck_WriteAddr;
	uint16_t NoCheck_NumByteToWrite;
	uint8_t* NoCheck_pBuffer;
/*********************************************************/	
	uint8_t  Write_PC;
	uint32_t WriteAddr;
	uint8_t* pBuffer;
	uint16_t NumByteToWrite;
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;   
/*********************************************************/	
	uint8_t Cmdbuf[4];
	uint8_t buf[4096];
	void (*Init)(struct W25qx_Driver_t *W25qx_Driver);
	void (*Test)(struct W25qx_Driver_t *W25qx_Driver);
	void (*Reset)(struct W25qx_Driver_t *W25qx_Driver);
	uint8_t (*Read)( struct W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
	uint8_t (*Write)(struct W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer, uint32_t WriteAddr,uint16_t NumByteToWrite);
	uint8_t (*NoCheckWrite)(struct W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer, uint32_t WriteAddr,uint16_t NumByteToWrite);
	uint8_t (*Erase_Sector)(struct W25qx_Driver_t *W25qx_Driver, uint32_t Dst_Addr);
	uint8_t (*Erase_Block)(struct W25qx_Driver_t *W25qx_Driver, uint32_t Dst_Addr);
}W25qx_Driver_t;

extern W25qx_Driver_t W25qx;

void W25QXX_Init(W25qx_Driver_t *W25qx_Driver);
void w25qxx_driver_pc_reset(W25qx_Driver_t *W25qx_Driver);
uint8_t W25QXX_Read(W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) ;
uint8_t W25QXX_Write(W25qx_Driver_t *W25qx_Driver,uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite) ;
uint8_t W25QXX_Write_NoCheck(W25qx_Driver_t *W25qx_Driver, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
uint8_t W25QXX_Erase_Sector(W25qx_Driver_t *W25qx_Driver,uint32_t Dst_Addr) ;
uint8_t W25QXX_Erase_Block(W25qx_Driver_t *W25qx_Driver,uint32_t Dst_Addr)  ;
void w25qxx_test(W25qx_Driver_t *W25qx_Driver);

void w25qxx_ymodem_write(uint32_t addr, uint8_t *buffer, uint32_t length);
void w25qxx_ymodem_erase(uint32_t addr, uint32_t ByteLen);


#endif
