#include "static_lib.h"

/**************************************************strtok***************************************************************************/

void string_token(strtok_t *strtok, char *str, char delim)
{
	static const void *function[CALL_STRTOK_SUM] = {
		[CALL_STRTOK_PROC_GROUP0] = &&CALL_STRTOK_PROC_GROUP0,
		[CALL_STRTOK_PROC_GROUP1] = &&CALL_STRTOK_PROC_GROUP1,
		[CALL_STRTOK_PROC_GROUP2] = &&CALL_STRTOK_PROC_GROUP2,
		[CALL_STRTOK_PROC_GROUP2_1]= &&CALL_STRTOK_PROC_GROUP2_1,//bug补丁
		[CALL_STRTOK_PROC_GROUP3] = &&CALL_STRTOK_PROC_GROUP3,
		[CALL_STRTOK_PROC_GROUP4] = &&CALL_STRTOK_PROC_GROUP4,
		[CALL_STRTOK_PROC_GROUP5] = &&CALL_STRTOK_PROC_GROUP5,
		[CALL_STRTOK_PROC_ENDP]   = &&CALL_STRTOK_PROC_ENDP,
	};
	goto *function[strtok->Register.R15_PC];
CALL_STRTOK_PROC_GROUP0:
	strtok->cur_ptr = str;
	strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP1;
	return;
CALL_STRTOK_PROC_GROUP1:
    if( strtok->cur_ptr == NULL) {// 如果传入的 str 为 NULL，则继续使用上次的分割部分
        strtok->cur_ptr = strtok->end_ptr;
    }
	strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP2;
	return;
CALL_STRTOK_PROC_GROUP2:
	if( strtok->cur_ptr == NULL){//指向NULL,错误返回
		strtok->ret_ptr = NULL;
		strtok->Register.R15_PC = CALL_STRTOK_PROC_ENDP;//直接退出-状态转换
	}else{
		strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP2_1;
	}
	return;
CALL_STRTOK_PROC_GROUP2_1:
	if( *strtok->cur_ptr == delim) {// 跳过起始的空格字符
        strtok->cur_ptr++;
    }else{
		strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP3;
	}
	return;
CALL_STRTOK_PROC_GROUP3://2us
    if( *strtok->cur_ptr == '\0') {// 如果字符串已经遍历完，则返回 NULL
		strtok->ret_ptr = NULL;
		strtok->Register.R15_PC = CALL_STRTOK_PROC_ENDP;//直接退出-状态转换
    }else{
		strtok->begin_ptr = strtok->cur_ptr;// 找到分割部分的起始位置
		strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP4;
	}
	return;	
CALL_STRTOK_PROC_GROUP4:
    if( *strtok->cur_ptr != '\0' && *strtok->cur_ptr != delim) {// 寻找分割部分的结束位置
        strtok->cur_ptr++;
    }else {
		strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP5;
	}
	return;
CALL_STRTOK_PROC_GROUP5://max 3us
    if( *strtok->cur_ptr == delim) {// 如果找到了分隔符，则替换为字符串结束符
        *strtok->cur_ptr = '\0';
        strtok->end_ptr = strtok->cur_ptr + 1;
    } else {
        strtok->end_ptr = NULL; // 如果字符串已经遍历完，则将 last_segment_end 指向 NULL
    }
	strtok->ret_ptr = strtok->begin_ptr;
	strtok->Register.R15_PC = CALL_STRTOK_PROC_ENDP;//直接退出-状态转换
	return;
CALL_STRTOK_PROC_ENDP:
	strtok->Register.R15_PC = CALL_STRTOK_PROC_GROUP0;
	return;
}

/**************************************************strtok***************************************************************************/


/**************************************************strcmp***************************************************************************/


