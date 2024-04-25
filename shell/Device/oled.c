#include "oled.h"

#define OLED_Cmd_Address		0x00
#define OLED_Data_Address		0x40

const uint8_t OLED_Init_Array[] = {
	0xAE,0x00,0x10,0x40,0xB0,0x81,0xFF,0xA1,0xA6,0x3F,
	0xC8,0xD3,0x00,0xD5,0x80,0xD8,0x05,0xD9,0xF1,0xDA,
	0x12,0xDB,0x30,0x8D,0x14,0xAF
};

OLED_Device_t OLED_Device = {
	.Bus		= I2C2_M_BITBANG,
	.Dev_Addr 	= 0x78,
	.Init 		= OLED_Init,
	.Gram 		= OLED_Refresh_Gram,
	.Write_String = OLED_ShowString,
	.Timer 		= {
		.TickPeroid = 10,
	},
	.Register 	= {
		.R9_Error = BUS_ERROR,
	},
};


char OLED_Write_Data(struct OLED_Device_t *Dev, uint8_t *buf, uint16_t size)
{
	enum
	{
		OLED_Device_SET_PROC_GROUP15,
		OLED_Device_SET_PROC_GROUP16,
		OLED_Device_SET_PROC_GROUP17,
		OLED_Device_SET_PROC_GROUP18,
		OLED_Device_SET_STATE_SUM,
	};
	static const void *function[OLED_Device_SET_STATE_SUM] = {
		[OLED_Device_SET_PROC_GROUP15]	= &&OLED_Device_SET_PROC_GROUP15,
		[OLED_Device_SET_PROC_GROUP16]	= &&OLED_Device_SET_PROC_GROUP16,
		[OLED_Device_SET_PROC_GROUP17]	= &&OLED_Device_SET_PROC_GROUP17,
		[OLED_Device_SET_PROC_GROUP18]	= &&OLED_Device_SET_PROC_GROUP18,
	};
	goto *function[Dev->Register.R15_PC];
OLED_Device_SET_PROC_GROUP15:
	if( I2Cx_Open(Dev->Bus) == CORE_SUCCESS){
		Dev->Register.R15_PC = OLED_Device_SET_PROC_GROUP16;
	}
	return CORE_RUNNING;
OLED_Device_SET_PROC_GROUP16:
	I2Cx_Set(Dev->Bus, Dev->Dev_Addr, OLED_Data_Address, 1, buf, size);
	Dev->Register.R15_PC = OLED_Device_SET_PROC_GROUP17;
	return CORE_RUNNING;
OLED_Device_SET_PROC_GROUP17:
	if( I2Cx_Endp(Dev->Bus) == CORE_DONE){
		Dev->Register.R15_PC = OLED_Device_SET_PROC_GROUP18;
	}
	return CORE_RUNNING;
OLED_Device_SET_PROC_GROUP18:
	Dev->Register.R9_Error = I2Cx_Result(Dev->Bus);
	Dev->Register.R15_PC = OLED_Device_SET_PROC_GROUP15;
	I2Cx_Close(Dev->Bus);
	return CORE_DONE;

}


char OLED_Write_Command(struct OLED_Device_t *Dev, uint8_t cmd)
{	
	enum
	{
		OLED_Device_CMD_PROC_GROUP15,
		OLED_Device_CMD_PROC_GROUP16,
		OLED_Device_CMD_PROC_GROUP17,
		OLED_Device_CMD_PROC_GROUP18,
		OLED_Device_CMD_STATE_SUM,
	};
	static const void *function[OLED_Device_CMD_STATE_SUM] = {
		[OLED_Device_CMD_PROC_GROUP15]	= &&OLED_Device_CMD_PROC_GROUP15,
		[OLED_Device_CMD_PROC_GROUP16]	= &&OLED_Device_CMD_PROC_GROUP16,
		[OLED_Device_CMD_PROC_GROUP17]	= &&OLED_Device_CMD_PROC_GROUP17,
		[OLED_Device_CMD_PROC_GROUP18]	= &&OLED_Device_CMD_PROC_GROUP18,
	};
	goto *function[Dev->Register.R15_PC];
OLED_Device_CMD_PROC_GROUP15:
	if( I2Cx_Open(Dev->Bus) == CORE_SUCCESS){
		Dev->Register.R15_PC = OLED_Device_CMD_PROC_GROUP16;
	}
	return CORE_RUNNING;
OLED_Device_CMD_PROC_GROUP16:
	Dev->Register.R2_cin = cmd;
	I2Cx_Set(Dev->Bus, Dev->Dev_Addr, OLED_Cmd_Address, 1, &Dev->Register.R2_cin, 1);
	Dev->Register.R15_PC = OLED_Device_CMD_PROC_GROUP17;
	return CORE_RUNNING;
OLED_Device_CMD_PROC_GROUP17:
	if( I2Cx_Endp(Dev->Bus) == CORE_DONE){
		Dev->Register.R15_PC = OLED_Device_CMD_PROC_GROUP18;
	}
	return CORE_RUNNING;
OLED_Device_CMD_PROC_GROUP18:
	Dev->Register.R9_Error = I2Cx_Result(Dev->Bus);
	Dev->Register.R15_PC = OLED_Device_CMD_PROC_GROUP15;
	I2Cx_Close(Dev->Bus);
	return CORE_DONE;
}



