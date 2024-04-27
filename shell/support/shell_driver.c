#include "shell_driver.h"


extern void shell_Cmd_Init(Shell_Device_Class_t *Shell, Usart_Bus_e Bus);

Shell_Device_Class_t Shell_Device = {
	.Shell_Driver = {
		.Register.R12_Lock = CORE_UNLOCK,
		.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP0,
	},
	.Timer = {
		.TickPeroid = 20,
	},
	.Init 	= shell_Cmd_Init,
	.Driver = shell_Driver,
};


char KeyboardInterrupt(Shell_Device_Class_t *Shell)
{
	if( QueueLength(&Shell->Shell_Driver.Shell_Queue) > 0){
		QueueEmpty(&Shell->Shell_Driver.Shell_Queue);
		return 1;
	}
	return 0;
}


void shell_Driver(Shell_Device_Class_t *Shell)
{
	static const void *function[SHELL_STATE_SUM] = {
		
		[SHELL_PASSWORD_AUTH_GROUP0] 			= &&SHELL_PASSWORD_AUTH_GROUP0,
		[SHELL_PASSWORD_AUTH_GROUP1] 			= &&SHELL_PASSWORD_AUTH_GROUP1,
		[SHELL_PASSWORD_AUTH_GROUP2] 			= &&SHELL_PASSWORD_AUTH_GROUP2,
		[SHELL_PASSWORD_AUTH_GROUP3] 			= &&SHELL_PASSWORD_AUTH_GROUP3,
		[SHELL_PASSWORD_AUTH_GROUP3_1] 			= &&SHELL_PASSWORD_AUTH_GROUP3_1,
		[SHELL_PASSWORD_AUTH_GROUP3_2] 			= &&SHELL_PASSWORD_AUTH_GROUP3_2,
		[SHELL_PASSWORD_AUTH_GROUP4] 			= &&SHELL_PASSWORD_AUTH_GROUP4,
		[SHELL_PASSWORD_AUTH_GROUP5] 			= &&SHELL_PASSWORD_AUTH_GROUP5,
		[SHELL_PASSWORD_AUTH_GROUP6] 			= &&SHELL_PASSWORD_AUTH_GROUP6,
		[SHELL_PASSWORD_AUTH_GROUP7] 			= &&SHELL_PASSWORD_AUTH_GROUP7,
		[SHELL_PASSWORD_AUTH_GROUP8] 			= &&SHELL_PASSWORD_AUTH_GROUP8,
		
		[SHELL_GET_QUEUE_DATA] 					= &&SHELL_GET_QUEUE_DATA,
		
		[SHELL_ARROW_KEYS_DETECT_GROUP0] 		= &&SHELL_ARROW_KEYS_DETECT_GROUP0,
		[SHELL_ARROW_KEYS_DETECT_GROUP1] 		= &&SHELL_ARROW_KEYS_DETECT_GROUP1,
		[SHELL_ARROW_KEYS_DETECT_GROUP2] 		= &&SHELL_ARROW_KEYS_DETECT_GROUP2,
		
		[SHELL_HISTORY_UP_RECALL_GROUP0] 		= &&SHELL_HISTORY_UP_RECALL_GROUP0,
		[SHELL_HISTORY_DOWN_RECALL_GROUP0] 		= &&SHELL_HISTORY_DOWN_RECALL_GROUP0,
		[SHELL_HISTORY_RECALL_GROUP1] 			= &&SHELL_HISTORY_RECALL_GROUP1,
		[SHELL_HISTORY_RECALL_GROUP2] 			= &&SHELL_HISTORY_RECALL_GROUP2,
		[SHELL_HISTORY_RECALL_GROUP3] 			= &&SHELL_HISTORY_RECALL_GROUP3,
		[SHELL_HISTORY_RECALL_GROUP4] 			= &&SHELL_HISTORY_RECALL_GROUP4,
		[SHELL_HISTORY_RECALL_GROUP5] 			= &&SHELL_HISTORY_RECALL_GROUP5,//10
		[SHELL_HISTORY_RECALL_GROUP6] 			= &&SHELL_HISTORY_RECALL_GROUP6,
		[SHELL_HISTORY_RECALL_GROUP7] 			= &&SHELL_HISTORY_RECALL_GROUP7,
		
		[SHELL_CURSOR_PTR_LEFT_SHIFT] 			= &&SHELL_CURSOR_PTR_LEFT_SHIFT,
		[SHELL_CURSOR_PTR_RIGHT_SHIFT] 			= &&SHELL_CURSOR_PTR_RIGHT_SHIFT,
		
		[SHELL_BACKSPACE_LEFT_SHIFT_GROUP0]  	= &&SHELL_BACKSPACE_LEFT_SHIFT_GROUP0,
		[SHELL_BACKSPACE_LEFT_SHIFT_GROUP1]  	= &&SHELL_BACKSPACE_LEFT_SHIFT_GROUP1,
		[SHELL_BACKSPACE_LEFT_SHIFT_GROUP2]  	= &&SHELL_BACKSPACE_LEFT_SHIFT_GROUP2,
		[SHELL_BACKSPACE_LEFT_SHIFT_GROUP3]  	= &&SHELL_BACKSPACE_LEFT_SHIFT_GROUP3,
		
		[SHELL_PUSH_STACK_DATA] 				= &&SHELL_PUSH_STACK_DATA,
		[SHELL_PUSH_PACKET_TAIL_0D_CMP] 		= &&SHELL_PUSH_PACKET_TAIL_0D_CMP,
	
		[SHELL_HISTORY_BACKUP_GROUP0] 			= &&SHELL_HISTORY_BACKUP_GROUP0,//20
		[SHELL_HISTORY_BACKUP_GROUP1] 			= &&SHELL_HISTORY_BACKUP_GROUP1,
		[SHELL_HISTORY_BACKUP_GROUP2] 			= &&SHELL_HISTORY_BACKUP_GROUP2,
		[SHELL_HISTORY_BACKUP_GROUP3] 			= &&SHELL_HISTORY_BACKUP_GROUP3,
		[SHELL_HISTORY_BACKUP_GROUP4] 			= &&SHELL_HISTORY_BACKUP_GROUP4,
		
		[SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP0] 	= &&SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP0,
		[SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP1] 	= &&SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP1,
		[SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP2] 	= &&SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP2,
		[SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP3] 	= &&SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP3,
		
		[SHELL_STRTOK_PROC_GROUP0_INIT]  		= &&SHELL_STRTOK_PROC_GROUP0_INIT,
		[SHELL_STRTOK_PROC_GROUP0_INIT2]  		= &&SHELL_STRTOK_PROC_GROUP0_INIT2,
		[SHELL_STRTOK_PROC_GROUP0_INIT3]  		= &&SHELL_STRTOK_PROC_GROUP0_INIT3,
		
		[SHELL_STRTOK_PROC_GROUP0]  			= &&SHELL_STRTOK_PROC_GROUP0,
		[SHELL_STRTOK_PROC_GROUP1]  			= &&SHELL_STRTOK_PROC_GROUP1,//30
		[SHELL_STRTOK_PROC_GROUP2]  			= &&SHELL_STRTOK_PROC_GROUP2,
		[SHELL_LIST_EACH_ENTRY_GROUP0]  		= &&SHELL_LIST_EACH_ENTRY_GROUP0,
		[SHELL_LIST_EACH_ENTRY_GROUP1]  		= &&SHELL_LIST_EACH_ENTRY_GROUP1,
		[SHELL_LIST_EACH_ENTRY_GROUP2]  		= &&SHELL_LIST_EACH_ENTRY_GROUP2,
		[SHELL_LIST_EACH_ENTRY_GROUP3]  		= &&SHELL_LIST_EACH_ENTRY_GROUP3,
		
		[SHELL_STRCMP_PROC_ENDP_GROUP]  		= &&SHELL_STRCMP_PROC_ENDP_GROUP,
		[SHELL_STRCPY_PROC_ENDP_GROUP]  		= &&SHELL_STRCPY_PROC_ENDP_GROUP,
		
		[SHELL_EXEC_ECHO_SHELL]   				= &&SHELL_EXEC_ECHO_SHELL,
		[SHELL_EXEC_ECHO_BACKSPACE]   			= &&SHELL_EXEC_ECHO_BACKSPACE,
		[SHELL_EXEC_ECHO_ERROR]  				= &&SHELL_EXEC_ECHO_ERROR,//40
		
		[SHELL_EXEC_ECHO_PRINTF]  				= &&SHELL_EXEC_ECHO_PRINTF,//40
		
		[SHELL_CALL_SLOT_FUNC_GROUP0]  			= &&SHELL_CALL_SLOT_FUNC_GROUP0,
		[SHELL_CALL_SLOT_FUNC_GROUP1]  			= &&SHELL_CALL_SLOT_FUNC_GROUP1,
		[SHELL_CALL_SLOT_FUNC_GROUP2]  			= &&SHELL_CALL_SLOT_FUNC_GROUP2,
		
		[SHELL_PROC_ENDP]   					= &&SHELL_PROC_ENDP,
	};
	goto *function[Shell->Shell_Driver.Register.R15_PC];
SHELL_PASSWORD_AUTH_GROUP0:
	if( Shell->Shell_Driver.Register.R12_Lock == CORE_UNLOCK){
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}else if( KeyboardInterrupt(Shell) == 1){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP1;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP1:
	string_printf(&Shell->Shell_fmt, "\r\nLogin Password: ");
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R14_LR = SHELL_PASSWORD_AUTH_GROUP2;
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP2:
	if( QueueDataOut(&Shell->Shell_Driver.Shell_Queue, &Shell->Shell_Driver.Register.R2_cin)){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP3;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP3:
	if( Shell->Shell_Driver.Register.R2_cin == 0x0D){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP3_1;
	}else if( Shell->Shell_Driver.Register.R2_cin >= 0x20 && Shell->Shell_Driver.Register.R2_cin <= 0x80){
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R2_cin);
		StackPush(&Shell->Shell_Driver.Shell_Stack, (char *)&Shell->Shell_Driver.Register.R2_cin);
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP2;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP2;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP3_1:
	if( QueueDataOut(&Shell->Shell_Driver.Shell_Queue, &Shell->Shell_Driver.Register.R2_cin)){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP3_2;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP3_2:
	if( Shell->Shell_Driver.Register.R2_cin == 0x0A){
		if( Shell->Shell_Driver.Shell_Stack.pMove == 0){
			Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP2;
		}else{
			Shell->Shell_Driver.Register.R14_LR = SHELL_PASSWORD_AUTH_GROUP4;
			Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT;
		}
	}else{
		StackEmpty(&Shell->Shell_Driver.Shell_Stack);
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP7;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP4:
	string_compare(&Shell->Shell_Strcmp, Shell->Shell_Driver.Shell_Args.argv[0], (char *)&SHELL_USER_PASSWORD);
	if( Shell->Shell_Strcmp.Register.R15_PC == CALL_STRCMP_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP5;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP5:
	StackEmpty(&Shell->Shell_Driver.Shell_Stack);
	Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP6;
	return;
SHELL_PASSWORD_AUTH_GROUP6:
	if( Shell->Shell_Strcmp.Register.R0_Result != 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP7;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_PASSWORD_AUTH_GROUP8;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP7:
	string_printf(&Shell->Shell_fmt, "\r\n\r\n## Error: Authentication failure\r\n");
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R14_LR = SHELL_PASSWORD_AUTH_GROUP1;
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_PASSWORD_AUTH_GROUP8:
	string_printf(&Shell->Shell_fmt, "\r\n\r\n## shell login successful.\r\n");
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R14_LR = SHELL_EXEC_ECHO_SHELL;
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_GET_QUEUE_DATA://max 2us
	if( QueueDataOut(&Shell->Shell_Driver.Shell_Queue, &Shell->Shell_Driver.Register.R2_cin)){
		Shell->Shell_Driver.Register.R15_PC = Shell->Shell_Driver.Register.R4_Status;
	}
	return;
SHELL_ARROW_KEYS_DETECT_GROUP0:
	if( Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_HEAD1_0x1B){
		Shell->Shell_Driver.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP1;
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}else{
		Shell->Shell_Driver.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP0;
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_DATA;
	}
	return;
SHELL_ARROW_KEYS_DETECT_GROUP1:
	if( Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_HEAD2_0x5B){
		Shell->Shell_Driver.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP2;
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}else{
		Shell->Shell_Driver.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP0;
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_DATA;
	}
	return;
SHELL_ARROW_KEYS_DETECT_GROUP2:
	Shell->Shell_Driver.Register.R4_Status = SHELL_ARROW_KEYS_DETECT_GROUP0;
	if(Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_UP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_UP_RECALL_GROUP0;
	}else if(Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_DOWN){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_DOWN_RECALL_GROUP0;
	}else if(Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_LEFT){
		Shell->Shell_Driver.Register.R15_PC = SHELL_CURSOR_PTR_LEFT_SHIFT;
	}else if(Shell->Shell_Driver.Register.R2_cin == FUNC_KEY_RIGHT){
		Shell->Shell_Driver.Register.R15_PC = SHELL_CURSOR_PTR_RIGHT_SHIFT;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_HISTORY_UP_RECALL_GROUP0:
	if( Shell->Shell_Driver.hist_Up_Down_Mark == 1 || Shell->Shell_Driver.hist_Prev_IDX != Shell->Shell_Driver.hist_End_IDX){
		Shell->Shell_Driver.hist_Up_Down_Mark =  0;
		Shell->Shell_Driver.Cursor_offset = Shell->Shell_Driver.Cursor_ptr;
		Shell->Shell_Driver.Cursor_ptr = Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Prev_IDX].pMove;
		Shell->Shell_Driver.hist_Trace_Back = Shell->Shell_Driver.hist_Prev_IDX;
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP1;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_HISTORY_DOWN_RECALL_GROUP0:
	if((Shell->Shell_Driver.hist_Up_Down_Mark == 0 && Shell->Shell_Driver.hist_Saved > 2)|| Shell->Shell_Driver.hist_Next_IDX != Shell->Shell_Driver.hist_Cur_IDX){
		Shell->Shell_Driver.hist_Up_Down_Mark  = 1;
		Shell->Shell_Driver.Cursor_offset = Shell->Shell_Driver.Cursor_ptr;
		Shell->Shell_Driver.Cursor_ptr = Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Next_IDX].pMove;
		Shell->Shell_Driver.hist_Trace_Back = Shell->Shell_Driver.hist_Next_IDX;
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP1;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_HISTORY_RECALL_GROUP1:
	if( StackLength(&Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Trace_Back]) > 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP2;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_HISTORY_RECALL_GROUP2:
	string_compare(&Shell->Shell_Strcmp, &Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Trace_Back].buf[0], &Shell->Shell_Driver.Shell_Stack.buf[0]);
	if( Shell->Shell_Strcmp.Register.R15_PC == CALL_STRCMP_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP3;
	}
	return;
	
SHELL_HISTORY_RECALL_GROUP3:
	if( Shell->Shell_Strcmp.Register.R0_Result != 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP4;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_HISTORY_RECALL_GROUP4:	
	if( Shell->Shell_Driver.Cursor_offset < Shell->Shell_Driver.Shell_Stack.pMove){//将光标移动到末尾
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Cursor_offset]);//2us
		Shell->Shell_Driver.Cursor_offset++;	
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP5;
	}
	return;
SHELL_HISTORY_RECALL_GROUP5:
	Shell->Shell_Driver.Register.R1_Index = 0;	
	if( StackPop(&Shell->Shell_Driver.Shell_Stack, (char *)&Shell->Shell_Driver.Register.R2_cin)){//清空栈
		Shell->Shell_Driver.Register.R14_LR = SHELL_HISTORY_RECALL_GROUP5;
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_BACKSPACE;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP6;
	}
	return;
SHELL_HISTORY_RECALL_GROUP6:
	Shell->Shell_Driver.Register.R3_cout = Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Trace_Back].buf[Shell->Shell_Driver.Register.R1_Index];
	if( Shell->Shell_Driver.Register.R3_cout != 0){
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_Driver.Register.R3_cout);
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_RECALL_GROUP7;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_HISTORY_RECALL_GROUP7:
	Shell->Shell_Driver.Register.R3_cout = Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Trace_Back].buf[Shell->Shell_Driver.Register.R1_Index];
	if( Shell->Shell_Driver.Register.R3_cout != 0){
		StackPush(&Shell->Shell_Driver.Shell_Stack, (char *)&Shell->Shell_Driver.Register.R3_cout);
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.hist_Prev_IDX = (Shell->Shell_Driver.hist_Trace_Back - 1 + SHELL_HISTORY_MAX) % SHELL_HISTORY_MAX;
		Shell->Shell_Driver.hist_Next_IDX = (Shell->Shell_Driver.hist_Trace_Back + 1)% SHELL_HISTORY_MAX;
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_CURSOR_PTR_LEFT_SHIFT:
	Shell->Shell_Driver.Register.R3_cout = '\b';
	if( Shell->Shell_Driver.Cursor_ptr > 0) {
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R3_cout);
		Shell->Shell_Driver.Cursor_ptr--;
	}
	Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	return;
SHELL_CURSOR_PTR_RIGHT_SHIFT:
	if( Shell->Shell_Driver.Cursor_ptr < Shell->Shell_Driver.Shell_Stack.pMove) {
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Cursor_ptr]);
		Shell->Shell_Driver.Cursor_ptr++;
	}
	Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	return;
SHELL_PUSH_STACK_DATA:
	if( Shell->Shell_Driver.Register.R2_cin == 0x08){//光标左移 BACKSPACE
		Shell->Shell_Driver.Register.R15_PC = SHELL_BACKSPACE_LEFT_SHIFT_GROUP0;
	}else if( Shell->Shell_Driver.Register.R2_cin == 0x0D){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_PACKET_TAIL_0D_CMP;
	}else if( Shell->Shell_Driver.Register.R2_cin >= 0x20 && Shell->Shell_Driver.Register.R2_cin <= 0x80){
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP0;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_PUSH_PACKET_TAIL_0D_CMP:
	if( QueueDataOut(&Shell->Shell_Driver.Shell_Queue, &Shell->Shell_Driver.Register.R2_cin)){
		if( Shell->Shell_Driver.Register.R2_cin == 0x0A){
			Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_BACKUP_GROUP0;
		}else{
			Shell->Shell_Driver.Cursor_ptr = 0; //光标位置软件清零
			Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_ERROR;
		}
	}
	return;
SHELL_HISTORY_BACKUP_GROUP0:
	Shell->Shell_Driver.hist_Up_Down_Mark = 1;
	Shell->Shell_Driver.Cursor_ptr = 0; //光标位置软件清零
	Shell->Shell_Driver.hist_Prev_IDX = (Shell->Shell_Driver.hist_Cur_IDX-1+SHELL_HISTORY_MAX)% SHELL_HISTORY_MAX;
	if( StackLength(&Shell->Shell_Driver.Shell_Stack) > 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_BACKUP_GROUP1;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_SHELL;
	}
	return;
SHELL_HISTORY_BACKUP_GROUP1:
	string_compare(&Shell->Shell_Strcmp, &Shell->Shell_Driver.Shell_Stack.buf[0], &Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Prev_IDX].buf[0]);
	if( Shell->Shell_Strcmp.Register.R15_PC == CALL_STRCMP_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_BACKUP_GROUP2;
	}
	return;
	
SHELL_HISTORY_BACKUP_GROUP2:
	if( Shell->Shell_Strcmp.Register.R0_Result != 0){
		StackEmpty(&Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Cur_IDX]);
		Shell->Shell_Driver.Register.R1_Index = 0;
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_BACKUP_GROUP3;
	}else{
		Shell->Shell_Driver.Register.R14_LR = SHELL_LIST_EACH_ENTRY_GROUP0;
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT;
	}
	return;
SHELL_HISTORY_BACKUP_GROUP3:	
	Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Cur_IDX].pMove = Shell->Shell_Driver.Shell_Stack.pMove;
	Shell->Shell_Driver.Register.R14_LR = SHELL_HISTORY_BACKUP_GROUP4;
	Shell->Shell_Driver.Register.R15_PC = SHELL_STRCPY_PROC_ENDP_GROUP;
	return;