void string_compare(strcmp_t *strcmp, char *src, char *desc) 
{
	static const void *function[CALL_STRCMP_SUM] = {
		[CALL_STRCMP_PROC_GROUP0] = &&CALL_STRCMP_PROC_GROUP0,
		[CALL_STRCMP_PROC_GROUP1] = &&CALL_STRCMP_PROC_GROUP1,
		[CALL_STRCMP_PROC_GROUP2] = &&CALL_STRCMP_PROC_GROUP2,
		[CALL_STRCMP_PROC_ENDP]   = &&CALL_STRCMP_PROC_ENDP,
	};
	goto *function[strcmp->Register.R15_PC];
CALL_STRCMP_PROC_GROUP0:
	strcmp->Register.R1_Index = 0;
	strcmp->src  = src;
	strcmp->desc = desc;
	strcmp->Register.R15_PC = CALL_STRCMP_PROC_GROUP1;
	return;
CALL_STRCMP_PROC_GROUP1:
	strcmp->Register.R2_cin = *(strcmp->desc+strcmp->Register.R1_Index);
	strcmp->Register.R3_cout = *(strcmp->src +strcmp->Register.R1_Index);
	if( strcmp->Register.R2_cin != 0 && (strcmp->Register.R2_cin == strcmp->Register.R3_cout)){
        strcmp->Register.R1_Index++;
    }else{
		strcmp->Register.R15_PC = CALL_STRCMP_PROC_GROUP2;
	}
	return;
CALL_STRCMP_PROC_GROUP2:
    strcmp->Register.R0_Result = strcmp->Register.R2_cin-strcmp->Register.R3_cout;
	strcmp->Register.R15_PC = CALL_STRCMP_PROC_ENDP;
	return;
CALL_STRCMP_PROC_ENDP:
	strcmp->Register.R15_PC = CALL_STRCMP_PROC_GROUP0;
	return;
}


/**************************************************strcmp***************************************************************************/


/**************************************************strcpy***************************************************************************/


void string_copy( strcpy_t *strcpy, char *src, char *desc)
{
	static const void *function[CALL_STRCPY_SUM] = {
		[CALL_STRCPY_PROC_GROUP0] = &&CALL_STRCPY_PROC_GROUP0,
		[CALL_STRCPY_PROC_GROUP1] = &&CALL_STRCPY_PROC_GROUP1,
		[CALL_STRCPY_PROC_ENDP]   = &&CALL_STRCPY_PROC_ENDP,
	};
	goto *function[strcpy->Register.R15_PC];
CALL_STRCPY_PROC_GROUP0:
	strcpy->src	 = src;
	strcpy->desc = desc;
	strcpy->Register.R15_PC = CALL_STRCPY_PROC_GROUP1;
	return;
CALL_STRCPY_PROC_GROUP1:
	if( *strcpy->src != '\0') {
		*strcpy->desc = *strcpy->src;
		strcpy->desc++;
		strcpy->src++;
	}else{
		*strcpy->desc = '\0'; // 添加字符串结束标志
		strcpy->Register.R15_PC = CALL_STRCPY_PROC_ENDP;
	}
	return;
CALL_STRCPY_PROC_ENDP:
	strcpy->Register.R15_PC = CALL_STRCPY_PROC_GROUP0;
	return;
}


/**************************************************strcpy***************************************************************************/

