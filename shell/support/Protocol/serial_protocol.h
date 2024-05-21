#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H

#include "core.h"
#include "servo_output.h"


#define SERIAL_BUF_SIZE 64

typedef enum {
    FRAME_STATE_HEADER_1,
    FRAME_STATE_HEADER_2,
    FRAME_STATE_LENGTH,
    FRAME_STATE_DATA,
    FRAME_STATE_CHECKSUM,
    FRAME_STATE_FOOTER,
    FRAME_STATE_INVALID
} SerialState;




typedef struct {
    uint8_t flag;            // ����
    SerialState state;       // ״̬
    uint8_t cnt;             // ������
    uint8_t len;             // ���ݳ���
    uint8_t calc_xor;        // ����У��
    uint8_t recv_xor;		 // ����У��
    uint8_t Buf_Size;		 // ���泤��
    uint8_t Buf[SERIAL_BUF_SIZE];// ��������
} SerialPacket_t;




extern SerialPacket_t SerialPacket;
extern void Serial_Receive(SerialPacket_t *frame, uint8_t bytedata);
extern void Serial_Data_Analysis(const uint8_t *datas, uint8_t len);



#endif
