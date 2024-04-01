#include "I2C_Software.h"



static void Software_I2C_Delay(I2C_M_Software_t *I2C_Driver)
{	
	for (I2C_Driver->Delay_cnt = 0; I2C_Driver->Delay_cnt < I2C_Driver->Delay_time; I2C_Driver->Delay_cnt++);
}


static void Software_I2C_Start(I2C_M_Software_t *I2C_Driver)
{
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
}


static void Software_I2C_Stop(I2C_M_Software_t *I2C_Driver)
{
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
}


static void Software_I2C_Write(I2C_M_Software_t *I2C_Driver, uint8_t data) 
{ 
    for(I2C_Driver->Mask = 0x80; I2C_Driver->Mask>0; I2C_Driver->Mask >>= 1) 
	{
        if( data & I2C_Driver->Mask) {
            GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
        } else {
            GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
        }
        GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
        Software_I2C_Delay(I2C_Driver);
        GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
        Software_I2C_Delay(I2C_Driver);
    }
}


static uint8_t Software_I2C_Read(I2C_M_Software_t *I2C_Driver) {       
    I2C_Driver->ReadData = 0;
    for(I2C_Driver->Mask = 0x80; I2C_Driver->Mask>0; I2C_Driver->Mask>>=1) {
		GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
		GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
        Software_I2C_Delay(I2C_Driver);
		if( GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
			I2C_Driver->ReadData &= ~I2C_Driver->Mask;
		}else{
			I2C_Driver->ReadData |=  I2C_Driver->Mask;
		}
        GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
        Software_I2C_Delay(I2C_Driver);
    }
    return I2C_Driver->ReadData;    // 消耗1个周期的SCL
}


static uint8_t Software_I2C_WaitAck(I2C_M_Software_t *I2C_Driver) {
	I2C_Driver->Index = 0;
    GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
    GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
    Software_I2C_Delay(I2C_Driver);
	while( GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 1 && I2C_Driver->Index++ < 50);
	I2C_Driver->Ack_state = GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	Software_I2C_Delay(I2C_Driver);
    return I2C_Driver->Ack_state;
}


