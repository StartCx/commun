#ifndef __OLED_H
#define __OLED_H

#include "I2C_Config.h"
#include "codetab.h"
#include "core.h"


enum
{
	STATE_POS1,
	STATE_POS2,
	STATE_POS3,
	STATE_DATA,
};
	
typedef struct OLED_Device_t{
	uint8_t  Bus;
	uint8_t  Dev_Addr;
	uint16_t Reg_Addr;
	CPU_RegisterClass_t  Register;
	uint8_t (*Init)(struct OLED_Device_t *Dev);
	void (*Write_Data)(struct OLED_Device_t *Dev,uint8_t *buf, uint16_t size);
	void (*Write_Command)(struct OLED_Device_t *Dev,uint8_t cmd);
	void (*Write_String)(struct OLED_Device_t *Dev, u8 x,u8 y,u8 *chr, u8 Char_Size);
	uint8_t (*Gram)(struct OLED_Device_t *Dev);
	uint8_t y;
	uint8_t oled_state;
	uint8_t OLED_GRAM[128*8];
	Timer_t	 Timer;
}OLED_Device_t;


extern OLED_Device_t OLED_Device;
extern OLED_Device_t OLED_Device2;


uint8_t OLED_Init(struct OLED_Device_t *Dev);
uint8_t OLED_Refresh_Gram(struct OLED_Device_t *Dev);
char OLED_Write_Data(struct OLED_Device_t *Dev, uint8_t *buf, uint16_t size);
char OLED_Write_Command(struct OLED_Device_t *Dev, uint8_t cmd);
void OLED_ShowString(struct OLED_Device_t *Dev, u8 x,u8 y,u8 *chr, u8 Char_Size);

#endif