/**************************************************sprintf***************************************************************************/
void string_printf(fmt_t *fmt, const char *format, ...) 
{
	static const void* function[CALL_SPRINTF_SUM] = {
		[CALL_SPRINTF_PROC_GROUP0] = &&CALL_SPRINTF_PROC_GROUP0,
		[CALL_SPRINTF_PROC_GROUP1] = &&CALL_SPRINTF_PROC_GROUP1,
		[CALL_SPRINTF_PROC_GROUP2] = &&CALL_SPRINTF_PROC_GROUP2,
		[CALL_SPRINTF_PROC_GROUP3] = &&CALL_SPRINTF_PROC_GROUP3,
		[CALL_SPRINTF_PROC_GROUP4] = &&CALL_SPRINTF_PROC_GROUP4,
		[CALL_SPRINTF_PROC_GROUP4_1] = &&CALL_SPRINTF_PROC_GROUP4_1,
		[CALL_SPRINTF_PROC_GROUP5] = &&CALL_SPRINTF_PROC_GROUP5,
		[CALL_SPRINTF_PROC_GROUP6] = &&CALL_SPRINTF_PROC_GROUP6,
		[CALL_SPRINTF_PROC_GROUP7] = &&CALL_SPRINTF_PROC_GROUP7,
		[CALL_SPRINTF_PROC_GROUP8] = &&CALL_SPRINTF_PROC_GROUP8,
		[CALL_SPRINTF_PROC_GROUP8_1] = &&CALL_SPRINTF_PROC_GROUP8_1,
		[CALL_SPRINTF_PROC_GROUP8_2] = &&CALL_SPRINTF_PROC_GROUP8_2,
		[CALL_SPRINTF_PROC_GROUP8_3] = &&CALL_SPRINTF_PROC_GROUP8_3,
		
		[CALL_SPRINTF_PROC_GROUP9] = &&CALL_SPRINTF_PROC_GROUP9,
		[CALL_SPRINTF_PROC_GROUP10] = &&CALL_SPRINTF_PROC_GROUP10,
		[CALL_SPRINTF_PROC_GROUP11] = &&CALL_SPRINTF_PROC_GROUP11,
		[CALL_SPRINTF_PROC_GROUP12] = &&CALL_SPRINTF_PROC_GROUP12,
		[CALL_SPRINTF_PROC_GROUP13] = &&CALL_SPRINTF_PROC_GROUP13,
		[CALL_SPRINTF_PROC_GROUP14] = &&CALL_SPRINTF_PROC_GROUP14,
		
		[CALL_SPRINTF_PROC_GROUP15] = &&CALL_SPRINTF_PROC_GROUP15,
		[CALL_SPRINTF_PROC_GROUP16] = &&CALL_SPRINTF_PROC_GROUP16,
		[CALL_SPRINTF_PROC_GROUP17] = &&CALL_SPRINTF_PROC_GROUP17,
		[CALL_SPRINTF_PROC_GROUP18] = &&CALL_SPRINTF_PROC_GROUP18,
		[CALL_SPRINTF_PROC_GROUP19] = &&CALL_SPRINTF_PROC_GROUP19,
		[CALL_SPRINTF_PROC_GROUP20] = &&CALL_SPRINTF_PROC_GROUP20,
		[CALL_SPRINTF_PROC_GROUP21] = &&CALL_SPRINTF_PROC_GROUP21,
		[CALL_SPRINTF_PROC_GROUP22] = &&CALL_SPRINTF_PROC_GROUP22,
		[CALL_SPRINTF_PROC_GROUP23] = &&CALL_SPRINTF_PROC_GROUP23,
		[CALL_SPRINTF_PROC_GROUP23_1] = &&CALL_SPRINTF_PROC_GROUP23_1,
		[CALL_SPRINTF_PROC_GROUP24] = &&CALL_SPRINTF_PROC_GROUP24,
		[CALL_SPRINTF_PROC_GROUP25] = &&CALL_SPRINTF_PROC_GROUP25,
		[CALL_SPRINTF_PROC_GROUP26] = &&CALL_SPRINTF_PROC_GROUP26,
		[CALL_SPRINTF_PROC_GROUP27] = &&CALL_SPRINTF_PROC_GROUP27,
		
		[CALL_SPRINTF_PROC_GROUP28] = &&CALL_SPRINTF_PROC_GROUP28,
		[CALL_SPRINTF_PROC_GROUP29] = &&CALL_SPRINTF_PROC_GROUP29,
		[CALL_SPRINTF_PROC_GROUP29_1] = &&CALL_SPRINTF_PROC_GROUP29_1,
		[CALL_SPRINTF_PROC_GROUP29_2] = &&CALL_SPRINTF_PROC_GROUP29_2,
		[CALL_SPRINTF_PROC_GROUP29_3] = &&CALL_SPRINTF_PROC_GROUP29_3,
		[CALL_SPRINTF_PROC_GROUP29_3_1] = &&CALL_SPRINTF_PROC_GROUP29_3_1,
		[CALL_SPRINTF_PROC_GROUP29_4] = &&CALL_SPRINTF_PROC_GROUP29_4,
		
		[CALL_SPRINTF_PROC_GROUP30] = &&CALL_SPRINTF_PROC_GROUP30,
		[CALL_SPRINTF_PROC_GROUP31] = &&CALL_SPRINTF_PROC_GROUP31,
		[CALL_SPRINTF_PROC_GROUP32] = &&CALL_SPRINTF_PROC_GROUP32,
		[CALL_SPRINTF_PROC_GROUP33] = &&CALL_SPRINTF_PROC_GROUP33,
		[CALL_SPRINTF_PROC_GROUP34] = &&CALL_SPRINTF_PROC_GROUP34,
		[CALL_SPRINTF_PROC_GROUP35] = &&CALL_SPRINTF_PROC_GROUP35,	
		[CALL_SPRINTF_PROC_GROUP35_1] = &&CALL_SPRINTF_PROC_GROUP35_1,	
		[CALL_SPRINTF_PROC_GROUP35_2] = &&CALL_SPRINTF_PROC_GROUP35_2,	
		[CALL_SPRINTF_PROC_GROUP35_3] = &&CALL_SPRINTF_PROC_GROUP35_3,	
		[CALL_SPRINTF_PROC_GROUP35_4] = &&CALL_SPRINTF_PROC_GROUP35_4,	
		[CALL_INT2STR_PROC_GROUP0] = &&CALL_INT2STR_PROC_GROUP0,
		[CALL_INT2STR_PROC_GROUP1] = &&CALL_INT2STR_PROC_GROUP1,
		[CALL_INT2STR_PROC_GROUP2] = &&CALL_INT2STR_PROC_GROUP2,
		[CALL_INT2STR_PROC_GROUP3] = &&CALL_INT2STR_PROC_GROUP3,
		[CALL_INT2STR_PROC_GROUP4] = &&CALL_INT2STR_PROC_GROUP4,
		[CALL_INT2STR_PROC_GROUP5] = &&CALL_INT2STR_PROC_GROUP5,
		[CALL_INT2STR_PROC_ENDP]   = &&CALL_INT2STR_PROC_ENDP,
		
		[CALL_SPRINTF_PROC_ENDP]   = &&CALL_SPRINTF_PROC_ENDP,
	};
	goto *function[fmt->Register.R15_PC];
CALL_SPRINTF_PROC_GROUP0:
	va_start(fmt->args, format);
	fmt->Register.R6_Count = 0;
	fmt->Register.R1_Index = 0;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP1;
	return;
CALL_SPRINTF_PROC_GROUP1:	
	if( format[fmt->Register.R6_Count] != '\0'){
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP2;
	}else{
		fmt->str[fmt->Register.R1_Index] = '\0';
		fmt->Register.R0_Result = fmt->Register.R1_Index;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_ENDP;
	}
	return;
CALL_SPRINTF_PROC_GROUP2:	
	if( format[fmt->Register.R6_Count] == '%'){
		fmt->Register.R6_Count++;
		fmt->Witch = 0;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP4;
	}else{
		fmt->str[fmt->Register.R1_Index] = format[fmt->Register.R6_Count];
		fmt->Register.R1_Index++;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	}
	return;
CALL_SPRINTF_PROC_GROUP3:	
	fmt->Register.R6_Count++;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP1;
	return;
CALL_SPRINTF_PROC_GROUP4://宽度计算
	if( format[fmt->Register.R6_Count] >= '0' && format[fmt->Register.R6_Count] <= '9') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP4_1;
	}else{
		fmt->Preci = 4; //解析精度,初始值4
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP5;
	}
	return;