SHELL_HISTORY_BACKUP_GROUP4:
	Shell->Shell_Driver.hist_Prev_IDX = Shell->Shell_Driver.hist_Cur_IDX;
	do
	{
		Shell->Shell_Driver.hist_Cur_IDX = (Shell->Shell_Driver.hist_Cur_IDX+1) % SHELL_HISTORY_MAX;
		Shell->Shell_Driver.hist_Next_IDX = Shell->Shell_Driver.hist_Cur_IDX;
	}
	while(0);
	
	if( Shell->Shell_Driver.hist_Saved < SHELL_HISTORY_MAX){
		Shell->Shell_Driver.hist_Saved++;
		Shell->Shell_Driver.hist_End_IDX = SHELL_HISTORY_MAX-1;
	}else{
		Shell->Shell_Driver.hist_End_IDX = Shell->Shell_Driver.hist_Prev_IDX;
	}
	Shell->Shell_Driver.Register.R14_LR = SHELL_LIST_EACH_ENTRY_GROUP0;
	Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT;
	return;
SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP0:
	if( Shell->Shell_Driver.Shell_Stack.pMove == sizeof(Shell->Shell_Driver.Shell_Stack.buf)-1){ //压栈的栈溢出判断
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}else{
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Shell_Stack.pMove - Shell->Shell_Driver.Cursor_ptr;
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP1;//开始压栈执行
	}
	return;
SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP1:
	if( Shell->Shell_Driver.Register.R1_Index > 0){
		Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Cursor_ptr+Shell->Shell_Driver.Register.R1_Index] = Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Cursor_ptr+Shell->Shell_Driver.Register.R1_Index -1];//数据向右移位
		Shell->Shell_Driver.Register.R1_Index--;
	}else{
		Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Cursor_ptr++]  = Shell->Shell_Driver.Register.R2_cin;
		Shell->Shell_Driver.Shell_Stack.buf[++Shell->Shell_Driver.Shell_Stack.pMove] = 0;
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Cursor_ptr - 1;
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP2;
	}
	return;
SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP2:
	if( Shell->Shell_Driver.Register.R1_Index < Shell->Shell_Driver.Shell_Stack.pMove){
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Register.R1_Index]);
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Shell_Stack.pMove - Shell->Shell_Driver.Cursor_ptr;
		Shell->Shell_Driver.Register.R15_PC = SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP3;
	}
	return;
SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP3:	
	Shell->Shell_Driver.Register.R3_cout = '\b';
	if( Shell->Shell_Driver.Register.R1_Index > 0){
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R3_cout);
		Shell->Shell_Driver.Register.R1_Index--;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_BACKSPACE_LEFT_SHIFT_GROUP0:
	if( Shell->Shell_Driver.Cursor_ptr > 0 && Shell->Shell_Driver.Shell_Stack.pMove > 0){
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Cursor_ptr;
		Shell->Shell_Driver.Register.R15_PC = SHELL_BACKSPACE_LEFT_SHIFT_GROUP1;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_BACKSPACE_LEFT_SHIFT_GROUP1:
	if( Shell->Shell_Driver.Register.R1_Index < Shell->Shell_Driver.Shell_Stack.pMove){
		Shell->Shell_Driver.Shell_Stack.buf[ Shell->Shell_Driver.Register.R1_Index - 1] = Shell->Shell_Driver.Shell_Stack.buf[ Shell->Shell_Driver.Register.R1_Index ];
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.Shell_Stack.pMove--;
		Shell->Shell_Driver.Cursor_ptr--;
		Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Shell_Stack.pMove] = '\0';
		Shell->Shell_Driver.Register.R3_cout = '\b';
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R3_cout);	//shell显示光标移动到被删除的位置
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Cursor_ptr;	//将stack从光标位置往后更新
		Shell->Shell_Driver.Register.R15_PC = SHELL_BACKSPACE_LEFT_SHIFT_GROUP2;
	}
	return;
