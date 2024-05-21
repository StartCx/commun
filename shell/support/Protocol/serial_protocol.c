#include "serial_protocol.h"



SerialPacket_t SerialPacket = {
	.flag = 0,
	.state = FRAME_STATE_HEADER_1,
	.Buf_Size = SERIAL_BUF_SIZE,
};

void Serial_Receive(SerialPacket_t *frame, uint8_t bytedata) 
{
    if( frame->flag == 1) {
        return;
    }
    switch(frame->state) 
	{
        case FRAME_STATE_HEADER_1:
            if (bytedata == 0xA5) {
                frame->state = FRAME_STATE_HEADER_2;
            }
            break;
        case FRAME_STATE_HEADER_2:
            if (bytedata == 0x5A) {
                frame->calc_xor = 0;
                frame->state = FRAME_STATE_LENGTH;
            } else if (bytedata == 0xA5) {
                frame->state = FRAME_STATE_HEADER_2;
            } else {
                frame->state = FRAME_STATE_HEADER_1;
            }
            break;
        case FRAME_STATE_LENGTH:
            frame->len = bytedata;
            if (frame->len == 0) {
                frame->state = FRAME_STATE_CHECKSUM;
            } else {
                frame->cnt = 0;
                frame->state = FRAME_STATE_DATA;
            }
            break;
        case FRAME_STATE_DATA:
            if (frame->cnt < frame->Buf_Size) {
                frame->Buf[frame->cnt++] = bytedata;
                frame->calc_xor ^= bytedata;
                if (frame->cnt == frame->len) {
                    frame->state = FRAME_STATE_CHECKSUM;
                }
            } else {
                frame->state = FRAME_STATE_HEADER_1;
            }
            break;
        case FRAME_STATE_CHECKSUM:
            frame->recv_xor = bytedata;
//            if (frame->recv_xor == frame->calc_xor) {
                frame->state = FRAME_STATE_FOOTER;
//            } else {
//                frame->state = FRAME_STATE_HEADER_1;
//            }
            break;
        case FRAME_STATE_FOOTER:
            if (bytedata == 0xEE) {
                frame->state = FRAME_STATE_HEADER_1;
                frame->flag = 1;
            } else if (bytedata == 0xA5) {
                frame->state = FRAME_STATE_HEADER_2;
            } else {
                frame->state = FRAME_STATE_HEADER_1;
            }
            break;
        default:
            frame->state = FRAME_STATE_HEADER_1;
            break;
    }
}


void Serial_Data_Analysis(const uint8_t *datas, uint8_t len) 
{
	//根据需要处理数据
	if( len == 5 && datas[0] == 0x01){
		Servo_Output0.target_duty 		= 50+(200*datas[1])/180;
		Servo_Output0.Timer.TickPeroid 	= datas[2]*10;
		Servo_Output1.target_duty 		= 50+(200*datas[3])/180;
		Servo_Output1.Timer.TickPeroid 	= datas[4]*10;
	}
}


