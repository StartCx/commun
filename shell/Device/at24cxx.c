#include "at24cxx.h"

EEPROM_Driver_t EEPROM_Driver = {
	.ID = AT24C256,
	.OWN_ADDR = 0xA0,
	.Read     = EEPROM_Read,
	.Write    = EEPROM_PageWrite,
	.Test     = AT24CXX_test,
	.Init	  = EEPROM_Driver_Init,
};


void EEPROM_Driver_Init(EEPROM_Driver_t *EEPROM_Driver)
{
	EEPROM_Driver->I2C_Driver = &I2C_Master;
}
//页写函数,有自动翻页功能,24C64一页32Byte,num最大可写65523个字节
uint8_t EEPROM_PageWrite(EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num)
{
	enum
	{
		I2C_AT24CXX_PARAM,
		I2C_PAGEWRITE_INIT,
		I2C_PAGEWRITE_GROUP1,
		I2C_PAGEWRITE_GROUP1_1,
		I2C_PAGEWRITE_GROUP1_2,
		I2C_PAGEWRITE_GROUP2,
		I2C_PAGEWRITE_GROUP3,
		I2C_PAGEWRITE_GROUP3_delay,
		I2C_PAGEWRITE_GROUP4,
		I2C_PAGEWRITE_GROUP5,
		I2C_PAGEWRITE_GROUP6,
		I2C_PAGEWRITE_GROUP7,
		I2C_PAGEWRITE_GROUP7_delay,
		I2C_PAGEWRITE_GROUP8,
		I2C_PAGEWRITE_GROUP9,
		I2C_PAGEWRITE_GROUP10,
		I2C_PAGEWRITE_ENDP,
		I2C_PAGEWRITE_SUM
	};
	static const void *function[I2C_PAGEWRITE_SUM] = {
		[I2C_AT24CXX_PARAM] 			= &&I2C_AT24CXX_PARAM,
		[I2C_PAGEWRITE_INIT] 			= &&I2C_PAGEWRITE_INIT,
		[I2C_PAGEWRITE_GROUP1] 			= &&I2C_PAGEWRITE_GROUP1,
		
		[I2C_PAGEWRITE_GROUP1_1] 		= &&I2C_PAGEWRITE_GROUP1_1,
		[I2C_PAGEWRITE_GROUP1_2] 		= &&I2C_PAGEWRITE_GROUP1_2,
		
		[I2C_PAGEWRITE_GROUP2] 			= &&I2C_PAGEWRITE_GROUP2,
		[I2C_PAGEWRITE_GROUP3] 			= &&I2C_PAGEWRITE_GROUP3,
		[I2C_PAGEWRITE_GROUP3_delay] 	= &&I2C_PAGEWRITE_GROUP3_delay,
		[I2C_PAGEWRITE_GROUP4] 			= &&I2C_PAGEWRITE_GROUP4,
		[I2C_PAGEWRITE_GROUP5] 			= &&I2C_PAGEWRITE_GROUP5,
		[I2C_PAGEWRITE_GROUP6] 			= &&I2C_PAGEWRITE_GROUP6,
		[I2C_PAGEWRITE_GROUP7] 			= &&I2C_PAGEWRITE_GROUP7,
		[I2C_PAGEWRITE_GROUP7_delay] 	= &&I2C_PAGEWRITE_GROUP7_delay,
		[I2C_PAGEWRITE_GROUP8] 			= &&I2C_PAGEWRITE_GROUP8,
		[I2C_PAGEWRITE_GROUP9] 			= &&I2C_PAGEWRITE_GROUP9,
		[I2C_PAGEWRITE_GROUP10] 		= &&I2C_PAGEWRITE_GROUP10,
		[I2C_PAGEWRITE_ENDP] 			= &&I2C_PAGEWRITE_ENDP,
	};
	goto *function[EEPROM_Driver->Write_Driver.R15_PC];
I2C_AT24CXX_PARAM:
	switch(EEPROM_Driver->ID){
		case AT24C01:{	EEPROM_Driver->PAGE_BYTE =  8; EEPROM_Driver->MAX_MEM_ADDRESS = 128;break;};
		case AT24C02:{  EEPROM_Driver->PAGE_BYTE =  8; EEPROM_Driver->MAX_MEM_ADDRESS = 256;break;};
		case AT24C04:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 512;break;};
		case AT24C08:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 1024;break;};
		case AT24C16:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 2048;break;};
		case AT24C32:{  EEPROM_Driver->PAGE_BYTE = 32; EEPROM_Driver->MAX_MEM_ADDRESS = 4096;break;};
		case AT24C64:{  EEPROM_Driver->PAGE_BYTE = 32; EEPROM_Driver->MAX_MEM_ADDRESS = 8192;break;};
		case AT24C128:{ EEPROM_Driver->PAGE_BYTE = 64; EEPROM_Driver->MAX_MEM_ADDRESS = 16384;break;};
		case AT24C256:{ EEPROM_Driver->PAGE_BYTE = 64; EEPROM_Driver->MAX_MEM_ADDRESS = 32768;break;};					
		case AT24C512:{ EEPROM_Driver->PAGE_BYTE =128; EEPROM_Driver->MAX_MEM_ADDRESS = 65536;break;};
		default:{return CORE_DONE;};
	}
	EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_INIT;
	return CORE_RUNNING;