CALL_SPRINTF_PROC_GROUP4_1:
	fmt->Witch = fmt->Witch * 10 + (format[fmt->Register.R6_Count] - '0');
	fmt->Register.R6_Count++;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP4;
	return;
CALL_SPRINTF_PROC_GROUP5:  
	if( format[fmt->Register.R6_Count] == '.'){
		fmt->Register.R6_Count++;
		fmt->Preci = 0;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP6;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP7;
	}
	return;
CALL_SPRINTF_PROC_GROUP6://精度计算
	if( format[fmt->Register.R6_Count] >= '0' && format[fmt->Register.R6_Count] <= '9') {
		fmt->Preci = fmt->Preci * 10 + (format[fmt->Register.R6_Count] - '0');
		fmt->Register.R6_Count++;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP7;
	}
	return;
CALL_SPRINTF_PROC_GROUP7:	
	if( format[fmt->Register.R6_Count] == '%') {
		fmt->str[fmt->Register.R1_Index] = '%';
		fmt->Register.R1_Index++;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP8;
	}
	return;
CALL_SPRINTF_PROC_GROUP8:
	fmt->ch = format[fmt->Register.R6_Count];
	if( fmt->ch == 'c') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP9;
	}else if( fmt->ch == 's') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP12;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP8_2;
	}
	return;
