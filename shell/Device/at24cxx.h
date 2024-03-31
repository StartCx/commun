#ifndef _AT24CXX_H
#define _AT24CXX_H


#include "core.h"
#include "simulated_i2c.h"


enum
{
	AT24C01,
	AT24C02,
	AT24C04,
	AT24C08,
	AT24C16,
	AT24C32,
	AT24C64,
	AT24C128,
	AT24C256,
	AT24C512,
};

typedef struct EEPROM_Driver_t
{
	unsigned char  ID;
	unsigned char  OWN_ADDR;
	unsigned short PAGE_BYTE;
	unsigned int   MAX_MEM_ADDRESS;
	
	unsigned short page;
	unsigned short remainder;
	unsigned short addr;
	unsigned short temp;
	unsigned short num;
	unsigned char *pDat;
	CPU_RegisterClass_t Write_Driver;
	CPU_RegisterClass_t Read_Driver;
	I2C_Driver_t *I2C_Driver;
	
	void (*Init)(struct EEPROM_Driver_t *EEPROM_Driver);
	void (*Test)(struct EEPROM_Driver_t *EEPROM_Driver);
	uint8_t (*Read)( struct EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num);
	uint8_t (*Write)(struct EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num);
}EEPROM_Driver_t;

extern EEPROM_Driver_t EEPROM_Driver;

extern void AT24CXX_test(EEPROM_Driver_t *EEPROM_Driver);
extern uint8_t EEPROM_PageWrite(EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num);
extern uint8_t EEPROM_Read(EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num);
extern void EEPROM_Driver_Init(EEPROM_Driver_t *EEPROM_Driver);
extern void EEPROM_ymodem_write(uint32_t addr, uint8_t *buffer, uint32_t length);

#endif