I2C_PAGEWRITE_INIT:
	if( address + num > EEPROM_Driver->MAX_MEM_ADDRESS){
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_INIT;
		return CORE_DONE;
	}
	EEPROM_Driver->temp = 0;
	EEPROM_Driver->addr = address;
	if(EEPROM_Driver->addr%EEPROM_Driver->PAGE_BYTE){
		EEPROM_Driver->temp = EEPROM_Driver->PAGE_BYTE-(EEPROM_Driver->addr%EEPROM_Driver->PAGE_BYTE);			//32-7=25 //计算出当前地址还差多少字节满1页
		if(num<=EEPROM_Driver->temp){
			EEPROM_Driver->temp = num;
		}
	}
	EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP1;
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP1:
	if( i2c_open_dev(EEPROM_Driver->I2C_Driver) == CORE_SUCCESS){
		EEPROM_Driver->Write_Driver.TimerTick = 3;
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP1_1;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP1_1:
	if( EEPROM_Driver->Write_Driver.TimerTick > 0){
		EEPROM_Driver->Write_Driver.TimerTick--;
		i2c_detect_addr_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR);
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP1_2;
	}else{
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_ENDP;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP1_2:	
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE)
	{
		if( i2c_endp_result(EEPROM_Driver->I2C_Driver) == NO_ERROR){
			EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP2;
		}else{
			EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP1_1;
		}
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP2:
	if(EEPROM_Driver->temp)//先填满写入地址的页
	{
		if( EEPROM_Driver->ID < AT24C04){
			i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 1, pDat, EEPROM_Driver->temp);
		}else{
			i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 2, pDat, EEPROM_Driver->temp);
		}
		
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP3;
	}else{
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP4;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP3:			
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE){
		EEPROM_Driver->Write_Driver.R6_Count = 5000;
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP3_delay;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP3_delay:
	if( EEPROM_Driver->Write_Driver.R6_Count == 0){
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP4;
	}else{
		EEPROM_Driver->Write_Driver.R6_Count--;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP4:
	num -= EEPROM_Driver->temp;
	EEPROM_Driver->addr += EEPROM_Driver->temp;//地址加上已经写入的字节
	EEPROM_Driver->page = num/EEPROM_Driver->PAGE_BYTE;			
	EEPROM_Driver->remainder = num%EEPROM_Driver->PAGE_BYTE;
	EEPROM_Driver->Write_Driver.R1_Index = 0;
	EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP5;
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP5:	
	if( EEPROM_Driver->Write_Driver.R1_Index < EEPROM_Driver->page){
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP6;
	}else{
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP8;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP6:
	if( EEPROM_Driver->ID < AT24C04){
		i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 1,pDat+EEPROM_Driver->temp, EEPROM_Driver->PAGE_BYTE);
	}else{
		i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 2,pDat+EEPROM_Driver->temp, EEPROM_Driver->PAGE_BYTE);
	}
	EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP7;
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP7:	
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE){
		EEPROM_Driver->addr += EEPROM_Driver->PAGE_BYTE;
		EEPROM_Driver->temp += EEPROM_Driver->PAGE_BYTE;
		EEPROM_Driver->Write_Driver.R1_Index++;
		EEPROM_Driver->Write_Driver.R6_Count = 5000;
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP7_delay;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP7_delay:
	if( EEPROM_Driver->Write_Driver.R6_Count == 0){
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP5;
	}else{
		EEPROM_Driver->Write_Driver.R6_Count--;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP8:		
	if(EEPROM_Driver->remainder)
	{
		if( EEPROM_Driver->ID < AT24C04){
			i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 1,pDat+EEPROM_Driver->temp, EEPROM_Driver->remainder);
		}else{
			i2c_write_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr, 2,pDat+EEPROM_Driver->temp, EEPROM_Driver->remainder);
		}
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP9;
	}else{
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP10;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP9:
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE){
		EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_GROUP10;
	}
	return CORE_RUNNING;