SHELL_BACKSPACE_LEFT_SHIFT_GROUP2:
	if( Shell->Shell_Driver.Register.R1_Index < Shell->Shell_Driver.Shell_Stack.pMove){
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_Driver.Shell_Stack.buf[Shell->Shell_Driver.Register.R1_Index]);//显示数据更新
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.Register.R3_cout = ' ';
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R3_cout); //将最后一个位置的数据填充为空格
		Shell->Shell_Driver.Register.R15_PC = SHELL_BACKSPACE_LEFT_SHIFT_GROUP3;
		Shell->Shell_Driver.Register.R1_Index = Shell->Shell_Driver.Shell_Stack.pMove - Shell->Shell_Driver.Cursor_ptr + 1;//设置需要回退个数
	}
	return;
SHELL_BACKSPACE_LEFT_SHIFT_GROUP3:
	Shell->Shell_Driver.Register.R3_cout = '\b';
	if( Shell->Shell_Driver.Register.R1_Index > 0){
		QueueDataIn(&Shell->Shell_Print_Queue, &Shell->Shell_Driver.Register.R3_cout);
		Shell->Shell_Driver.Register.R1_Index--;
	}else{
		Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	}
	return;
SHELL_STRTOK_PROC_GROUP0_INIT:
	Shell->Shell_Driver.Register.Pointer = NULL;
	string_token( &Shell->Shell_Strtok, &(Shell->Shell_Driver.Shell_Stack.buf[0]), ':');
	if( Shell->Shell_Strtok.Register.R15_PC == CALL_STRTOK_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT2;
	}
	return;
