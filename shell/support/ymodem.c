#include "ymodem.h"
#include "flash.h"


YModem ymodem;

static Timer_t Packet_Handle_Timer = {
	.TickPeroid = 1000,
};
static Timer_t Delay_Echo_Timer = {
	.TickPeroid = 100000,
};


void YModem_ACK(void) {
	printf("%c", YMODEM_ACK);
}

void YModem_NACK(void) {
	printf("%c", YMODEM_NAK);
}

void YModem_C(void) {
	printf("%c", YMODEM_C);
}


static uint8_t Char2Uint(char hexChar)
{
    uint8_t val = 0;
    if(hexChar >= '0' && hexChar <= '9'){
        val = hexChar - '0';
    }
    return val;
}

static int32_t ParseFileDescription(uint8_t * content, uint8_t contentLen, char * fileName, uint32_t * fileSize)
{
    uint8_t strFileSize[FILE_SIZE_STR_LENGTH] = {0};
    uint8_t *pTemp;
    uint32_t num;
    if(content == NULL || fileName == NULL || fileSize == NULL){
		return CORE_ERROR;
	}
    *fileSize = 0;
    pTemp = strFileSize;
    num = 0;
    /* Get file name */
    while(1)
    {
        *fileName =  *content;
        if(*fileName == '\0')
            break;
        fileName++;
        content++;
    }
    content++;/* skip '\0'*/
    /* Get file size */
    while(1)
    {
        *pTemp = *content;
        if(*pTemp == ' '){
			break;
		} 
        num *= 10;
        num += Char2Uint(*pTemp);
        pTemp++;
        content++;
    }
    *fileSize = num;
	return CORE_OK;
}

void YModem_Recv(Queue1048_t *p)
{
	switch (ymodem.status) {
	case 0:
		if( p->data[0] == YMODEM_SOH){
			ymodem.process = UPDATE_RUNNING;
			ymodem.addr = ymodem.Init_addr;
			ParseFileDescription((&p->data[0] + FRAME_HEADER_SIZE), SOH_DATA_SIZE, ymodem.filename, &ymodem.filesize);
			if( ymodem.Erase != NULL){
				ymodem.Erase(ymodem.addr, ymodem.filesize);
			}
			YModem_ACK();
			YModem_C();
			ymodem.status++;
		}else{
			ymodem.process = UPDATE_QUIT;
		}
		break;
	case 1:
		if( p->data[0] == YMODEM_SOH){
			if( ymodem.filesize-ymodem.addr < 128){
				ymodem.Write(ymodem.addr, &p->data[3], ymodem.filesize-ymodem.addr);
				ymodem.addr = ymodem.filesize;
			}else{
				ymodem.Write(ymodem.addr, &p->data[3], 128);
				ymodem.addr += 128;
			}
			YModem_ACK();
		}else if( p->data[0] == YMODEM_STX){
			if( ymodem.filesize-ymodem.addr < 1024){
				ymodem.Write(ymodem.addr, &p->data[3], ymodem.filesize-ymodem.addr);
				ymodem.addr = ymodem.filesize;
			}else{
				ymodem.Write(ymodem.addr, &p->data[3], 1024);
				ymodem.addr += 1024;
			}
			YModem_ACK();
		}else if( p->data[0] == YMODEM_EOT){
			YModem_NACK();
			ymodem.status++;
		}else {
			ymodem.status = 0;
		}
		break;
	case 2:
		if( p->data[0] == YMODEM_EOT){
			YModem_ACK();
			YModem_C();
			ymodem.status++;
		}
		break;
	case 3:
		if( p->data[0] == YMODEM_SOH){
			YModem_ACK();
			ymodem.status = 0;
			ymodem.process = UPDATE_QUIT;
		}
	}
	QueueEmpty(p);
}


int ymodem_Task(Queue1048_t *p)
{	
	QueueEmpty(p);
	ymodem.process = UPDATE_PROGRAM;
	printf("\r\n");
	while(1)
	{
		if( p->length == 0 || ymodem.length != p->length)//一个数据包接收未完成或未开始
		{
			if( ymodem.process == UPDATE_PROGRAM && ymodem.status == 0)
			{
				if( Scheduler(&Delay_Echo_Timer) == 1)
				{
					YModem_C();
				}
			}
			else if( ymodem.process == UPDATE_QUIT)
			{
				return CORE_DONE;
			}
			ymodem.length = p->length;
			Restart_Scheduler_Timer(&Packet_Handle_Timer);
		}
		else if( Scheduler(&Packet_Handle_Timer) == 1)//一个数据包接收完成或超时
		{
			YModem_Recv(p);
		}
	}
}