I2C_PAGEWRITE_GROUP10:
	EEPROM_Driver->Write_Driver.R15_PC = I2C_PAGEWRITE_ENDP;
	return CORE_RUNNING;
I2C_PAGEWRITE_ENDP:
	i2c_close_dev(EEPROM_Driver->I2C_Driver);
	EEPROM_Driver->Write_Driver.R15_PC = I2C_AT24CXX_PARAM;
	return CORE_DONE;
}




//页写函数,有自动翻页功能,24C64一页32Byte,num最大可写65523个字节
uint8_t EEPROM_Read(EEPROM_Driver_t *EEPROM_Driver, unsigned short address,unsigned char *pDat, unsigned short num)
{
	enum
	{
		EEPROM_READ_PARAM,
		EEPROM_READ_INIT,
		EEPROM_READ_WAIT_1,
		EEPROM_READ_WAIT_2,
		EEPROM_READ_GROUP1,
		EEPROM_READ_GROUP2,
		EEPROM_READ_GROUP3,
		EEPROM_READ_SUM
	};
	static const void *function[EEPROM_READ_SUM] = {
		[EEPROM_READ_PARAM] 		= &&EEPROM_READ_PARAM,
		[EEPROM_READ_INIT] 			= &&EEPROM_READ_INIT,
		
		[EEPROM_READ_WAIT_1] 		= &&EEPROM_READ_WAIT_1,
		[EEPROM_READ_WAIT_2] 		= &&EEPROM_READ_WAIT_2,
		
		[EEPROM_READ_GROUP1] 		= &&EEPROM_READ_GROUP1,
		[EEPROM_READ_GROUP2] 		= &&EEPROM_READ_GROUP2,
		[EEPROM_READ_GROUP3] 		= &&EEPROM_READ_GROUP3,
	};
	goto *function[EEPROM_Driver->Read_Driver.R15_PC];
EEPROM_READ_PARAM:
	switch(EEPROM_Driver->ID){
		case AT24C01:{	EEPROM_Driver->PAGE_BYTE =  8; EEPROM_Driver->MAX_MEM_ADDRESS = 128;break;};
		case AT24C02:{  EEPROM_Driver->PAGE_BYTE =  8; EEPROM_Driver->MAX_MEM_ADDRESS = 256;break;};
		case AT24C04:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 512;break;};
		case AT24C08:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 1024;break;};
		case AT24C16:{  EEPROM_Driver->PAGE_BYTE = 16; EEPROM_Driver->MAX_MEM_ADDRESS = 2048;break;};
		case AT24C32:{  EEPROM_Driver->PAGE_BYTE = 32; EEPROM_Driver->MAX_MEM_ADDRESS = 4096;break;};
		case AT24C64:{  EEPROM_Driver->PAGE_BYTE = 32; EEPROM_Driver->MAX_MEM_ADDRESS = 8192;break;};
		case AT24C128:{ EEPROM_Driver->PAGE_BYTE = 64; EEPROM_Driver->MAX_MEM_ADDRESS = 16384;break;};
		case AT24C256:{ EEPROM_Driver->PAGE_BYTE = 64; EEPROM_Driver->MAX_MEM_ADDRESS = 32768;break;};					
		case AT24C512:{ EEPROM_Driver->PAGE_BYTE =128; EEPROM_Driver->MAX_MEM_ADDRESS = 65536;break;};
		default:{return CORE_DONE;};
	}
	EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_INIT;
	return CORE_RUNNING;