SHELL_STRTOK_PROC_GROUP0_INIT2:
	if( Shell->Shell_Strtok.ret_ptr != NULL){
		Shell->Shell_Driver.Register.Pointer = (void*)Shell->Shell_Strtok.ret_ptr;
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT3;
	}else{
		if( Shell->Shell_Driver.Register.Pointer == NULL){
			Shell->Shell_Driver.Register.Pointer = (void*)&(Shell->Shell_Driver.Shell_Stack.buf[0]);
		}
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0;
	}
	return;
SHELL_STRTOK_PROC_GROUP0_INIT3:
	string_token( &Shell->Shell_Strtok, NULL, ':');
	if( Shell->Shell_Strtok.Register.R15_PC == CALL_STRTOK_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP0_INIT2;
	}
	return;
SHELL_STRTOK_PROC_GROUP0:
	Shell->Shell_Driver.Shell_Args.argc = 0;
	string_token( &Shell->Shell_Strtok, (char *)Shell->Shell_Driver.Register.Pointer, ' ');
	if( Shell->Shell_Strtok.Register.R15_PC == CALL_STRTOK_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP1;
	}
	return;
SHELL_STRTOK_PROC_GROUP1:
	if( Shell->Shell_Strtok.ret_ptr != NULL && Shell->Shell_Driver.Shell_Args.argc < ARGC_MAX){
		Shell->Shell_Driver.Shell_Args.argv[ Shell->Shell_Driver.Shell_Args.argc ] = Shell->Shell_Strtok.ret_ptr;
		Shell->Shell_Driver.Shell_Args.argc++;
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP2;
	}else{
		Shell->Shell_Driver.Register.R15_PC = Shell->Shell_Driver.Register.R14_LR;
	}
	return;
