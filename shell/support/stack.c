#include "stack.h"



void S_StackEmpty(char buf[], int *pMove, uint16_t Stack_Size)
{
	*pMove = 0;
	buf[*pMove] = '\0';
}


char S_StackPush(char buf[], int *pMove, uint16_t Stack_Size, char *value)
{
	if (*pMove < Stack_Size)
	{
		buf[*pMove] = *value;// ´æ´¢ch£¬µÈÐ§ÓÚ *p = ch; p++;
		(*pMove)++;
		buf[*pMove] = '\0';
		return 1;
	}
	return 0;
}


char S_StackPop(char buf[], int *pMove, uint16_t Stack_Size, char *value)
{
	if (*pMove > 0)
	{
		(*pMove)--;
		*value = buf[*pMove];
		buf[*pMove] = '\0';
		return 1;
	}
	return 0;
}



uint16_t S_StackLength(int *pMove)
{
	return (*pMove);
}

