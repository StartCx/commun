#ifndef __TIMELINE_H
#define __TIMELINE_H


#include "stm32f10x.h"
#include "core.h"


void IWDG_Init(void);
void IWDG_Feed(void);

extern void TIM4_Configuration(void);
extern void TIM3_Configuration(void) ;

#endif