CALL_SPRINTF_PROC_GROUP8_1:	
	fmt->str[fmt->Register.R1_Index] = format[fmt->Register.R6_Count];
	fmt->Register.R1_Index++;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	return;
CALL_SPRINTF_PROC_GROUP8_2:
	fmt->ch = format[fmt->Register.R6_Count];
	if( fmt->ch == 'd') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP16;
	}else if( fmt->ch == 'x') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP21;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP8_3;
	}
	return;
CALL_SPRINTF_PROC_GROUP8_3:
	fmt->ch = format[fmt->Register.R6_Count];
	if( fmt->ch == 'f') {
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP28;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP8_1;
	}
	return;
CALL_SPRINTF_PROC_GROUP9:	
	fmt->ch = (char)va_arg(fmt->args, int);
	fmt->Length = fmt->Witch - 1;
	fmt->Register.R7_ACC = 0;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP10;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP11;
	return;
CALL_SPRINTF_PROC_GROUP10:
	fmt->str[fmt->Register.R1_Index] = fmt->ch;
	fmt->Register.R1_Index++;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	return;
CALL_SPRINTF_PROC_GROUP11:	
	if( fmt->Register.R7_ACC < fmt->Length){
		fmt->str[fmt->Register.R1_Index] = ' ';
		fmt->Register.R1_Index++;
		fmt->Register.R7_ACC++;
	}else{
		fmt->Register.R7_ACC = 0;
		fmt->Register.R15_PC = fmt->Register.R14_LR;
	}
	return;
CALL_SPRINTF_PROC_GROUP12:	
	fmt->pString = va_arg(fmt->args, char*);
	fmt->Register.R7_ACC = 0;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP13;
	return;
CALL_SPRINTF_PROC_GROUP13:
	if( fmt->pString[fmt->Register.R7_ACC] != '\0'){//计算字符串长度
		fmt->Register.R7_ACC++;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP15;
	}
	return;
CALL_SPRINTF_PROC_GROUP14:
	if( fmt->pString[fmt->Register.R7_ACC] != '\0'){
		fmt->str[fmt->Register.R1_Index] = fmt->pString[fmt->Register.R7_ACC];
		fmt->Register.R1_Index++;
		fmt->Register.R7_ACC++;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	}
	return;
CALL_SPRINTF_PROC_GROUP15:
	fmt->Length = fmt->Witch - fmt->Register.R7_ACC;//打印空格个数
	fmt->Register.R7_ACC = 0;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP14;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP11;
	return;
CALL_SPRINTF_PROC_GROUP16:
	fmt->int32_Value = va_arg(fmt->args, int);
	fmt->Register.R7_ACC = 0;
	fmt->tempInt = fmt->int32_Value < 0 ? -fmt->int32_Value : fmt->int32_Value;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP17;
	return;
CALL_SPRINTF_PROC_GROUP17:
	if( fmt->tempInt != 0){
		fmt->tempInt /= 10;
		fmt->Register.R7_ACC++;
	}else{
		fmt->Length = fmt->Witch-fmt->Register.R7_ACC-(fmt->int32_Value <= 0 ? 1 : 0);
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP18;
	}
	return;