SHELL_STRTOK_PROC_GROUP2:
    string_token( &Shell->Shell_Strtok, NULL, ' ');
	if( Shell->Shell_Strtok.Register.R15_PC == CALL_STRTOK_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_STRTOK_PROC_GROUP1;
	}
	return;
SHELL_LIST_EACH_ENTRY_GROUP0:
	Shell->Shell_Driver.pList = list_entry((&Shell->Shell_List_Header)->next, typeof(*Shell->Shell_Driver.pList), list);
	Shell->Shell_Driver.Register.R15_PC = SHELL_LIST_EACH_ENTRY_GROUP1;
	return;
SHELL_LIST_EACH_ENTRY_GROUP1:
	Shell->Shell_Driver.Register.R1_Index = 0;
	if( &Shell->Shell_Driver.pList->list != (&Shell->Shell_List_Header)){
		Shell->Shell_Driver.Register.R15_PC = SHELL_LIST_EACH_ENTRY_GROUP3;
	}else if( Shell->Shell_Driver.Shell_Args.argc == 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_SHELL;
	}else {
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_ERROR;
	}
	return;
SHELL_LIST_EACH_ENTRY_GROUP2:
	if( Shell->Shell_Strcmp.Register.R0_Result == 0){
		Shell->Shell_Driver.Register.R15_PC = SHELL_CALL_SLOT_FUNC_GROUP0;
	}else{
		Shell->Shell_Driver.pList = list_entry(Shell->Shell_Driver.pList->list.next, typeof(*Shell->Shell_Driver.pList), list);
		Shell->Shell_Driver.Register.R15_PC = SHELL_LIST_EACH_ENTRY_GROUP1;
	}
	return;
