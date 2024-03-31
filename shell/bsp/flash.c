#include "flash.h"

void FlashErase(uint32_t addr, uint32_t count) {
	uint8_t i;
	int pos = count % 4096;
	if( pos != 0){
		count = count/4096 + 1;
	}else{
		count = count/4096;
	}
	FLASH_Unlock();
	for (i = 0; i < count; ++i) {
		FLASH_ErasePage(addr + i * FLASH_SECTOR_SIZE);
	}
	FLASH_Lock();
}
 
void FlashWrite(uint32_t addr, uint8_t *buffer, uint32_t length) {
	uint16_t i, data = 0;
	FLASH_Unlock();
	for (i = 0; i < length; i += 2) {
		data = (*(buffer + i + 1) << 8) + (*(buffer + i));
		FLASH_ProgramHalfWord((uint32_t)(addr + i), data);
	}
	FLASH_Lock();
}
 
void FlashRead(uint32_t addr, uint8_t *buffer, uint32_t length) {
	memcpy(buffer, (void *)addr, length);
}


