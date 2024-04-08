#include "I2C_Hardware.h"



/*******************************************************************************************************************************/

void I2C_M_Hardware_Config(I2C_M_Hardware_t *I2C_Driver)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;
	I2C_DeInit(I2C_Driver->I2Cx);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  	/* ¿ªÂ©Êä³ö */
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SCL;
	GPIO_Init(I2C_Driver->PORT_SCL, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = I2C_Driver->PIN_SDA;
	GPIO_Init(I2C_Driver->PORT_SDA, &GPIO_InitStructure);
	
	/* I2C ÅäÖÃ */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x01; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Driver->I2C_Speed;
	I2C_Init(I2C_Driver->I2Cx, &I2C_InitStructure);
	I2C_Cmd(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->I2Cx->CR1 = 0x401;
	I2C_Driver->I2Cx->SR1 = 0x00;
	
}


uint8_t I2C_M_Hardware_Open(I2C_M_Hardware_t *I2C_Driver)
{
	if( I2C_Driver->Lock == CORE_UNLOCK){
		I2C_Driver->Lock =  CORE_LOCK;
		return CORE_SUCCESS;
	}
	return CORE_ERROR;
}

void I2C_M_Hardware_Close(I2C_M_Hardware_t *I2C_Driver)
{
	I2C_Driver->Lock = CORE_UNLOCK;
}

uint8_t I2C_M_Hardware_Endp(I2C_M_Hardware_t *I2C_Driver)
{
	return CORE_DONE;
}

uint8_t I2C_M_Hardware_Result(I2C_M_Hardware_t *I2C_Driver)
{
	return I2C_Driver->Ret;
}


void I2C_M_Hardware_Set(I2C_M_Hardware_t *I2C_Driver,uint8_t Dev_Addr, uint16_t Reg_Addr, uint8_t Reg_Size, uint8_t *pData, uint16_t Size)
{
	if( I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BERR) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_STOPF)){
		I2C_M_Hardware_Config(I2C_Driver);
	}
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		} 
	} 
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  {
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		} 
	} 
	
	I2C_Send7bitAddress(I2C_Driver->I2Cx, Dev_Addr, I2C_Direction_Transmitter);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))  {
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		}
	}
	
	if( Reg_Size == 2){
		I2C_SendData(I2C_Driver->I2Cx, (uint8_t)(Reg_Addr>>8));  
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while(! I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = REG_ADDR_NO_ACK;
				goto ERROR;
			}
		} 
	}
	
	I2C_SendData(I2C_Driver->I2Cx, (uint8_t)(Reg_Addr&0x00FF));  
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(! I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = REG_ADDR_NO_ACK;
			goto ERROR;
		}
	} 

	while(Size--)  
	{
		I2C_SendData(I2C_Driver->I2Cx, *pData); 
		pData++; 
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while (!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = WR_VALUE_NO_ACK;
				goto ERROR;
			}
		} 
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
}



void I2C_M_Hardware_Get(I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint16_t Reg_Addr,uint8_t Reg_Size, uint8_t *pData, uint16_t Size) 
{
	if( I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BERR) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_STOPF)){
		I2C_M_Hardware_Config(I2C_Driver);
	}
	
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_Send7bitAddress(I2C_Driver->I2Cx, Dev_Addr, I2C_Direction_Transmitter);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		}
	}
	if( Reg_Size == 2){
		I2C_SendData(I2C_Driver->I2Cx, (uint8_t)(Reg_Addr>>8));  
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while(! I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = REG_ADDR_NO_ACK;
				goto ERROR;
			}
		} 
	}
	I2C_SendData(I2C_Driver->I2Cx, (uint8_t)(Reg_Addr&0x00FF));  
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(! I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = REG_ADDR_NO_ACK;
			goto ERROR;
		}
	} 
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_Send7bitAddress(I2C_Driver->I2Cx, Dev_Addr, I2C_Direction_Receiver);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		}
	}
	while(Size)  
	{
		if( Size == 1){
			I2C_AcknowledgeConfig(I2C_Driver->I2Cx, DISABLE);
			I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
		}
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while(I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = WR_VALUE_NO_ACK;
				goto ERROR;
			} 
		} 
		*pData = I2C_ReceiveData(I2C_Driver->I2Cx);
		pData++; 
		Size--;           
	}
	I2C_AcknowledgeConfig(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Ret = NO_ERROR;
	return;
ERROR:
	I2C_AcknowledgeConfig(I2C_Driver->I2Cx, ENABLE);
	I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
}




void I2C_M_Hardware_Write(I2C_M_Hardware_t *I2C_Driver,uint8_t Dev_Addr, uint8_t *pData, uint16_t Size)
{
	if( I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BERR) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_STOPF)){
		I2C_M_Hardware_Config(I2C_Driver);
	}
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY))   {
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	} 
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  {
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	} 
	I2C_Send7bitAddress(I2C_Driver->I2Cx, Dev_Addr, I2C_Direction_Transmitter);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		}
	}
	while(Size--)  
	{
		I2C_SendData(I2C_Driver->I2Cx, *pData); 
		pData++; 
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while (!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = WR_VALUE_NO_ACK;
				goto ERROR;
			} 
		} 
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
}



void I2C_M_Hardware_Read(I2C_M_Hardware_t *I2C_Driver, uint8_t Dev_Addr, uint8_t *pData, uint16_t Size) 
{	
	if( I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BERR) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_STOPF)){
		I2C_M_Hardware_Config(I2C_Driver);
	}
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_Send7bitAddress(I2C_Driver->I2Cx, Dev_Addr, I2C_Direction_Receiver);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		}
	}
	while(Size)  
	{
		if( Size == 1){
			I2C_AcknowledgeConfig(I2C_Driver->I2Cx, DISABLE);
			I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
		}
		I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
		while(I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0){
			if((I2C_Driver->Delay_cnt--) == 0){
				I2C_Driver->Ret = WR_VALUE_NO_ACK;
				goto ERROR;
			} 
		} 
		*pData = I2C_ReceiveData(I2C_Driver->I2Cx);
		pData++;
		Size--;
	}
	I2C_AcknowledgeConfig(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Ret = NO_ERROR;
	return;
ERROR:
	I2C_AcknowledgeConfig(I2C_Driver->I2Cx, ENABLE);
	I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
}


void I2C_M_Hardware_Detect(I2C_M_Hardware_t *I2C_Driver, uint8_t _Address)
{
	if( I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BERR) || I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_STOPF)){
		I2C_M_Hardware_Config(I2C_Driver);
	}
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(I2C_GetFlagStatus(I2C_Driver->I2Cx, I2C_FLAG_BUSY)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_GenerateSTART(I2C_Driver->I2Cx, ENABLE);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_MODE_SELECT)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = BUS_ERROR;
			goto ERROR;
		}
	}
	I2C_Send7bitAddress(I2C_Driver->I2Cx, _Address, I2C_Direction_Transmitter);
	I2C_Driver->Delay_cnt = I2C_Driver->Delay_time;
	while(!I2C_CheckEvent(I2C_Driver->I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if((I2C_Driver->Delay_cnt--) == 0){
			I2C_Driver->Ret = DEV_ADDR_NO_ACK;
			goto ERROR;
		} 
	}
	I2C_Driver->Ret = NO_ERROR;
ERROR:
	I2C_GenerateSTOP(I2C_Driver->I2Cx, ENABLE);
	return;
}