EEPROM_READ_INIT:
	if( address + num > EEPROM_Driver->MAX_MEM_ADDRESS){
		EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_INIT;
		return CORE_DONE;
	}
	EEPROM_Driver->addr = address;
	EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_GROUP1;
	return CORE_RUNNING;
EEPROM_READ_GROUP1:
	if( i2c_open_dev(EEPROM_Driver->I2C_Driver) == CORE_SUCCESS){
		EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_WAIT_1;
		EEPROM_Driver->Read_Driver.TimerTick = 3;
	}
	return CORE_RUNNING;
EEPROM_READ_WAIT_1:
	if( EEPROM_Driver->Read_Driver.TimerTick > 0){
		EEPROM_Driver->Read_Driver.TimerTick--;
		i2c_detect_addr_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR);
		EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_WAIT_2;
	}else{
		i2c_close_dev(EEPROM_Driver->I2C_Driver);
		EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_PARAM;
		return CORE_DONE;
	}
	return CORE_RUNNING;
EEPROM_READ_WAIT_2:
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE)
	{
		if( i2c_endp_result(EEPROM_Driver->I2C_Driver) == NO_ERROR){
			EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_GROUP2;
		}else{
			EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_WAIT_1;
		}
	}
	return CORE_RUNNING;
EEPROM_READ_GROUP2:
	if( EEPROM_Driver->ID < AT24C04){
		i2c_read_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr,1, pDat, num);
	}else{
		i2c_read_dev_reg_value_proc(EEPROM_Driver->I2C_Driver, EEPROM_Driver->OWN_ADDR, EEPROM_Driver->addr,2, pDat, num);
	}
	EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_GROUP3;
	return CORE_RUNNING;
EEPROM_READ_GROUP3:			
	if( i2c_write_read_endp(EEPROM_Driver->I2C_Driver) == CORE_DONE){
		i2c_close_dev(EEPROM_Driver->I2C_Driver);
		EEPROM_Driver->Read_Driver.R15_PC = EEPROM_READ_PARAM;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}

void EEPROM_ymodem_write(uint32_t addr, uint8_t *buffer, uint32_t length)
{
	while( EEPROM_PageWrite(&EEPROM_Driver,addr,buffer,length) != CORE_DONE);
}

void AT24CXX_test(EEPROM_Driver_t *EEPROM_Driver)
{
	int i;
	uint8_t Txbuf[256];
	uint8_t Rxbuf[256];
	
	for( i = 0; i< 256;i++)
	{
		Txbuf[i] = 256-i;
	}
	printf("\r\n/**********write start**********/");
	printf("\r\nAT24CXX write addr 0 data:\r\n");
	for( i = 0; i<256;i++){
		if( i %8 == 0){
			printf("\r\n");
		}
		printf("0x%02x ",Txbuf[i]);
	}
	printf(" | 0x%02x\r\n",i);
	
	while(EEPROM_PageWrite(EEPROM_Driver,0x0, Txbuf, 256) != CORE_DONE);
	printf("\r\n/************wait***********/");
	while(EEPROM_Read(EEPROM_Driver, 0x00,Rxbuf, 256) != CORE_DONE);
	
	printf("\r\nAT24CXX read addr 0 data:\r\n");
	for( i = 0; i<256;i++){
		if( i %8 == 0){
			printf("\r\n");
		}
		printf("0x%02x ",Rxbuf[i]);
	}
	printf(" | 0x%02x\r\n",i);
	printf("/************read done**********/\r\n");
}