static void Software_I2C_Ack(I2C_M_Software_t *I2C_Driver)
{
	GPIO_SET_LOW(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
}


static void Software_I2C_NAck(I2C_M_Software_t *I2C_Driver)
{
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);
	GPIO_SET_LOW(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	Software_I2C_Delay(I2C_Driver);	
}

/*******************************************************************************************************************************/

void I2C_M_Software_Config(I2C_M_Software_t *I2C_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SCL;
	GPIO_Init(I2C_Driver->PORT_SCL, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SDA;
	GPIO_Init(I2C_Driver->PORT_SDA, &GPIO_InitStructure);
	
	GPIO_SET_HIGH(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL);
	GPIO_SET_HIGH(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA);
}


uint8_t I2C_M_Software_Open(I2C_M_Software_t *I2C_Driver)
{
	if( I2C_Driver->Lock == CORE_UNLOCK){
		I2C_Driver->Lock =  CORE_LOCK;
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void I2C_M_Software_Close(I2C_M_Software_t *I2C_Driver)
{
	I2C_Driver->Lock = CORE_UNLOCK;
}

uint8_t I2C_M_Software_Endp(I2C_M_Software_t *I2C_Driver)
{
	return CORE_DONE;
}

uint8_t I2C_M_Software_Result(I2C_M_Software_t *I2C_Driver)
{
	return I2C_Driver->Ret;
}


void I2C_M_Software_Set(I2C_M_Software_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *value, uint16_t Size)
{
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Ret = BUS_ERROR;	/* 器件无应答 */
		return;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, Dev_Addr | 0);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	if( Reg_Size == 2){
		Software_I2C_Write(I2C_Driver, (uint8_t)(Reg_Addr>>8));             //数据地址(16位)
		if( Software_I2C_WaitAck(I2C_Driver) != 0){
			I2C_Driver->Ret = REG_ADDR_NO_ACK;	/* 寄存器无应答 */
			goto ERROR;
		}
	}
	Software_I2C_Write(I2C_Driver, (uint8_t)(Reg_Addr&0x00FF));
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = REG_ADDR_NO_ACK;	/* 寄存器无应答 */
		goto ERROR;
	}
	for( I2C_Driver->Index = 0; I2C_Driver->Index < Size; I2C_Driver->Index++ ){
		Software_I2C_Write(I2C_Driver, value[I2C_Driver->Index]);
		if( Software_I2C_WaitAck(I2C_Driver) != 0){
			I2C_Driver->Ret = WR_VALUE_NO_ACK;	/* 寄存器无应答 */
			goto ERROR;
		}
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	Software_I2C_Stop(I2C_Driver);
}



void I2C_M_Software_Get(I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *value, uint16_t Size) 
{
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Ret = BUS_ERROR;	/* 器件无应答 */
		return;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, Dev_Addr | 0);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	if( Reg_Size == 2){
		Software_I2C_Write(I2C_Driver, (uint8_t)(Reg_Addr>>8));             //数据地址(16位)
		if( Software_I2C_WaitAck(I2C_Driver) != 0){
			I2C_Driver->Ret = REG_ADDR_NO_ACK;	/* 寄存器无应答 */
			goto ERROR;
		}
	}
	Software_I2C_Write(I2C_Driver, (uint8_t)(Reg_Addr&0x00FF));
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = REG_ADDR_NO_ACK;	/* 寄存器无应答 */
		goto ERROR;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, Dev_Addr | 1);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	for( I2C_Driver->Index = 0; I2C_Driver->Index < Size; I2C_Driver->Index++ ){
		value[I2C_Driver->Index] = Software_I2C_Read(I2C_Driver);
		if (I2C_Driver->Index != Size - 1){
			Software_I2C_Ack(I2C_Driver);
		}else{
			Software_I2C_NAck(I2C_Driver);
		}
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	Software_I2C_Stop(I2C_Driver);
}




void I2C_M_Software_Write(I2C_M_Software_t *I2C_Driver,uint8_t Dev_Addr, uint8_t *value, uint16_t Size)
{
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Ret = BUS_ERROR;	/* 器件无应答 */
		return;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, Dev_Addr | 0);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	for( I2C_Driver->Index = 0; I2C_Driver->Index < Size; I2C_Driver->Index++ ){
		Software_I2C_Write(I2C_Driver, value[I2C_Driver->Index]);
		if( Software_I2C_WaitAck(I2C_Driver) != 0){
			I2C_Driver->Ret = WR_VALUE_NO_ACK;	/* 寄存器无应答 */
			goto ERROR;
		}
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	Software_I2C_Stop(I2C_Driver);
}



void I2C_M_Software_Read(I2C_M_Software_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *value, uint16_t Size) 
{	
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Ret = BUS_ERROR;	/* 器件无应答 */
		return;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, Dev_Addr | 1);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	for( I2C_Driver->Index = 0; I2C_Driver->Index < Size; I2C_Driver->Index++ ){
		value[I2C_Driver->Index] = Software_I2C_Read(I2C_Driver);
		if (I2C_Driver->Index != Size - 1){
			Software_I2C_Ack(I2C_Driver);
		}else{
			Software_I2C_NAck(I2C_Driver);
		}
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	Software_I2C_Stop(I2C_Driver);
}


void I2C_M_Software_Detect(I2C_M_Software_t *I2C_Driver, uint8_t _Address)
{
	if( GPIO_GET_STATE(I2C_Driver->PORT_SCL, I2C_Driver->PIN_SCL) == 0 || GPIO_GET_STATE(I2C_Driver->PORT_SDA, I2C_Driver->PIN_SDA) == 0){
		I2C_Driver->Ret = BUS_ERROR;	/* 器件无应答 */
		goto ERROR;
	}
	Software_I2C_Start(I2C_Driver);
	Software_I2C_Write(I2C_Driver, _Address & ~0x01);
	if( Software_I2C_WaitAck(I2C_Driver) != 0){
		I2C_Driver->Ret = DEV_ADDR_NO_ACK;	/* 器件无应答 */
		goto ERROR;
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	Software_I2C_Stop(I2C_Driver);
}
