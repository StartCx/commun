#ifndef __STACK_H
#define __STACK_H

#include "core.h"


void S_StackEmpty(char buf[], int *pMove, uint16_t Stack_Size);
char S_StackPush(char buf[], int *pMove, uint16_t Queue_Size, char *value);
char S_StackPop(char buf[], int *pMove, uint16_t Queue_Size, char *value);
uint16_t S_StackLength(int *pMove);



#define StackEmpty(p)	   		S_StackEmpty(&(p)->buf[0], &(p)->pMove, sizeof((p)->buf))
#define StackPush(p, value)	   	S_StackPush(&(p)->buf[0], &(p)->pMove, sizeof((p)->buf), value)
#define StackPop(p, value)	   	S_StackPop(&(p)->buf[0], &(p)->pMove, sizeof((p)->buf), value)
#define StackLength(p)			S_StackLength(&(p)->pMove)



typedef struct { char buf[16];   int pMove; } Stack16_t;
typedef struct { char buf[32];   int pMove; } Stack32_t;
typedef struct { char buf[64];   int pMove; } Stack64_t;
typedef struct { char buf[128];  int pMove; } Stack128_t;
typedef struct { char buf[256];  int pMove; } Stack256_t;
typedef struct { char buf[512];  int pMove; } Stack512_t;
typedef struct { char buf[1024]; int pMove; } Stack1024_t;


#endif