SHELL_LIST_EACH_ENTRY_GROUP3://max 4us
	string_compare(&Shell->Shell_Strcmp, Shell->Shell_Driver.Shell_Args.argv[0], Shell->Shell_Driver.pList->name);
	if( Shell->Shell_Strcmp.Register.R15_PC == CALL_STRCMP_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_LIST_EACH_ENTRY_GROUP2;
	}
	return;
SHELL_STRCMP_PROC_ENDP_GROUP:
SHELL_STRCPY_PROC_ENDP_GROUP:
	string_copy(&Shell->Shell_Strcpy, &Shell->Shell_Driver.Shell_Stack.buf[0], &Shell->Shell_Driver.hist_Stack[Shell->Shell_Driver.hist_Cur_IDX].buf[0]);
	if( Shell->Shell_Strcpy.Register.R15_PC == CALL_STRCPY_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_HISTORY_BACKUP_GROUP4;
	}
	return;
SHELL_CALL_SLOT_FUNC_GROUP0:
	Shell->Shell_Driver.Register.R0_Result = Shell->Shell_Driver.pList->exec(Shell->Shell_Driver.Shell_Args.argc,Shell->Shell_Driver.Shell_Args.argv);
	Shell->Shell_Driver.Register.R15_PC = SHELL_CALL_SLOT_FUNC_GROUP1;
	return;
SHELL_CALL_SLOT_FUNC_GROUP1:
	if( Shell->Shell_Driver.Register.R0_Result == CORE_DONE){
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_SHELL;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}else if( Shell->Shell_Driver.Register.R0_Result == CORE_RUNNING){
		Shell->Shell_Driver.Register.R15_PC = SHELL_CALL_SLOT_FUNC_GROUP0;
	}else if( Shell->Shell_Driver.Register.R0_Result == CORE_EXIT){
		StackEmpty(&Shell->Shell_Driver.Shell_Stack);
		Shell->Shell_Driver.Register.R15_PC = SHELL_CALL_SLOT_FUNC_GROUP2;
	}
	return;
SHELL_CALL_SLOT_FUNC_GROUP2:
	string_printf(&Shell->Shell_fmt, " \x1b[H\x1b[J## shell logout successful.\r\n");
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R14_LR = SHELL_PASSWORD_AUTH_GROUP0;
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_EXEC_ECHO_SHELL:
	string_printf(&Shell->Shell_fmt, "%s", SHELL_USER);
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R14_LR = SHELL_PROC_ENDP;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_EXEC_ECHO_BACKSPACE:
	string_printf(&Shell->Shell_fmt, "\b \b");
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_EXEC_ECHO_ERROR://max 4us
	string_printf(&Shell->Shell_fmt, SHELL_CMD_ERROR);
	if( Shell->Shell_fmt.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell->Shell_Driver.Register.R15_PC = SHELL_EXEC_ECHO_PRINTF;
		Shell->Shell_Driver.Register.R14_LR = SHELL_EXEC_ECHO_SHELL;
		Shell->Shell_Driver.Register.R1_Index = 0;
	}
	return;
SHELL_EXEC_ECHO_PRINTF:
	if( Shell->Shell_fmt.str[Shell->Shell_Driver.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell->Shell_Print_Queue, (uint8_t*)&Shell->Shell_fmt.str[Shell->Shell_Driver.Register.R1_Index]);
		Shell->Shell_Driver.Register.R1_Index++;
	}else{
		Shell->Shell_Driver.Register.R15_PC = Shell->Shell_Driver.Register.R14_LR;
	}
	return;