//初始化OLED
uint8_t OLED_Init(struct OLED_Device_t *Dev)
{
	enum
	{
		OLED_Device_INIT_PROC_GROUP15,
		OLED_Device_INIT_PROC_GROUP16,
		OLED_Device_INIT_PROC_GROUP17,
		OLED_Device_INIT_PROC_GROUP18,
		OLED_Device_INIT_STATE_SUM,
	};
	static const void *function[OLED_Device_INIT_STATE_SUM] = {
		[OLED_Device_INIT_PROC_GROUP15]	= &&OLED_Device_INIT_PROC_GROUP15,
		[OLED_Device_INIT_PROC_GROUP16]	= &&OLED_Device_INIT_PROC_GROUP16,
		[OLED_Device_INIT_PROC_GROUP17]	= &&OLED_Device_INIT_PROC_GROUP17,
		[OLED_Device_INIT_PROC_GROUP18]	= &&OLED_Device_INIT_PROC_GROUP18,
	};
	goto *function[Dev->Register.R15_PC];
OLED_Device_INIT_PROC_GROUP15:
	if( I2Cx_Open(Dev->Bus) == CORE_SUCCESS){
		Dev->Register.R15_PC = OLED_Device_INIT_PROC_GROUP16;
	}
	return CORE_RUNNING;
OLED_Device_INIT_PROC_GROUP16:
	I2Cx_Set(Dev->Bus, Dev->Dev_Addr, OLED_Cmd_Address, 1, (uint8_t*)OLED_Init_Array, sizeof(OLED_Init_Array));
	Dev->Register.R15_PC = OLED_Device_INIT_PROC_GROUP17;
	return CORE_RUNNING;
OLED_Device_INIT_PROC_GROUP17:
	if( I2Cx_Endp(Dev->Bus) == CORE_DONE){
		Dev->Register.R15_PC = OLED_Device_INIT_PROC_GROUP18;
	}
	return CORE_RUNNING;
OLED_Device_INIT_PROC_GROUP18:
	Dev->Register.R9_Error = I2Cx_Result(Dev->Bus);
	I2Cx_Close(Dev->Bus);
	Dev->Register.R15_PC = OLED_Device_INIT_PROC_GROUP15;
	if( Dev->Register.R9_Error == NO_ERROR){
		return CORE_DONE;
	}
	return CORE_DONE;
}





//更新显存到LCD
uint8_t OLED_Refresh_Gram(struct OLED_Device_t *Dev)
{
	if( Dev->Register.R9_Error != NO_ERROR){
		if( Dev->Init(Dev) == CORE_DONE){
			Dev->oled_state = STATE_POS1;
		}
	}else if( Dev->oled_state == STATE_POS1){
		if(OLED_Write_Command(Dev,0xb0+Dev->y) == CORE_DONE){
			Dev->oled_state = STATE_POS2;
		}
	}else if( Dev->oled_state == STATE_POS2){
		if(OLED_Write_Command(Dev,0x00) == CORE_DONE){
			Dev->oled_state = STATE_POS3;
		}
	}else if( Dev->oled_state == STATE_POS3){
		if(OLED_Write_Command(Dev,0x10) == CORE_DONE){
			Dev->oled_state = STATE_DATA;
		}
	}else if( Dev->oled_state == STATE_DATA){
		if(OLED_Write_Data(Dev,&Dev->OLED_GRAM[Dev->y*128],128) == CORE_DONE){
			Dev->oled_state = STATE_POS1;
			Dev->y = (Dev->y + 1) % 8;
			if( Dev->y == 7){
				return 1;
			}
		}
	}
	return 0;
}




void OLED_ShowChar(struct OLED_Device_t *Dev, u8 x,u8 y,u8 chr, u8 Char_Size)
{
	u8 start_x = x;
	
	chr=chr-' ';//得到偏移后的值
	if( Char_Size == 16)
	{
		for(uint8_t i=0;i<8;i++)
		{
			Dev->OLED_GRAM[x+y*128] = F8X16[chr*16+i];
			x++;
		}
		y++;
		x = start_x;
		for(uint8_t i=0;i<8;i++)
		{
			Dev->OLED_GRAM[x+y*128] = F8X16[chr*16+i+8];
			x++;
		}
	}
	else
	{
		for(uint8_t i=0;i<5;i++)
		{
			Dev->OLED_GRAM[x+y*128] = F5x8[chr][i] ;
			x++;
		}
	}
}


//显示一个字符号串
void OLED_ShowString(struct OLED_Device_t *Dev, u8 x,u8 y,u8 *chr, u8 Char_Size)
{
	unsigned char j=0;
	unsigned char x_trans = 0;
	if( Char_Size == 16){
		x_trans = 8;
	}else{
		x_trans = 5;
	}
	while (chr[j]!= '\0')
	{
		OLED_ShowChar(Dev,x,y,chr[j], Char_Size);
		x+=x_trans;
		if(x>=127)
		{
			x=0;
			y+=2;
		}
		j++;
	}
}