CALL_SPRINTF_PROC_GROUP18:
	fmt->Register.R7_ACC = 0;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP19;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP11;
	return;
CALL_SPRINTF_PROC_GROUP19:
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP20;
	fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP0;
	return;	
CALL_SPRINTF_PROC_GROUP20:
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	return;
CALL_SPRINTF_PROC_GROUP21:
	fmt->uint32_Value = va_arg(fmt->args, unsigned int);
	fmt->Register.R7_ACC = 0;
	fmt->tempUint = fmt->uint32_Value;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP22;
	return;
CALL_SPRINTF_PROC_GROUP22:
	fmt->tempUint >>= 4; //除以16
	fmt->Register.R7_ACC++;
	if( fmt->tempUint == 0){
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP23;
	}
	return;
CALL_SPRINTF_PROC_GROUP23:
	fmt->Length = fmt->Witch - fmt->Register.R7_ACC;
	fmt->Register.R7_ACC = 0;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP24;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP23_1;
	return;
CALL_SPRINTF_PROC_GROUP23_1:
	if( fmt->Register.R7_ACC < fmt->Length){
		fmt->str[fmt->Register.R1_Index] = '0';
		fmt->Register.R1_Index++;
		fmt->Register.R7_ACC++;
	}else{
		fmt->Register.R7_ACC = 0;
		fmt->Register.R15_PC = fmt->Register.R14_LR;
	}
	return;
CALL_SPRINTF_PROC_GROUP24:	
	fmt->Register.R7_ACC = 0;
	if( fmt->uint32_Value == 0){
		fmt->temp_buffer[fmt->Register.R7_ACC++] = '0';
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP25;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP26;
	}
	return;
CALL_SPRINTF_PROC_GROUP25:
	if( fmt->Register.R7_ACC > 0){
		fmt->str[fmt->Register.R1_Index] = fmt->temp_buffer[--fmt->Register.R7_ACC];
		fmt->Register.R1_Index++;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	}
	return;
CALL_SPRINTF_PROC_GROUP26:	
	if( fmt->uint32_Value > 0){
		fmt->tempInt = fmt->uint32_Value  & 0xF;//除以16
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP27;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP25;
	}
	return;
CALL_SPRINTF_PROC_GROUP27:
	if( fmt->tempInt < 10){
		fmt->temp_buffer[fmt->Register.R7_ACC++] = fmt->tempInt + '0';
	}else{
		fmt->temp_buffer[fmt->Register.R7_ACC++] = fmt->tempInt - 10 + 'A';
	}
	fmt->uint32_Value >>= 4;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP26;
	return;
CALL_SPRINTF_PROC_GROUP28:
	fmt->float32_Value = va_arg(fmt->args, double);
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29;
	return;
CALL_SPRINTF_PROC_GROUP29:
	fmt->Register.R8_Flag = false;
	if( fmt->float32_Value < 0){
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_1;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_2;
	}
	return;
CALL_SPRINTF_PROC_GROUP29_1:
	fmt->Register.R8_Flag = true;
	fmt->float32_Value = -fmt->float32_Value;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_2;
	return;
CALL_SPRINTF_PROC_GROUP29_2:
	fmt->Register.R7_ACC = 0;
	fmt->tempInt = (int) fmt->float32_Value;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_3;
	return;
CALL_SPRINTF_PROC_GROUP29_3:
	fmt->tempFloat = fmt->float32_Value * 1000;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_3_1;
	return;
CALL_SPRINTF_PROC_GROUP29_3_1:
	fmt->float2Int = (int) fmt->tempFloat - fmt->tempInt*1000;
	fmt->Reversed = 0;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP29_4;
	return;
CALL_SPRINTF_PROC_GROUP29_4://int型数字反转
	if( fmt->float2Int != 0){
		fmt->Reversed = fmt->Reversed * 10 + fmt->float2Int % 10;
		fmt->float2Int /= 10;
	}else{
		fmt->float2Int = fmt->Reversed;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP30;
	}
	return;