SHELL_PROC_ENDP://max 3us
	StackEmpty(&Shell->Shell_Driver.Shell_Stack);
	Shell->Shell_Driver.Register.R15_PC = SHELL_GET_QUEUE_DATA;
	return;
}

void shell_Putchar(Shell_Device_Class_t *Shell)
{
	enum
	{
		PUTCHAR_GET_QUEUE_DATA = 0,
		PUTCHAR_WAIT_FLAG_RESET,
		PUTCHAR_PROCESS_DATA,
		PUTCHAR_SUM,
	};
	static const void *function[PUTCHAR_SUM] = {
		[PUTCHAR_GET_QUEUE_DATA] = &&PUTCHAR_GET_QUEUE_DATA,
		[PUTCHAR_WAIT_FLAG_RESET]= &&PUTCHAR_WAIT_FLAG_RESET,
		[PUTCHAR_PROCESS_DATA]   = &&PUTCHAR_PROCESS_DATA,
	};
	goto *function[Shell->Shell_putchar.R15_PC];
PUTCHAR_GET_QUEUE_DATA:
	if( QueueDataOut(&(Shell->Shell_Print_Queue), &Shell->Shell_putchar.R3_cout)){
		Shell->Shell_putchar.R15_PC = PUTCHAR_WAIT_FLAG_RESET;
	}
	return;
PUTCHAR_WAIT_FLAG_RESET:
	if( USART_GetFlagStatus(Shell->USARTx,USART_FLAG_TXE) == SET){
		Shell->Shell_putchar.R15_PC = PUTCHAR_PROCESS_DATA;
	}
	return;
PUTCHAR_PROCESS_DATA:
	USART_SendData(Shell->USARTx, Shell->Shell_putchar.R3_cout);
	Shell->Shell_putchar.R15_PC = PUTCHAR_GET_QUEUE_DATA;
	return;
}

void shell_SIM_Uart_Putchar(Shell_Device_Class_t *Shell)
{
	enum
	{
		PUTCHAR_GET_QUEUE_DATA = 0,
		PUTCHAR_WAIT_FLAG_RESET,
		PUTCHAR_PROCESS_DATA,
		PUTCHAR_SUM,
	};
	static const void *function[PUTCHAR_SUM] = {
		[PUTCHAR_GET_QUEUE_DATA] = &&PUTCHAR_GET_QUEUE_DATA,
		[PUTCHAR_WAIT_FLAG_RESET]= &&PUTCHAR_WAIT_FLAG_RESET,
		[PUTCHAR_PROCESS_DATA]   = &&PUTCHAR_PROCESS_DATA,
	};
	goto *function[Shell->Shell_putchar.R15_PC];
PUTCHAR_GET_QUEUE_DATA:
	if( QueueDataOut(&(Shell->Shell_Print_Queue), &Shell->Shell_putchar.R3_cout)){
		Shell->Shell_putchar.R15_PC = PUTCHAR_WAIT_FLAG_RESET;
	}
	return;
PUTCHAR_WAIT_FLAG_RESET:
	if( SIM_UART_GetFlagStatus( Shell->SIM_UART_Driver, SIM_USART_TX_STATE) == CORE_IDLE){
		Shell->Shell_putchar.R15_PC = PUTCHAR_PROCESS_DATA;
	}
	return;
PUTCHAR_PROCESS_DATA:
	SIM_USART_SendData( Shell->SIM_UART_Driver, Shell->Shell_putchar.R3_cout);
	Shell->Shell_putchar.R15_PC = PUTCHAR_GET_QUEUE_DATA;
	return;
}

//获取数据
void shell_SIM_Uart_Getchar(Shell_Device_Class_t *Shell)
{
	QueueDataIn(&Shell_Device.Shell_Driver.Shell_Queue, &SIM_UART.Rx_Data);
}

//获取数据
void shell_Getchar(Shell_Device_Class_t *Shell)
{
	if( USART_GetFlagStatus(Shell->USARTx,USART_FLAG_RXNE) != RESET){
		uint8_t temp = USART_ReceiveData(Shell->USARTx);//1s/115200*10=86.8us
		QueueDataIn(&Shell->Shell_Driver.Shell_Queue, &temp);
	}
}


//中断获取
void shell_Getchar_IT(Shell_Device_Class_t *Shell)
{
	
	if(USART_GetITStatus(Shell->USARTx,USART_IT_RXNE) != RESET)
	{							
		uint8_t temp = USART_ReceiveData(Shell->USARTx);
		QueueDataIn(&Shell->Shell_Driver.Shell_Queue, &temp);
		USART_ClearITPendingBit(Shell->USARTx,USART_IT_RXNE);
	}
}






