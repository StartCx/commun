#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define FLASH_START_ADDR    ((uint32_t)0x8000000)
#define FLASH_END_ADDR      ((uint32_t)(0x8000000 + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))
#define FLASH_SECTOR_NUM    64
#define FLASH_SECTOR_SIZE   1024

void FlashErase(uint32_t addr, uint32_t count);
void FlashWrite(uint32_t addr, uint8_t *buffer, uint32_t length);
void FlashRead(uint32_t addr, uint8_t *buffer, uint32_t length);

#endif