CALL_SPRINTF_PROC_GROUP30:
	if( fmt->tempInt != 0){
		fmt->tempInt /= 10;
		fmt->Register.R7_ACC++;
	}else{
		fmt->Length = fmt->Witch-fmt->Register.R7_ACC-fmt->Register.R8_Flag-1;
		fmt->Length = fmt->Length - fmt->Preci;
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP31;
	}
	return;
CALL_SPRINTF_PROC_GROUP31:
	fmt->Register.R7_ACC = 0;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP32;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP11;
	return;
CALL_SPRINTF_PROC_GROUP32:	
	if( fmt->Register.R8_Flag == true){
		fmt->str[fmt->Register.R1_Index++] = '-';
	}
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP33;
	return;
CALL_SPRINTF_PROC_GROUP33:
	fmt->int32_Value = (int) fmt->float32_Value;
	fmt->Register.R14_LR = CALL_SPRINTF_PROC_GROUP34;
	fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP0;
	return;
CALL_SPRINTF_PROC_GROUP34:
	fmt->str[fmt->Register.R1_Index++] = '.';
	fmt->Register.R7_ACC = 0;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35;
	return;
CALL_SPRINTF_PROC_GROUP35:
	if( fmt->Register.R7_ACC < fmt->Preci){
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35_1;
	}else{
		fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP3;
	}
	return;
CALL_SPRINTF_PROC_GROUP35_1:
	fmt->tempInt = (fmt->float2Int%10);
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35_2;
	return;
CALL_SPRINTF_PROC_GROUP35_2:
	fmt->str[fmt->Register.R1_Index++] = fmt->tempInt + '0';
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35_3;
	return;
CALL_SPRINTF_PROC_GROUP35_3:
	fmt->float2Int /= 10;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35_4;
	return;
CALL_SPRINTF_PROC_GROUP35_4:
	fmt->Register.R7_ACC++;
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP35;
	return;
CALL_INT2STR_PROC_GROUP0:
	fmt->Register.R7_ACC = 0;
	fmt->Register.R8_Flag = false;
	fmt->tempInt = fmt->int32_Value;
	fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP1;
	return;
CALL_INT2STR_PROC_GROUP1:
	if( fmt->tempInt == 0){
		fmt->temp_buffer[fmt->Register.R7_ACC++] = '0';
		fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP2;
	}else{
		fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP4;
	}
	return;
CALL_INT2STR_PROC_GROUP2:	
	if( fmt->Register.R8_Flag == true){
		fmt->str[fmt->Register.R1_Index] = '-';
		fmt->Register.R1_Index++;
	}
	fmt->Register.R0_Result = fmt->Register.R7_ACC;
	fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP3;
	return;
CALL_INT2STR_PROC_GROUP3:
	if( fmt->Register.R7_ACC > 0){
		fmt->str[fmt->Register.R1_Index++] = fmt->temp_buffer[--fmt->Register.R7_ACC];
	}else{
		fmt->str[fmt->Register.R1_Index] = '\0';
		fmt->Register.R15_PC = CALL_INT2STR_PROC_ENDP;
	}
	return;
CALL_INT2STR_PROC_GROUP4:
	if( fmt->tempInt < 0){
		fmt->Register.R8_Flag = true;
		fmt->tempInt = -fmt->tempInt;
	}
	fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP5;
	return;
CALL_INT2STR_PROC_GROUP5:
	if( fmt->tempInt > 0){
		fmt->temp_buffer[fmt->Register.R7_ACC++] = (fmt->tempInt % 10) + '0';
		fmt->tempInt /= 10;
	}else{
		fmt->Register.R15_PC = CALL_INT2STR_PROC_GROUP2;
	}
	return;
CALL_INT2STR_PROC_ENDP:
	fmt->Register.R15_PC = fmt->Register.R14_LR;
	return;
CALL_SPRINTF_PROC_ENDP:
	va_end(fmt->args);
	fmt->Register.R15_PC = CALL_SPRINTF_PROC_GROUP0;
	return;
}

