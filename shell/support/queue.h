#ifndef __QUEUE_H
#define __QUEUE_H

#include "core.h"


void S_QueueEmpty(uint16_t *front, uint16_t *tail,uint16_t *length);
uint16_t S_QueueDataIn(uint16_t *front, uint16_t *tail,uint16_t *length, uint8_t data[], uint16_t Queue_Size, uint8_t*value);
uint16_t S_QueueDataOut(uint16_t *front, uint16_t *tail, uint16_t *length, uint8_t data[], uint16_t Queue_Size, uint8_t *value);
uint16_t S_QueueLength(uint16_t *length);


#define QueueEmpty(p)	   			S_QueueEmpty  (&(p)->front, &(p)->tail, &(p)->length)
#define QueueDataIn(p, value)		S_QueueDataIn (&(p)->front, &(p)->tail, &(p)->length, &(p)->data[0], sizeof((p)->data), value)
#define QueueDataOut(p, value)	  	S_QueueDataOut(&(p)->front, &(p)->tail, &(p)->length, &(p)->data[0], sizeof((p)->data), value)
#define QueueLength(p)				S_QueueLength (&(p)->length)


typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[16];}   Queue16_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[32];}   Queue32_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[64];}   Queue64_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[128];}  Queue128_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[256];}  Queue256_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[512];}  Queue512_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[1048];} Queue1048_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[2048];} Queue2048_t;
typedef struct{uint16_t front; uint16_t tail; uint16_t length; uint8_t data[4096];} Queue4096_t;


#endif
