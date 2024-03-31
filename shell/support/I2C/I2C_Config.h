#ifndef __I2C_CONFIG_H
#define __I2C_CONFIG_H



#include "core.h"
#include "I2C_StructLib.h"
#include "I2C_Bitbang.h"
#include "I2C_Software.h"
#include "I2C_Hardware.h"

extern I2C_M_Hardware_t I2C1_Master;
extern I2C_M_Bitbang_t  I2C2_Master;
extern I2C_M_Software_t I2C3_Master;
extern I2C_S_Bitbang_t  I2C4_Slave;

#endif