/**************************************************sprintf***************************************************************************/


/**************************************************sscanf***************************************************************************/

void string_scanf(sscanf_t *sscanf, char *str)
{
	static const void *function[CALL_SSCANF_SUM] = {
		[CALL_SSCANF_PROC_GROUP0] = &&CALL_SSCANF_PROC_GROUP0,
		[CALL_SSCANF_PROC_GROUP1] = &&CALL_SSCANF_PROC_GROUP1,
		[CALL_SSCANF_PROC_GROUP2] = &&CALL_SSCANF_PROC_GROUP2,
		[CALL_SSCANF_PROC_GROUP3] = &&CALL_SSCANF_PROC_GROUP3,
		[CALL_SSCANF_PROC_GROUP4] = &&CALL_SSCANF_PROC_GROUP4,
		[CALL_SSCANF_PROC_GROUP5] = &&CALL_SSCANF_PROC_GROUP5,
		[CALL_SSCANF_PROC_GROUP6] = &&CALL_SSCANF_PROC_GROUP6,
		[CALL_SSCANF_PROC_GROUP7] = &&CALL_SSCANF_PROC_GROUP7,
		[CALL_SSCANF_PROC_ENDP]   = &&CALL_SSCANF_PROC_ENDP,
	};
	goto *function[sscanf->Register.R15_PC];

CALL_SSCANF_PROC_GROUP0:
	sscanf->base = 10; //默认十进制
	sscanf->Register.R1_Index = 0;
	sscanf->Register.R0_Result = 0;
	if( str != NULL){
		sscanf->str = str;
	}
	sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP1;
	return;
CALL_SSCANF_PROC_GROUP1:
	sscanf->Register.R9_Error = CORE_OK;
	if( sscanf->str[0] == '0'){
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP2;
	}else{
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP3;
	}
	return;
CALL_SSCANF_PROC_GROUP2:
	if( sscanf->str[1] == 'x' || sscanf->str[1] == 'X'){
		sscanf->base = 16; //十六进制
		sscanf->Register.R1_Index += 2;
	}else if( sscanf->str[1] == 'b' || sscanf->str[1] == 'B'){
		sscanf->base = 2;  //二进制
		sscanf->Register.R1_Index += 2;
	}
	sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP3;
	return;
CALL_SSCANF_PROC_GROUP3:
	if( sscanf->str[sscanf->Register.R1_Index] != '\0'){
		sscanf->ch = sscanf->str[sscanf->Register.R1_Index];
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP4;
	}else{
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_ENDP;
	}
	return;
CALL_SSCANF_PROC_GROUP4:
	if( sscanf->ch >= '0' && sscanf->ch <= '9'){
		sscanf->Register.R2_cin = sscanf->ch - '0';
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP7;
	}else{
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP5;
	}
	return;
CALL_SSCANF_PROC_GROUP5:
	if( sscanf->ch >= 'A' && sscanf->ch <= 'F'){
		sscanf->Register.R2_cin = sscanf->ch - 'A' + 10;
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP7;
	}else{
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP6;
	}
	return;
CALL_SSCANF_PROC_GROUP6:
	if( sscanf->ch >= 'a' && sscanf->ch <= 'f'){
		sscanf->Register.R2_cin = sscanf->ch - 'a' + 10;
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP7;
	}else{
		sscanf->Register.R9_Error = CORE_ERROR;
		sscanf->Register.R15_PC = CALL_SSCANF_PROC_ENDP;// 非法字符，退出循环
	}
	return;
CALL_SSCANF_PROC_GROUP7:
	sscanf->Register.R0_Result = sscanf->Register.R0_Result * sscanf->base + sscanf->Register.R2_cin;
	sscanf->Register.R1_Index++;
	sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP3;
	return;
	
CALL_SSCANF_PROC_ENDP:
	sscanf->Register.R15_PC = CALL_SSCANF_PROC_GROUP0;
	return;
}

/**************************************************sscanf***************************************************************************/

