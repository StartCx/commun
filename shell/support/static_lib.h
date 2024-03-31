#ifndef __LIB_STRING_H
#define __LIB_STRING_H

#include "core.h"
#include <stdbool.h>
#include <stdarg.h>

/**************************************************strtok***************************************************************************/
enum
{
	CALL_STRTOK_PROC_GROUP0 = 0,
	CALL_STRTOK_PROC_GROUP1,
	CALL_STRTOK_PROC_GROUP2,
	CALL_STRTOK_PROC_GROUP2_1,
	CALL_STRTOK_PROC_GROUP3,
	CALL_STRTOK_PROC_GROUP4,
	CALL_STRTOK_PROC_GROUP5,
	CALL_STRTOK_PROC_ENDP,
	CALL_STRTOK_SUM,
};


typedef struct
{
	CPU_RegisterClass_t  Register;
	char* ret_ptr;
	char* begin_ptr;
	char* end_ptr;
	char* cur_ptr;
}strtok_t;

void string_token(strtok_t *strtok, char* str, char delim);

/**************************************************strtok***************************************************************************/


/**************************************************strcmp***************************************************************************/

enum
{
	CALL_STRCMP_PROC_GROUP0 = 0,
	CALL_STRCMP_PROC_GROUP1,
	CALL_STRCMP_PROC_GROUP2,
	CALL_STRCMP_PROC_ENDP,
	CALL_STRCMP_SUM,
};


typedef struct
{
	CPU_RegisterClass_t  Register;
	char *desc;
	char *src;
}strcmp_t;

void string_compare(strcmp_t *strcmp, char *src, char *desc);


/**************************************************strcmp***************************************************************************/


/**************************************************strcpy***************************************************************************/

enum
{
	CALL_STRCPY_PROC_GROUP0 = 0,
	CALL_STRCPY_PROC_GROUP1,
	CALL_STRCPY_PROC_ENDP,
	CALL_STRCPY_SUM,
};

typedef struct
{
	CPU_RegisterClass_t  Register;
	char *desc;
	char *src;
	char *dst;
}strcpy_t;

void string_copy( strcpy_t *strcpy, char *src, char *desc);

/**************************************************strcpy***************************************************************************/


/**************************************************sprintf***************************************************************************/

enum
	{
		CALL_SPRINTF_PROC_GROUP0 = 0,
		CALL_SPRINTF_PROC_GROUP1,
		CALL_SPRINTF_PROC_GROUP2,
		CALL_SPRINTF_PROC_GROUP3,
		CALL_SPRINTF_PROC_GROUP4,
		CALL_SPRINTF_PROC_GROUP4_1,
		CALL_SPRINTF_PROC_GROUP5,
		CALL_SPRINTF_PROC_GROUP6,
		CALL_SPRINTF_PROC_GROUP7,
		CALL_SPRINTF_PROC_GROUP8,
		CALL_SPRINTF_PROC_GROUP8_1,
		CALL_SPRINTF_PROC_GROUP8_2,
		CALL_SPRINTF_PROC_GROUP8_3,
		CALL_SPRINTF_PROC_GROUP9,
		CALL_SPRINTF_PROC_GROUP10,
		CALL_SPRINTF_PROC_GROUP11,
		CALL_SPRINTF_PROC_GROUP12,
		CALL_SPRINTF_PROC_GROUP13,
		CALL_SPRINTF_PROC_GROUP14,
		CALL_SPRINTF_PROC_GROUP15,
		CALL_SPRINTF_PROC_GROUP16,
		CALL_SPRINTF_PROC_GROUP17,
		
		CALL_SPRINTF_PROC_GROUP18,
		CALL_SPRINTF_PROC_GROUP19,
		CALL_SPRINTF_PROC_GROUP20,
		CALL_SPRINTF_PROC_GROUP21,
		
		CALL_SPRINTF_PROC_GROUP22,
		CALL_SPRINTF_PROC_GROUP23,
		CALL_SPRINTF_PROC_GROUP23_1,
		CALL_SPRINTF_PROC_GROUP24,
		CALL_SPRINTF_PROC_GROUP25,
		CALL_SPRINTF_PROC_GROUP26,
		CALL_SPRINTF_PROC_GROUP27,
		
		CALL_SPRINTF_PROC_GROUP28,
		CALL_SPRINTF_PROC_GROUP29,
		CALL_SPRINTF_PROC_GROUP29_1,
		CALL_SPRINTF_PROC_GROUP29_2,
		CALL_SPRINTF_PROC_GROUP29_2_1,
		CALL_SPRINTF_PROC_GROUP29_2_2,
		CALL_SPRINTF_PROC_GROUP29_3,
		CALL_SPRINTF_PROC_GROUP29_3_1,
		CALL_SPRINTF_PROC_GROUP29_4,
		CALL_SPRINTF_PROC_GROUP30,
		CALL_SPRINTF_PROC_GROUP31,
		CALL_SPRINTF_PROC_GROUP32,
		CALL_SPRINTF_PROC_GROUP33,
		CALL_SPRINTF_PROC_GROUP34,
		CALL_SPRINTF_PROC_GROUP35,
		CALL_SPRINTF_PROC_GROUP35_1,
		CALL_SPRINTF_PROC_GROUP35_2,
		CALL_SPRINTF_PROC_GROUP35_3,
		CALL_SPRINTF_PROC_GROUP35_4,
		
		
		CALL_INT2STR_PROC_GROUP0,
		CALL_INT2STR_PROC_GROUP1,
		CALL_INT2STR_PROC_GROUP2,
		CALL_INT2STR_PROC_GROUP3,
		CALL_INT2STR_PROC_GROUP4,
		CALL_INT2STR_PROC_GROUP5,
		CALL_INT2STR_PROC_ENDP,
		
		
		CALL_SPRINTF_PROC_ENDP,
		CALL_SPRINTF_SUM,
	};
	
typedef struct
{
	char      ch;
	char      *pString;
	int32_t   Length;	//长度
	int32_t   Witch;	//宽度
	int32_t   Preci;	//精度
	int32_t   int32_Value;
	uint32_t  uint32_Value;
	float32_t float32_Value;
	uint32_t  tempUint;
	int32_t   tempInt;
	float32_t tempFloat;
	int32_t   float2Int;
	int32_t	  Reversed;
	va_list   args;
	CPU_RegisterClass_t  Register;
	char temp_buffer[48];//浮点、整数临时缓存
	char str[100];		 //字符串存储位置
}fmt_t;

void string_printf(fmt_t *fmt, const char *format, ...);
/**************************************************sprintf***************************************************************************/


/**************************************************sscanf***************************************************************************/

enum
{
	CALL_SSCANF_PROC_GROUP0 = 0,
	CALL_SSCANF_PROC_GROUP1,
	CALL_SSCANF_PROC_GROUP2,
	CALL_SSCANF_PROC_GROUP3,
	CALL_SSCANF_PROC_GROUP4,
	CALL_SSCANF_PROC_GROUP5,
	CALL_SSCANF_PROC_GROUP6,
	CALL_SSCANF_PROC_GROUP7,
	CALL_SSCANF_PROC_ENDP,
	CALL_SSCANF_SUM,
};

typedef struct
{
	CPU_RegisterClass_t  Register;
	char      	ch;
	char      	*str;
	int 		base;
}sscanf_t;

void string_scanf(sscanf_t *sscanf, char *str);
/**************************************************sscanf***************************************************************************/

#endif

