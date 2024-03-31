#include "queue.h"


/***************************************************************************************************************************/

inline void S_QueueEmpty(uint16_t *front, uint16_t *tail,uint16_t *length)
{
	*front = *tail = 0;
	*length= 0;
}


uint16_t S_QueueDataIn(uint16_t *front, uint16_t *tail, uint16_t *length, uint8_t data[], uint16_t Queue_Size, uint8_t *value)
{
	if (*length > 0 && *front == *tail) {
		return 0;
	}
	*length += 1;
	data[*tail] = *value;
	*tail = (*tail + 1) % Queue_Size;
	return 1;
}



uint16_t S_QueueDataOut(uint16_t *front, uint16_t *tail, uint16_t *length, uint8_t data[], uint16_t Queue_Size, uint8_t *value)
{	
	if (*length){
		__disable_irq();
		*length -= 1;	
		*value = data[*front];
		*front = (*front + 1) % Queue_Size;
		__enable_irq();
		return 1;
	}
	return 0;
}


inline uint16_t S_QueueLength(uint16_t *length)
{
	return *length;
}



/***************************************************************************************************************************/
