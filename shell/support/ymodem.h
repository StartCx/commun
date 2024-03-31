#ifndef __YMODEM_H
#define __YMODEM_H

#include "queue.h"
#include "flash.h"

#define FILE_SIZE_STR_LENGTH    50
#define FRAME_HEADER_SIZE       3
#define STX_DATA_SIZE           1024
#define SOH_DATA_SIZE           128

#define YMODEM_SOH		0x01



#define YMODEM_STX		0x02
#define YMODEM_EOT		0x04
#define YMODEM_ACK		0x06
#define YMODEM_NAK		0x15
#define YMODEM_CA		0x18
#define YMODEM_C		0x43


typedef enum {
	UPDATE_RUNNING,
	UPDATE_PROGRAM,
	UPDATE_QUIT,
} ProcessStatus;

typedef struct {
	ProcessStatus process;
	uint8_t status;
	uint8_t id;
	uint32_t addr;
	uint32_t Init_addr;
	uint32_t filesize;
	uint32_t length;
	char filename[32];
	void (*Write)(uint32_t addr, uint8_t *buffer, uint32_t length);
	void (*Read)(uint32_t addr, uint8_t *buffer, uint32_t length);
	void (*Erase)(uint32_t addr, uint32_t ByteLen);
} YModem;

extern YModem ymodem;

void YModem_Recv(Queue1048_t *p);
int ymodem_Task(Queue1048_t *p);
#endif
