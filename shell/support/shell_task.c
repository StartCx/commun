#include "shell_task.h"

static Shell_Cmd_Task_t Shell_Task;



int do_clear(int argc,char *argv[])
{
	enum
	{
		CLEAR_PROC_GROUP0 = 0,
		CLEAR_PROC_GROUP1,
		CLEAR_PROC_GROUP2,
		CLEAR_PROC_GROUP3,
		CLEAR_PROC_SUM,
	};
	static const void *function[CLEAR_PROC_SUM] = {
		[CLEAR_PROC_GROUP0] 	= &&CLEAR_PROC_GROUP0,
		[CLEAR_PROC_GROUP1] 	= &&CLEAR_PROC_GROUP1,
		[CLEAR_PROC_GROUP2] 	= &&CLEAR_PROC_GROUP2,
		[CLEAR_PROC_GROUP3] 	= &&CLEAR_PROC_GROUP3,
	};
	goto *function[Shell_Task.Register.R15_PC];
CLEAR_PROC_GROUP0:
	if(argc == 1){
		Shell_Task.Register.R15_PC = CLEAR_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = CLEAR_PROC_GROUP2;
	}
	return CORE_RUNNING;
CLEAR_PROC_GROUP1:	
	string_printf(&Shell_Task.fmt_driver, " \x1b[H\x1b[J");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = CLEAR_PROC_GROUP3;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
CLEAR_PROC_GROUP2:	
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = CLEAR_PROC_GROUP3;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
CLEAR_PROC_GROUP3:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = CLEAR_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}


int do_help(int argc,char *argv[])
{
	enum
	{
		HELP_PROC_GROUP0 = 0,
		HELP_PROC_GROUP1,
		HELP_PROC_GROUP2,
		HELP_PROC_GROUP3,
		HELP_PROC_GROUP4,
		HELP_PROC_GROUP5,
		HELP_PROC_SUM,
	};
	static const void *function[HELP_PROC_SUM] = {
		[HELP_PROC_GROUP0] 	= &&HELP_PROC_GROUP0,
		[HELP_PROC_GROUP1] 	= &&HELP_PROC_GROUP1,
		[HELP_PROC_GROUP2] 	= &&HELP_PROC_GROUP2,
		[HELP_PROC_GROUP3] 	= &&HELP_PROC_GROUP3,
		[HELP_PROC_GROUP4] 	= &&HELP_PROC_GROUP4,
		[HELP_PROC_GROUP5] 	= &&HELP_PROC_GROUP5,
	};
	goto *function[Shell_Task.Register.R15_PC];
HELP_PROC_GROUP0:
	if(argc == 1){
		Shell_Task.pos = list_entry(Shell_Device.Shell_List_Header.next, typeof(*Shell_Task.pos), list);
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP2;
	}else{
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP1;
	}
	return CORE_RUNNING;
HELP_PROC_GROUP1:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP5;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
HELP_PROC_GROUP2:
	if( &Shell_Task.pos->list != (&Shell_Device.Shell_List_Header)){
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP3;
		Shell_Task.Register.R1_Index = 0;
	}else{
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
HELP_PROC_GROUP3:
	if( Shell_Task.pos->usage[Shell_Task.Register.R1_Index] != 0){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.pos->usage[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP4;
	}
	return CORE_RUNNING;
HELP_PROC_GROUP4:
	Shell_Task.pos = list_entry((&Shell_Task.pos->list)->next, typeof(*Shell_Task.pos), list);
	Shell_Task.Register.R15_PC = HELP_PROC_GROUP2;
	return CORE_RUNNING;
HELP_PROC_GROUP5:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = HELP_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}

int do_leds(int argc,char *argv[])
{
	enum
	{
		LED_PROC_GROUP0 = 0,
		LED_PROC_GROUP1,
		LED_PROC_GROUP2,
		LED_PROC_GROUP3,
		LED_PROC_GROUP4,
		LED_PROC_GROUP5,
		LED_PROC_GROUP6,
		LED_PROC_GROUP7,
		LED_PROC_GROUP7_1,
		LED_PROC_GROUP8,
		LED_PROC_GROUP8_1,
		LED_PROC_GROUP9,
		LED_PROC_GROUP10,
		LED_STATE_SUM,
	};
	static const void *function[LED_STATE_SUM] = {
		[LED_PROC_GROUP0] 	= &&LED_PROC_GROUP0,
		[LED_PROC_GROUP1] 	= &&LED_PROC_GROUP1,
		[LED_PROC_GROUP2]   = &&LED_PROC_GROUP2,
		[LED_PROC_GROUP3]   = &&LED_PROC_GROUP3,
		[LED_PROC_GROUP4] 	= &&LED_PROC_GROUP4,
		[LED_PROC_GROUP5] 	= &&LED_PROC_GROUP5,
		[LED_PROC_GROUP6] 	= &&LED_PROC_GROUP6,
		[LED_PROC_GROUP7] 	= &&LED_PROC_GROUP7,
		[LED_PROC_GROUP7_1] = &&LED_PROC_GROUP7_1,
		[LED_PROC_GROUP8] 	= &&LED_PROC_GROUP8,
		[LED_PROC_GROUP8_1] = &&LED_PROC_GROUP8_1,
		[LED_PROC_GROUP9] 	= &&LED_PROC_GROUP9,
		[LED_PROC_GROUP10] 	= &&LED_PROC_GROUP10,
	};
	goto *function[Shell_Task.Register.R15_PC];
LED_PROC_GROUP0:
	if( argc == 3){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = LED_PROC_GROUP10;
	}
	return CORE_RUNNING;
LED_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP2;
	}
	return CORE_RUNNING;
LED_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP10;
	}else{
		
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = LED_PROC_GROUP3;
	}
	return CORE_RUNNING;
LED_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object == 0){
		Shell_Task.GPIO_Output = &Led_Device;
		Shell_Task.Register.R15_PC = LED_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = LED_PROC_GROUP10;
	}
	return CORE_RUNNING;
LED_PROC_GROUP4:
	string_scanf(&Shell_Task.sscanf_driver,argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP5;
	}
	return CORE_RUNNING;
LED_PROC_GROUP5:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP10;
	}else{
		Shell_Task.Register.R4_Status = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.Register.R4_Status >= GPIO_MODE_SUM){
			Shell_Task.Register.R15_PC = LED_PROC_GROUP10;
		}else{
			Shell_Task.Register.R15_PC = LED_PROC_GROUP6;
		}
	}
	return CORE_RUNNING;
LED_PROC_GROUP6:	
	Led_Device.Set(&Led_Device, Shell_Task.Register.R4_Status);
	Shell_Task.Register.R15_PC = LED_PROC_GROUP7;
	return CORE_RUNNING;
LED_PROC_GROUP7:
	string_printf(&Shell_Task.fmt_driver, "\r\n## LED %d set to Mode %d\r\n", Shell_Task.Register.R5_Object, Shell_Task.sscanf_driver.Register.R0_Result);
	Shell_Task.Register.R15_PC = LED_PROC_GROUP7_1;
	return CORE_RUNNING;
LED_PROC_GROUP7_1:
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}else{
		Shell_Task.Register.R15_PC = LED_PROC_GROUP7;
	}
	return CORE_RUNNING;
LED_PROC_GROUP8:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: LED %d set to Mode %d\r\n", Shell_Task.Register.R5_Object, Shell_Task.sscanf_driver.Register.R0_Result);
	Shell_Task.Register.R15_PC = LED_PROC_GROUP8_1;
	return CORE_RUNNING;
LED_PROC_GROUP8_1:
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}else{
		Shell_Task.Register.R15_PC = LED_PROC_GROUP8;
	}
	return CORE_RUNNING;
LED_PROC_GROUP9:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = LED_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
LED_PROC_GROUP10:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = LED_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}

int do_top(int argc,char *argv[])
{	
	enum
	{
		TOP_PROC_GROUP0,
		TOP_PROC_GROUP1,
		TOP_PROC_GROUP1_1,
		TOP_PROC_GROUP2,
		TOP_PROC_GROUP3,
		TOP_PROC_GROUP4,
		TOP_PROC_SUM,
	};
	static const void *function[TOP_PROC_SUM] = {
		[TOP_PROC_GROUP0] 	= &&TOP_PROC_GROUP0,
		[TOP_PROC_GROUP1] 	= &&TOP_PROC_GROUP1,
		[TOP_PROC_GROUP1_1] = &&TOP_PROC_GROUP1_1,
		[TOP_PROC_GROUP2] 	= &&TOP_PROC_GROUP2,
		[TOP_PROC_GROUP3] 	= &&TOP_PROC_GROUP3,
		[TOP_PROC_GROUP4] 	= &&TOP_PROC_GROUP4,
	};
	goto *function[Shell_Task.Register.R15_PC];
TOP_PROC_GROUP0:
	if(argc == 1){
		Shell_Task.Register.R15_PC = TOP_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = TOP_PROC_GROUP2;
	}
	return CORE_RUNNING;
TOP_PROC_GROUP1:
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n## cpu   : Max = %06d0 us | cur = %06d0 us | period = %07d us.\r\n",Cur_Usage_Max_Cpu, Cur_Usage_Cpu, 100);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = TOP_PROC_GROUP3;
		Shell_Task.Register.R14_LR = TOP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
TOP_PROC_GROUP1_1:
	Shell_Task.Register.R15_PC = TOP_PROC_GROUP0;
	return CORE_DONE;
TOP_PROC_GROUP2:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = TOP_PROC_GROUP3;
		Shell_Task.Register.R14_LR = TOP_PROC_GROUP1_1;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
TOP_PROC_GROUP3:	
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
TOP_PROC_GROUP4:
	string_printf(&Shell_Task.fmt_driver, "## stack : top = 0x%x | cur = 0x%x | bottom = 0x%x.\r\n",stack_top_addr, stack_top_sp, stack_bottom_fp);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = TOP_PROC_GROUP3;
		Shell_Task.Register.R14_LR = TOP_PROC_GROUP1_1;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;

}



int do_i2cdetect(int argc,char *argv[])
{	
	enum
	{
		I2C_DETECT_PROC_GROUP0,
		I2C_DETECT_PROC_GROUP0_1,
		I2C_DETECT_PROC_GROUP0_2,
		I2C_DETECT_PROC_GROUP0_2_1,
		I2C_DETECT_PROC_GROUP0_3,
		I2C_DETECT_PROC_GROUP0_4,
		I2C_DETECT_PROC_GROUP1,
		I2C_DETECT_PROC_GROUP1_ERROR,
		I2C_DETECT_PROC_GROUP2,
		I2C_DETECT_PROC_GROUP3,
		I2C_DETECT_PROC_GROUP4,
		I2C_DETECT_PROC_GROUP5,
		I2C_DETECT_PROC_GROUP6,
		I2C_DETECT_PROC_GROUP7,
		I2C_DETECT_PROC_GROUP8,
		I2C_DETECT_PROC_GROUP9,
		I2C_DETECT_PROC_GROUP10,
		I2C_DETECT_STATE_SUM,
	};

	static const void *function[I2C_DETECT_STATE_SUM] = {
		[I2C_DETECT_PROC_GROUP0] 	= &&I2C_DETECT_PROC_GROUP0,
		
		[I2C_DETECT_PROC_GROUP0_1] 	= &&I2C_DETECT_PROC_GROUP0_1,
		[I2C_DETECT_PROC_GROUP0_2] 	= &&I2C_DETECT_PROC_GROUP0_2,
		[I2C_DETECT_PROC_GROUP0_2_1]= &&I2C_DETECT_PROC_GROUP0_2_1,
		[I2C_DETECT_PROC_GROUP0_3] 	= &&I2C_DETECT_PROC_GROUP0_3,
		[I2C_DETECT_PROC_GROUP0_4] 	= &&I2C_DETECT_PROC_GROUP0_4,
		
		[I2C_DETECT_PROC_GROUP1] 		= &&I2C_DETECT_PROC_GROUP1,
		[I2C_DETECT_PROC_GROUP1_ERROR] 	= &&I2C_DETECT_PROC_GROUP1_ERROR,
		[I2C_DETECT_PROC_GROUP2] 	= &&I2C_DETECT_PROC_GROUP2,
		[I2C_DETECT_PROC_GROUP3] 	= &&I2C_DETECT_PROC_GROUP3,
		[I2C_DETECT_PROC_GROUP4] 	= &&I2C_DETECT_PROC_GROUP4,
		[I2C_DETECT_PROC_GROUP5] 	= &&I2C_DETECT_PROC_GROUP5,
		[I2C_DETECT_PROC_GROUP6] 	= &&I2C_DETECT_PROC_GROUP6,
		[I2C_DETECT_PROC_GROUP7] 	= &&I2C_DETECT_PROC_GROUP7,
		[I2C_DETECT_PROC_GROUP8]	= &&I2C_DETECT_PROC_GROUP8,
		[I2C_DETECT_PROC_GROUP9]	= &&I2C_DETECT_PROC_GROUP9,
		[I2C_DETECT_PROC_GROUP10]	= &&I2C_DETECT_PROC_GROUP10,
	};

	goto *function[Shell_Task.Register.R15_PC];
I2C_DETECT_PROC_GROUP0:
	if(argc == 2){
		Shell_Task.Dev_Addr = 0x00;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_1;
	}else{
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_3;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP0_1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_2;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP0_2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_3;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_2_1;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP0_2_1:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP1;
		Shell_Task.Register.retry  = 1000;
	}else{
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_3;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP0_3:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0_4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP0_4:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0;
		return CORE_DONE;	
	}
	return CORE_RUNNING;	
I2C_DETECT_PROC_GROUP1:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP2;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP9;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP1_ERROR;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP1_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DETECT_PROC_GROUP9;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP2:
	I2Cx_Detect(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr);
	Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP3;
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP3:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP4;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP4:
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP5;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP10;
	}else{
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP6;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP5:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cdetect addr=0x%2x online ", Shell_Task.Dev_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DETECT_PROC_GROUP8;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP6:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cdetect addr=0x%2x offline", Shell_Task.Dev_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DETECT_PROC_GROUP8;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP7:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP8:	
	if( Shell_Task.Dev_Addr == 0xFF){
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP9;
	}else{
		Shell_Task.Dev_Addr++;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP1;
	}
	return CORE_RUNNING;
I2C_DETECT_PROC_GROUP9:	
	Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP0;
	return CORE_DONE;
I2C_DETECT_PROC_GROUP10:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DETECT_PROC_GROUP9;
		Shell_Task.Register.R15_PC = I2C_DETECT_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}



int do_i2cdump(int argc,char *argv[])
{	
	enum
	{
		I2C_DUMP_PROC_GROUP0,
		I2C_DUMP_PROC_GROUP0_1,
		I2C_DUMP_PROC_GROUP0_2,
		I2C_DUMP_PROC_GROUP0_2_1,
		I2C_DUMP_PROC_GROUP0_3,
		I2C_DUMP_PROC_GROUP1,
		I2C_DUMP_PROC_GROUP2,
		I2C_DUMP_PROC_GROUP3,
		I2C_DUMP_PROC_GROUP4,
		I2C_DUMP_PROC_GROUP5,
		I2C_DUMP_PROC_GROUP5_ERROR,
		I2C_DUMP_PROC_GROUP6,
		I2C_DUMP_PROC_GROUP7,
		I2C_DUMP_PROC_GROUP8,
		I2C_DUMP_PROC_GROUP9,
		I2C_DUMP_PROC_GROUP10,
		I2C_DUMP_PROC_GROUP11,
		I2C_DUMP_PROC_GROUP11_1,
		I2C_DUMP_PROC_GROUP12,
		I2C_DUMP_PROC_GROUP13,
		I2C_DUMP_PROC_GROUP14,
		I2C_DUMP_PROC_GROUP15,
		I2C_DUMP_PROC_GROUP16,
		I2C_DUMP_PROC_GROUP17,
		I2C_DUMP_STATE_SUM,
	};
	static const void *function[I2C_DUMP_STATE_SUM] = {
		[I2C_DUMP_PROC_GROUP0] 	= &&I2C_DUMP_PROC_GROUP0,
		[I2C_DUMP_PROC_GROUP0_1]= &&I2C_DUMP_PROC_GROUP0_1,
		[I2C_DUMP_PROC_GROUP0_2]= &&I2C_DUMP_PROC_GROUP0_2,
		[I2C_DUMP_PROC_GROUP0_2_1]= &&I2C_DUMP_PROC_GROUP0_2_1,
		[I2C_DUMP_PROC_GROUP0_3]= &&I2C_DUMP_PROC_GROUP0_3,
		[I2C_DUMP_PROC_GROUP1] 	= &&I2C_DUMP_PROC_GROUP1,
		[I2C_DUMP_PROC_GROUP2] 	= &&I2C_DUMP_PROC_GROUP2,
		[I2C_DUMP_PROC_GROUP3] 	= &&I2C_DUMP_PROC_GROUP3,
		[I2C_DUMP_PROC_GROUP4] 	= &&I2C_DUMP_PROC_GROUP4,
		[I2C_DUMP_PROC_GROUP5] 	= &&I2C_DUMP_PROC_GROUP5,
		[I2C_DUMP_PROC_GROUP5_ERROR] = &&I2C_DUMP_PROC_GROUP5_ERROR,
		[I2C_DUMP_PROC_GROUP6] 	= &&I2C_DUMP_PROC_GROUP6,
		[I2C_DUMP_PROC_GROUP7] 	= &&I2C_DUMP_PROC_GROUP7,
		[I2C_DUMP_PROC_GROUP8]	= &&I2C_DUMP_PROC_GROUP8,
		[I2C_DUMP_PROC_GROUP9]	= &&I2C_DUMP_PROC_GROUP9,
		[I2C_DUMP_PROC_GROUP10]	= &&I2C_DUMP_PROC_GROUP10,
		[I2C_DUMP_PROC_GROUP11]	= &&I2C_DUMP_PROC_GROUP11,
		[I2C_DUMP_PROC_GROUP11_1]= &&I2C_DUMP_PROC_GROUP11_1,
		[I2C_DUMP_PROC_GROUP12]	= &&I2C_DUMP_PROC_GROUP12,
		[I2C_DUMP_PROC_GROUP13]	= &&I2C_DUMP_PROC_GROUP13,
		[I2C_DUMP_PROC_GROUP14]	= &&I2C_DUMP_PROC_GROUP14,
		[I2C_DUMP_PROC_GROUP15]	= &&I2C_DUMP_PROC_GROUP15,
		[I2C_DUMP_PROC_GROUP16]	= &&I2C_DUMP_PROC_GROUP16,
		[I2C_DUMP_PROC_GROUP17]	= &&I2C_DUMP_PROC_GROUP17,
	};
	goto *function[Shell_Task.Register.R15_PC];
I2C_DUMP_PROC_GROUP0:
	if(argc == 3){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP0_1;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP0_1:	
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP0_2;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP0_2:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP13;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP0_2_1;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP0_2_1:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP0_3;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP0_3:
	Shell_Task.Reg_Addr = 0x00;
	Shell_Task.sscanf_driver.str = argv[2];
	Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP1;
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP2;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP13;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP3;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP3:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cdump: bus%d dev_addr=0x%2x Loading...\r\n\r\n", Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP5;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.retry  = 1000;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP4:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP5:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP6;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP12;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP5_ERROR;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP5_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP12;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP6:
	I2Cx_Get(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Reg_Addr, 1, &Shell_Task.Reg_Value[0], 1);
	Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP7;
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP7:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP8;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP8:	
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP9;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP10;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP9:
	string_printf(&Shell_Task.fmt_driver, "## i2cdump: reg=0x%2x val=0x%2x\r\n", Shell_Task.Reg_Addr, Shell_Task.Reg_Value[0]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP11;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP10:
	if( Shell_Task.Register.R0_Result == DEV_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP14;
	}else if( Shell_Task.Register.R0_Result == REG_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP15;
	}else if( Shell_Task.Register.R0_Result == WR_VALUE_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP16;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP17;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP11;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP11:
	if( Shell_Task.Reg_Addr == UINT8_MAX){
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP12;
	}else{
		Shell_Task.Reg_Addr++;
		Shell_Task.Register.R6_Count = 0;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP11_1;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP11_1:
	if( Shell_Task.Register.R6_Count > 0){
		Shell_Task.Register.R6_Count--;
	}else{
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP5;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP12:
	Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP0;
	return CORE_DONE;
I2C_DUMP_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP12;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP14:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: DEV_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP12;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP15:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x REG_ADDR_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP11;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP16:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x WR_VALUE_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP11;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_DUMP_PROC_GROUP17:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_DUMP_PROC_GROUP11;
		Shell_Task.Register.R15_PC = I2C_DUMP_PROC_GROUP4;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}


int do_i2cget(int argc,char *argv[])
{
	enum
	{
		I2C_GET_PROC_GROUP0,
		I2C_GET_PROC_GROUP2,
		I2C_GET_PROC_GROUP3,
		I2C_GET_PROC_GROUP4,
		I2C_GET_PROC_GROUP6,
		I2C_GET_PROC_GROUP7,
		I2C_GET_PROC_GROUP9,
		I2C_GET_PROC_GROUP10,
		I2C_GET_PROC_GROUP12,
		I2C_GET_PROC_GROUP10_Extra,
		I2C_GET_PROC_GROUP12_Extra,
		I2C_GET_PROC_GROUP13,
		I2C_GET_PROC_GROUP14,
		I2C_GET_PROC_GROUP15,
		I2C_GET_PROC_GROUP15_ERROR,
		I2C_GET_PROC_GROUP16,
		
		I2C_GET_PROC_GROUP17,
		I2C_GET_PROC_GROUP18,
		I2C_GET_PROC_GROUP19,
		I2C_GET_PROC_GROUP20,
		
		I2C_GET_PROC_GROUP20_1,
		I2C_GET_PROC_GROUP20_2,
		I2C_GET_PROC_GROUP20_3,
		I2C_GET_PROC_GROUP21,
		I2C_GET_PROC_GROUP22,
		I2C_GET_PROC_GROUP23,
		I2C_GET_PROC_GROUP24,
		I2C_GET_PROC_GROUP25,
		I2C_GET_PROC_GROUP26,
		I2C_GET_PROC_GROUP27,
		I2C_GET_STATE_SUM,
	};
	static const void *function[I2C_GET_STATE_SUM] = {
		[I2C_GET_PROC_GROUP0] 	= &&I2C_GET_PROC_GROUP0,
		[I2C_GET_PROC_GROUP2] 	= &&I2C_GET_PROC_GROUP2,
		[I2C_GET_PROC_GROUP3] 	= &&I2C_GET_PROC_GROUP3,
		[I2C_GET_PROC_GROUP4] 	= &&I2C_GET_PROC_GROUP4,
		[I2C_GET_PROC_GROUP6] 	= &&I2C_GET_PROC_GROUP6,
		[I2C_GET_PROC_GROUP7] 	= &&I2C_GET_PROC_GROUP7,
		[I2C_GET_PROC_GROUP9]	= &&I2C_GET_PROC_GROUP9,
		[I2C_GET_PROC_GROUP10]	= &&I2C_GET_PROC_GROUP10,
		[I2C_GET_PROC_GROUP12]	= &&I2C_GET_PROC_GROUP12,
		
		[I2C_GET_PROC_GROUP10_Extra]	= &&I2C_GET_PROC_GROUP10_Extra,
		[I2C_GET_PROC_GROUP12_Extra]	= &&I2C_GET_PROC_GROUP12_Extra,
		
		[I2C_GET_PROC_GROUP13]	= &&I2C_GET_PROC_GROUP13,
		[I2C_GET_PROC_GROUP14]	= &&I2C_GET_PROC_GROUP14,
		[I2C_GET_PROC_GROUP15]	= &&I2C_GET_PROC_GROUP15,
		[I2C_GET_PROC_GROUP15_ERROR]= &&I2C_GET_PROC_GROUP15_ERROR,
		[I2C_GET_PROC_GROUP16]	= &&I2C_GET_PROC_GROUP16,
		[I2C_GET_PROC_GROUP17]	= &&I2C_GET_PROC_GROUP17,
		[I2C_GET_PROC_GROUP18]	= &&I2C_GET_PROC_GROUP18,
		[I2C_GET_PROC_GROUP19]	= &&I2C_GET_PROC_GROUP19,
		[I2C_GET_PROC_GROUP20]	= &&I2C_GET_PROC_GROUP20,
		
		[I2C_GET_PROC_GROUP20_1]	= &&I2C_GET_PROC_GROUP20_1,
		[I2C_GET_PROC_GROUP20_2]	= &&I2C_GET_PROC_GROUP20_2,
		[I2C_GET_PROC_GROUP20_3]	= &&I2C_GET_PROC_GROUP20_3,
		
		[I2C_GET_PROC_GROUP21]	= &&I2C_GET_PROC_GROUP21,
		[I2C_GET_PROC_GROUP22]	= &&I2C_GET_PROC_GROUP22,
		[I2C_GET_PROC_GROUP23]	= &&I2C_GET_PROC_GROUP23,
		[I2C_GET_PROC_GROUP24]	= &&I2C_GET_PROC_GROUP24,
		[I2C_GET_PROC_GROUP25]	= &&I2C_GET_PROC_GROUP25,
		[I2C_GET_PROC_GROUP26]	= &&I2C_GET_PROC_GROUP26,
		[I2C_GET_PROC_GROUP27]	= &&I2C_GET_PROC_GROUP27,
	};
	goto *function[Shell_Task.Register.R15_PC];
I2C_GET_PROC_GROUP0:
	if(argc == 6){
		string_scanf(&Shell_Task.sscanf_driver, argv[1]);
		if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
			Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP2;
		}
	}else{
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP3;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP4:
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP6;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP6:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP7;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP7:
	string_scanf(&Shell_Task.sscanf_driver, argv[3]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP9;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP9:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP10;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP10:
	string_scanf(&Shell_Task.sscanf_driver, argv[4]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP12;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP12:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Size = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP10_Extra;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP10_Extra:
	string_scanf(&Shell_Task.sscanf_driver, argv[5]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP12_Extra;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP12_Extra:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP22;
	}else{
		Shell_Task.Value_Length = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, "\r\n## I2cget: bus%d dev=0x%2x reg=0x%2x reg_len = %d read_len=%d", Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Reg_Addr, Shell_Task.Reg_Size, Shell_Task.Value_Length);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP15;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP14;
		Shell_Task.Register.retry  = 1000;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP14:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP15:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP16;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP21;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP15_ERROR;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP15_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP16:
	I2Cx_Get(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Reg_Addr,Shell_Task.Reg_Size, &Shell_Task.Reg_Value[0], Shell_Task.Value_Length);
	Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP17;
	return CORE_RUNNING;
I2C_GET_PROC_GROUP17:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP18;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP18:
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP19;
	}else{
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP23;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP19:
	string_printf(&Shell_Task.fmt_driver, "\r\n loading...\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R6_Count = 0;
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP20_1;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP20:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP20_1:
	if( Shell_Task.Register.R6_Count == Shell_Task.Value_Length){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP21;
	}else{
		if( Shell_Task.Register.R6_Count % 8 == 0){
			Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20_3;
		}else{
			Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20_2;
		}
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP20_2:
	string_printf(&Shell_Task.fmt_driver, "0x%2x ",Shell_Task.Reg_Value[Shell_Task.Register.R6_Count]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP20_1;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP20_3:
	string_printf(&Shell_Task.fmt_driver, "\r\n[0x%2x]-> ",Shell_Task.Reg_Addr+Shell_Task.Register.R6_Count);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP20_2;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP21:
	Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP0;
	return CORE_DONE;
I2C_GET_PROC_GROUP22:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP23:
	if( Shell_Task.Register.R0_Result == DEV_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP24;
	}else if( Shell_Task.Register.R0_Result == REG_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP25;
	}else if( Shell_Task.Register.R0_Result == WR_VALUE_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP26;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP27;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP24:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: DEV_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP25:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x REG_ADDR_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP26:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x WR_VALUE_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_GET_PROC_GROUP27:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_GET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_GET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}


int do_i2cset(int argc,char *argv[])
{
	enum
	{
		I2C_SET_PROC_GROUP0,
		I2C_SET_PROC_GROUP2,
		I2C_SET_PROC_GROUP3,
		I2C_SET_PROC_GROUP4,
		I2C_SET_PROC_GROUP6,
		I2C_SET_PROC_GROUP7,
		I2C_SET_PROC_GROUP8,
		I2C_SET_PROC_GROUP9,
		I2C_SET_PROC_GROUP10,
		I2C_SET_PROC_GROUP12,
		I2C_SET_PROC_GROUP10_Extra,
		I2C_SET_PROC_GROUP12_Extra,
		I2C_SET_PROC_GROUP13,
		I2C_SET_PROC_GROUP14,
		I2C_SET_PROC_GROUP15,
		I2C_SET_PROC_GROUP15_ERROR,
		I2C_SET_PROC_GROUP16,
		
		I2C_SET_PROC_GROUP17,
		I2C_SET_PROC_GROUP18,
		I2C_SET_PROC_GROUP19,
		I2C_SET_PROC_GROUP20,
		I2C_SET_PROC_GROUP21,
		I2C_SET_PROC_GROUP22,
		I2C_SET_PROC_GROUP23,
		I2C_SET_PROC_GROUP24,
		I2C_SET_PROC_GROUP25,
		I2C_SET_PROC_GROUP26,
		I2C_SET_PROC_GROUP27,
		I2C_SET_STATE_SUM,
	};
	static const void *function[I2C_SET_STATE_SUM] = {
		[I2C_SET_PROC_GROUP0] 	= &&I2C_SET_PROC_GROUP0,
		[I2C_SET_PROC_GROUP2] 	= &&I2C_SET_PROC_GROUP2,
		[I2C_SET_PROC_GROUP3] 	= &&I2C_SET_PROC_GROUP3,
		[I2C_SET_PROC_GROUP4] 	= &&I2C_SET_PROC_GROUP4,
		[I2C_SET_PROC_GROUP6] 	= &&I2C_SET_PROC_GROUP6,
		[I2C_SET_PROC_GROUP7] 	= &&I2C_SET_PROC_GROUP7,
		[I2C_SET_PROC_GROUP9]	= &&I2C_SET_PROC_GROUP9,
		[I2C_SET_PROC_GROUP10]	= &&I2C_SET_PROC_GROUP10,
		[I2C_SET_PROC_GROUP12]	= &&I2C_SET_PROC_GROUP12,
		[I2C_SET_PROC_GROUP10_Extra]	= &&I2C_SET_PROC_GROUP10_Extra,
		[I2C_SET_PROC_GROUP12_Extra]	= &&I2C_SET_PROC_GROUP12_Extra,
		
		[I2C_SET_PROC_GROUP13]	= &&I2C_SET_PROC_GROUP13,
		[I2C_SET_PROC_GROUP14]	= &&I2C_SET_PROC_GROUP14,
		[I2C_SET_PROC_GROUP15]	= &&I2C_SET_PROC_GROUP15,
		[I2C_SET_PROC_GROUP15_ERROR]= &&I2C_SET_PROC_GROUP15_ERROR,
		[I2C_SET_PROC_GROUP16]	= &&I2C_SET_PROC_GROUP16,
		[I2C_SET_PROC_GROUP17]	= &&I2C_SET_PROC_GROUP17,
		[I2C_SET_PROC_GROUP18]	= &&I2C_SET_PROC_GROUP18,
		[I2C_SET_PROC_GROUP19]	= &&I2C_SET_PROC_GROUP19,
		[I2C_SET_PROC_GROUP20]	= &&I2C_SET_PROC_GROUP20,
		[I2C_SET_PROC_GROUP21]	= &&I2C_SET_PROC_GROUP21,
		[I2C_SET_PROC_GROUP22]	= &&I2C_SET_PROC_GROUP22,
		[I2C_SET_PROC_GROUP23]	= &&I2C_SET_PROC_GROUP23,
		[I2C_SET_PROC_GROUP24]	= &&I2C_SET_PROC_GROUP24,
		[I2C_SET_PROC_GROUP25]	= &&I2C_SET_PROC_GROUP25,
		[I2C_SET_PROC_GROUP26]	= &&I2C_SET_PROC_GROUP26,
		[I2C_SET_PROC_GROUP27]	= &&I2C_SET_PROC_GROUP27,
	};
	goto *function[Shell_Task.Register.R15_PC];
I2C_SET_PROC_GROUP0:
	if(argc == 6){
		string_scanf(&Shell_Task.sscanf_driver, argv[1]);
		if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
			Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP2;
		}
		return CORE_RUNNING;
	}else{
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP3;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP4:
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP6;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP6:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP7;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP7:
	string_scanf(&Shell_Task.sscanf_driver, argv[3]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP9;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP9:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP10;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP10:
	string_scanf(&Shell_Task.sscanf_driver, argv[4]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP12;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP12:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Size = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP10_Extra;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP10_Extra:
	string_scanf(&Shell_Task.sscanf_driver, argv[5]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP12_Extra;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP12_Extra:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Value[0] = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cset: bus%d dev=0x%2x reg=0x%2x reg_len = %d, val=0x%2x\r\n", Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Reg_Addr,Shell_Task.Reg_Size, Shell_Task.Reg_Value[0]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP15;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP14;
		Shell_Task.Register.retry  = 1000;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP14:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP15:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP16;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP21;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP15_ERROR;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP15_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP16:
	I2Cx_Set(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Reg_Addr, Shell_Task.Reg_Size, &Shell_Task.Reg_Value[0], 1);
	Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP17;
	return CORE_RUNNING;
I2C_SET_PROC_GROUP17:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP18;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP18:
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP19;
	}else{
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP23;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP19:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cset: set value success\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP20:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP21:
	Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP0;
	return CORE_DONE;
I2C_SET_PROC_GROUP22:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP23:
	if( Shell_Task.Register.R0_Result == DEV_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP24;
	}else if( Shell_Task.Register.R0_Result == REG_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP25;
	}else if( Shell_Task.Register.R0_Result == WR_VALUE_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP26;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP27;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP24:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: DEV_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP25:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x REG_ADDR_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP26:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x WR_VALUE_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_SET_PROC_GROUP27:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_SET_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_SET_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}



int do_i2cwrite(int argc,char *argv[])
{
	enum
	{
		I2C_WRITE_PROC_GROUP0,
		I2C_WRITE_PROC_GROUP2,
		I2C_WRITE_PROC_GROUP3,
		I2C_WRITE_PROC_GROUP4,
		I2C_WRITE_PROC_GROUP5,
		I2C_WRITE_PROC_GROUP6,
		I2C_WRITE_PROC_GROUP6_1,
		I2C_WRITE_PROC_GROUP7,
		I2C_WRITE_PROC_GROUP9,
		I2C_WRITE_PROC_GROUP13,
		I2C_WRITE_PROC_GROUP14,
		I2C_WRITE_PROC_GROUP15,
		I2C_WRITE_PROC_GROUP15_ERROR,
		I2C_WRITE_PROC_GROUP16,
		
		I2C_WRITE_PROC_GROUP17,
		I2C_WRITE_PROC_GROUP18,
		I2C_WRITE_PROC_GROUP19,
		I2C_WRITE_PROC_GROUP20,
		I2C_WRITE_PROC_GROUP21,
		I2C_WRITE_PROC_GROUP22,
		I2C_WRITE_PROC_GROUP23,
		I2C_WRITE_PROC_GROUP24,
		I2C_WRITE_PROC_GROUP25,
		I2C_WRITE_PROC_GROUP26,
		I2C_WRITE_PROC_GROUP27,
		I2C_WRITE_STATE_SUM,
	};
	static const void *function[I2C_WRITE_STATE_SUM] = {
		[I2C_WRITE_PROC_GROUP0] 	= &&I2C_WRITE_PROC_GROUP0,
		[I2C_WRITE_PROC_GROUP2] 	= &&I2C_WRITE_PROC_GROUP2,
		[I2C_WRITE_PROC_GROUP3] 	= &&I2C_WRITE_PROC_GROUP3,
		[I2C_WRITE_PROC_GROUP4] 	= &&I2C_WRITE_PROC_GROUP4,
		[I2C_WRITE_PROC_GROUP5] 	= &&I2C_WRITE_PROC_GROUP5,
		[I2C_WRITE_PROC_GROUP6] 	= &&I2C_WRITE_PROC_GROUP6,
		[I2C_WRITE_PROC_GROUP6_1] 	= &&I2C_WRITE_PROC_GROUP6_1,
		[I2C_WRITE_PROC_GROUP7] 	= &&I2C_WRITE_PROC_GROUP7,
		[I2C_WRITE_PROC_GROUP9]		= &&I2C_WRITE_PROC_GROUP9,
		[I2C_WRITE_PROC_GROUP13]	= &&I2C_WRITE_PROC_GROUP13,
		[I2C_WRITE_PROC_GROUP14]	= &&I2C_WRITE_PROC_GROUP14,
		[I2C_WRITE_PROC_GROUP15]	= &&I2C_WRITE_PROC_GROUP15,
		[I2C_WRITE_PROC_GROUP15_ERROR]	= &&I2C_WRITE_PROC_GROUP15_ERROR,
		[I2C_WRITE_PROC_GROUP16]	= &&I2C_WRITE_PROC_GROUP16,
		[I2C_WRITE_PROC_GROUP17]	= &&I2C_WRITE_PROC_GROUP17,
		[I2C_WRITE_PROC_GROUP18]	= &&I2C_WRITE_PROC_GROUP18,
		[I2C_WRITE_PROC_GROUP19]	= &&I2C_WRITE_PROC_GROUP19,
		[I2C_WRITE_PROC_GROUP20]	= &&I2C_WRITE_PROC_GROUP20,
		[I2C_WRITE_PROC_GROUP21]	= &&I2C_WRITE_PROC_GROUP21,
		[I2C_WRITE_PROC_GROUP22]	= &&I2C_WRITE_PROC_GROUP22,
		[I2C_WRITE_PROC_GROUP23]	= &&I2C_WRITE_PROC_GROUP23,
		[I2C_WRITE_PROC_GROUP24]	= &&I2C_WRITE_PROC_GROUP24,
		[I2C_WRITE_PROC_GROUP25]	= &&I2C_WRITE_PROC_GROUP25,
		[I2C_WRITE_PROC_GROUP26]	= &&I2C_WRITE_PROC_GROUP26,
		[I2C_WRITE_PROC_GROUP27]	= &&I2C_WRITE_PROC_GROUP27,
	};
	goto *function[Shell_Task.Register.R15_PC];
I2C_WRITE_PROC_GROUP0:
	if(argc >= 4){
		Shell_Task.sscanf_driver.str = argv[1];
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP2;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP5;
	}else{
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP22;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP3;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP4:
	Shell_Task.sscanf_driver.str = argv[2];
	Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP6;
	Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP5;
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP5:
	string_scanf(&Shell_Task.sscanf_driver, NULL);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP6:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP22;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP6_1;
		Shell_Task.Register.R6_Count = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP6_1:
	if( Shell_Task.Register.R6_Count < (argc-3)){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP7;
	}else{
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP7:
	Shell_Task.sscanf_driver.str = argv[Shell_Task.Register.R6_Count+3];
	Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP9;
	Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP5;
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP9:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP22;
	}else{
		Shell_Task.Reg_Value[Shell_Task.Register.R6_Count] = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP6_1;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cwrite: bus%d dev=0x%2x\r\n", Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP15;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP14;
		Shell_Task.Register.retry  = 1000;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP14:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP15:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP16;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP21;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP15_ERROR;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP15_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP16:
	I2Cx_Write(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, &Shell_Task.Reg_Value[0], Shell_Task.Register.R6_Count);
	Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP17;
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP17:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP18;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP18:
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP19;
	}else{
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP23;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP19:
	string_printf(&Shell_Task.fmt_driver, "\r\n## i2cwrite: set value success\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP20:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP21:
	Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP0;
	return CORE_DONE;
I2C_WRITE_PROC_GROUP22:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP23:
	if( Shell_Task.Register.R0_Result == DEV_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP24;
	}else if( Shell_Task.Register.R0_Result == REG_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP25;
	}else if( Shell_Task.Register.R0_Result == WR_VALUE_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP26;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP27;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP24:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: DEV_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP25:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: REG_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP26:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: WR_VALUE_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_WRITE_PROC_GROUP27:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_WRITE_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_WRITE_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}



int do_i2cread(int argc,char *argv[])
{
	enum
	{
		I2C_READ_PROC_GROUP0,
		I2C_READ_PROC_GROUP2,
		I2C_READ_PROC_GROUP3,
		I2C_READ_PROC_GROUP4,
		I2C_READ_PROC_GROUP5,
		I2C_READ_PROC_GROUP6,
		I2C_READ_PROC_GROUP10,
		I2C_READ_PROC_GROUP11,
		I2C_READ_PROC_GROUP12,
		I2C_READ_PROC_GROUP13,
		I2C_READ_PROC_GROUP14,
		I2C_READ_PROC_GROUP15,
		I2C_READ_PROC_GROUP15_ERROR,
		I2C_READ_PROC_GROUP16,
		
		I2C_READ_PROC_GROUP17,
		I2C_READ_PROC_GROUP18,
		I2C_READ_PROC_GROUP19,
		I2C_READ_PROC_GROUP20,
		
		I2C_READ_PROC_GROUP20_1,
		I2C_READ_PROC_GROUP20_2,
		I2C_READ_PROC_GROUP20_3,
		I2C_READ_PROC_GROUP21,
		I2C_READ_PROC_GROUP22,
		I2C_READ_PROC_GROUP23,
		I2C_READ_PROC_GROUP24,
		I2C_READ_PROC_GROUP25,
		I2C_READ_PROC_GROUP26,
		I2C_READ_PROC_GROUP27,
		I2C_READ_STATE_SUM,
	};
	static const void *function[I2C_READ_STATE_SUM] = {
		[I2C_READ_PROC_GROUP0] 	= &&I2C_READ_PROC_GROUP0,
		[I2C_READ_PROC_GROUP2] 	= &&I2C_READ_PROC_GROUP2,
		[I2C_READ_PROC_GROUP3] 	= &&I2C_READ_PROC_GROUP3,
		[I2C_READ_PROC_GROUP4] 	= &&I2C_READ_PROC_GROUP4,
		[I2C_READ_PROC_GROUP5] 	= &&I2C_READ_PROC_GROUP5,
		[I2C_READ_PROC_GROUP6] 	= &&I2C_READ_PROC_GROUP6,
		[I2C_READ_PROC_GROUP10]	= &&I2C_READ_PROC_GROUP10,
		[I2C_READ_PROC_GROUP12]	= &&I2C_READ_PROC_GROUP12,
		[I2C_READ_PROC_GROUP13]	= &&I2C_READ_PROC_GROUP13,
		[I2C_READ_PROC_GROUP14]	= &&I2C_READ_PROC_GROUP14,
		[I2C_READ_PROC_GROUP15]	= &&I2C_READ_PROC_GROUP15,
		[I2C_READ_PROC_GROUP15_ERROR]= &&I2C_READ_PROC_GROUP15_ERROR,
		[I2C_READ_PROC_GROUP16]	= &&I2C_READ_PROC_GROUP16,
		[I2C_READ_PROC_GROUP17]	= &&I2C_READ_PROC_GROUP17,
		[I2C_READ_PROC_GROUP18]	= &&I2C_READ_PROC_GROUP18,
		[I2C_READ_PROC_GROUP19]	= &&I2C_READ_PROC_GROUP19,
		[I2C_READ_PROC_GROUP20]	= &&I2C_READ_PROC_GROUP20,
		
		[I2C_READ_PROC_GROUP20_1]	= &&I2C_READ_PROC_GROUP20_1,
		[I2C_READ_PROC_GROUP20_2]	= &&I2C_READ_PROC_GROUP20_2,
		[I2C_READ_PROC_GROUP20_3]	= &&I2C_READ_PROC_GROUP20_3,
		
		[I2C_READ_PROC_GROUP21]	= &&I2C_READ_PROC_GROUP21,
		[I2C_READ_PROC_GROUP22]	= &&I2C_READ_PROC_GROUP22,
		[I2C_READ_PROC_GROUP23]	= &&I2C_READ_PROC_GROUP23,
		[I2C_READ_PROC_GROUP24]	= &&I2C_READ_PROC_GROUP24,
		[I2C_READ_PROC_GROUP25]	= &&I2C_READ_PROC_GROUP25,
		[I2C_READ_PROC_GROUP26]	= &&I2C_READ_PROC_GROUP26,
		[I2C_READ_PROC_GROUP27]	= &&I2C_READ_PROC_GROUP27,
	};
	goto *function[Shell_Task.Register.R15_PC];
I2C_READ_PROC_GROUP0:
	Shell_Task.Reg_Addr = 0;
	if(argc == 4){
		Shell_Task.sscanf_driver.str = argv[1];
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP2;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP5;
	}else{
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP22;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP3;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object < I2C_BUS_SUM){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP22;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP4:
	Shell_Task.sscanf_driver.str = argv[2];
	Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP6;
	Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP5;
	return CORE_RUNNING;
I2C_READ_PROC_GROUP5:
	string_scanf(&Shell_Task.sscanf_driver,NULL);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP6:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP22;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP10;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP10:
	Shell_Task.sscanf_driver.str = argv[3];
	Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP12;
	Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP5;
	return CORE_RUNNING;
I2C_READ_PROC_GROUP12:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP22;
	}else{
		Shell_Task.Value_Length = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.Value_Length > 256){
			Shell_Task.Value_Length = 256;
		}
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP13;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, "\r\n## I2cget: bus%d dev=0x%2x len=%d", Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, Shell_Task.Value_Length);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP15;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP14;
		Shell_Task.Register.retry  = 1000;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP14:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP15:
	if( I2Cx_Open(Shell_Task.Register.R5_Object) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP16;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP21;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP15_ERROR;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP15_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Open i2c bus%d timeout\r\n", Shell_Task.Register.R5_Object);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP16:
	I2Cx_Read(Shell_Task.Register.R5_Object, Shell_Task.Dev_Addr, &Shell_Task.Reg_Value[0], Shell_Task.Value_Length);
	Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP17;
	return CORE_RUNNING;
I2C_READ_PROC_GROUP17:
	if( I2Cx_Endp(Shell_Task.Register.R5_Object) == CORE_DONE){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP18;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP18:
	Shell_Task.Register.R0_Result = I2Cx_Result(Shell_Task.Register.R5_Object);
	I2Cx_Close(Shell_Task.Register.R5_Object);
	if( Shell_Task.Register.R0_Result == NO_ERROR ){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP19;
	}else{
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP23;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP19:
	string_printf(&Shell_Task.fmt_driver, " loading...\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R6_Count = 0;
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP20_1;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP20:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP20_1:
	if( Shell_Task.Register.R6_Count == Shell_Task.Value_Length){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP21;
	}else{
		if( Shell_Task.Register.R6_Count % 8 == 0){
			Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20_3;
		}else{
			Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20_2;
		}
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP20_2:
	string_printf(&Shell_Task.fmt_driver, "0x%2x ",Shell_Task.Reg_Value[Shell_Task.Register.R6_Count]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP20_1;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP20_3:
	string_printf(&Shell_Task.fmt_driver, "\r\n[0x%2x]-> ",Shell_Task.Reg_Addr+Shell_Task.Register.R6_Count);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP20_2;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP21:
	Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP0;
	return CORE_DONE;
I2C_READ_PROC_GROUP22:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP23:
	if( Shell_Task.Register.R0_Result == DEV_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP24;
	}else if( Shell_Task.Register.R0_Result == REG_ADDR_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP25;
	}else if( Shell_Task.Register.R0_Result == WR_VALUE_NO_ACK){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP26;
	}else if( Shell_Task.Register.R0_Result == BUS_ERROR){
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP27;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP24:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: DEV_ADDR_NO_ACK\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP25:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x REG_ADDR_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP26:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: reg=0x%2x WR_VALUE_NO_ACK\r\n", Shell_Task.Reg_Addr);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
I2C_READ_PROC_GROUP27:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Error: i2c bus error\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = I2C_READ_PROC_GROUP21;
		Shell_Task.Register.R15_PC = I2C_READ_PROC_GROUP20;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}



int do_reboot(int argc,char *argv[])
{	
	__set_FAULTMASK(1); //关闭所有中断
	NVIC_SystemReset(); //进行软件复位
	return CORE_DONE;
}

	


int do_adc(int argc,char *argv[])
{	
	enum
	{
		ADC_PROC_GROUP0,
		ADC_PROC_GROUP1,
		ADC_PROC_GROUP2,
		ADC_PROC_GROUP3,
		ADC_PROC_GROUP4,
		ADC_PROC_GROUP5,
		ADC_PROC_GROUP6,
		ADC_PROC_GROUP7,
		ADC_PROC_GROUP8,
		ADC_PROC_SUM,
	};
	static const void *function[ADC_PROC_SUM] = {
		[ADC_PROC_GROUP0] 	= &&ADC_PROC_GROUP0,
		[ADC_PROC_GROUP1] 	= &&ADC_PROC_GROUP1,
		[ADC_PROC_GROUP2] 	= &&ADC_PROC_GROUP2,
		[ADC_PROC_GROUP3] 	= &&ADC_PROC_GROUP3,
		[ADC_PROC_GROUP4] 	= &&ADC_PROC_GROUP4,
		[ADC_PROC_GROUP5] 	= &&ADC_PROC_GROUP5,
		[ADC_PROC_GROUP6] 	= &&ADC_PROC_GROUP6,
		[ADC_PROC_GROUP7] 	= &&ADC_PROC_GROUP7,
		[ADC_PROC_GROUP8] 	= &&ADC_PROC_GROUP8,
	};
	goto *function[Shell_Task.Register.R15_PC];
ADC_PROC_GROUP0:
	if(argc == 1){
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP1;
	}
	return CORE_RUNNING;
ADC_PROC_GROUP1:
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);      //启动转换,等待转换完成
	Shell_Task.Register.R15_PC = ADC_PROC_GROUP2;
	return CORE_RUNNING;
ADC_PROC_GROUP2:
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
		Shell_Task.Register.R0_Result = (uint32_t)ADC_GetConversionValue(ADC1);
		Shell_Task.Register.R0_Result = (Shell_Task.Register.R0_Result*3300)>>12;
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP3;
	}
	return CORE_RUNNING;
ADC_PROC_GROUP3:	
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n##Vref = 3300mV, PA0 = %4dmV,",Shell_Task.Register.R0_Result);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = ADC_PROC_GROUP4;
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
ADC_PROC_GROUP4:
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);      //启动转换,等待转换完成
	Shell_Task.Register.R15_PC = ADC_PROC_GROUP5;
	return CORE_RUNNING;
ADC_PROC_GROUP5:
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){
		Shell_Task.Register.R0_Result = (uint32_t)ADC_GetConversionValue(ADC1);
		Shell_Task.Register.R0_Result = (Shell_Task.Register.R0_Result*3300)>>12;
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP6;
	}
	return CORE_RUNNING;
ADC_PROC_GROUP6:	
	string_printf(&Shell_Task.fmt_driver, "PA1 = %4dmV.\r\n",Shell_Task.Register.R0_Result);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = ADC_PROC_GROUP8;
		Shell_Task.Register.R15_PC = ADC_PROC_GROUP7;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;	
ADC_PROC_GROUP7:	
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
ADC_PROC_GROUP8:	
	Shell_Task.Register.R15_PC = ADC_PROC_GROUP0;
	return CORE_DONE;
}





int do_spi(int argc,char *argv[])
{
	enum
	{
		SPI_STATE_ARGC_COMP,
		SPI_STATE_BUS_GET,
		SPI_STATE_BUS_GET_SUCCESS,
		SPI_STATE_ERROR_ECHO,
		SPI_STATE_BUS_NUM_COMP,
		
		SPI_STATE_DEVICE_GET,
		SPI_STATE_DEVICE_GET_SUCCESS,
		SPI_STATE_DEVICE_NUM_COMP,
		
		SPI_STATE_OPEN_DEVICE,
		SPI_STATE_OPEN_DEVICE_ERROR,
		SPI_STATE_TX_DATA_PARAM,
		SPI_STATE_TX_DATA_PARAM_START,
		SPI_STATE_TX_DATA_PARAM_SUCCESS,
		SPI_STATE_RX_DATA_PARAM_START,
		SPI_STATE_RX_DATA_PARAM_SUCCESS,
		
		SPI_STATE_TX_DATA_ECHO,
		SPI_STATE_TX_DATA_ECHO_HEAD,
		SPI_STATE_TX_DATA_ECHO_START,
		SPI_STATE_DATA_ECHO_QUEUE,
		
		SPI_STATE_WRITE_READ_DEVICE,
		SPI_STATE_WRITE_READ_DEVICE_WAIT,
		SPI_STATE_ECHO_READ_DATA,
		SPI_STATE_RX_DATA_ECHO_HEAD,
		SPI_STATE_ECHO_READ_DATA_START,
		SPI_STATE_CLOSE_SPI_ENTER,
		SPI_STATE_CLOSE_SPI,
		SPI_STATE_CLOSE_SPI_1,
		
		SPI_STATE_ERROR_ECHO_QUEUE,
		SPI_STATE_SUM,
	};

	static const void *function[SPI_STATE_SUM] = {
		[SPI_STATE_ARGC_COMP] 			= &&SPI_STATE_ARGC_COMP,
		[SPI_STATE_BUS_GET] 			= &&SPI_STATE_BUS_GET,
		[SPI_STATE_BUS_GET_SUCCESS]		= &&SPI_STATE_BUS_GET_SUCCESS,
		[SPI_STATE_ERROR_ECHO] 			= &&SPI_STATE_ERROR_ECHO,
		[SPI_STATE_BUS_NUM_COMP]		= &&SPI_STATE_BUS_NUM_COMP,
		
		[SPI_STATE_DEVICE_GET] 			= &&SPI_STATE_DEVICE_GET,
		[SPI_STATE_DEVICE_GET_SUCCESS]	= &&SPI_STATE_DEVICE_GET_SUCCESS,
		[SPI_STATE_DEVICE_NUM_COMP]		= &&SPI_STATE_DEVICE_NUM_COMP,
		
		[SPI_STATE_OPEN_DEVICE]			= &&SPI_STATE_OPEN_DEVICE,
		[SPI_STATE_OPEN_DEVICE_ERROR]	= &&SPI_STATE_OPEN_DEVICE_ERROR,
		[SPI_STATE_TX_DATA_PARAM]		= &&SPI_STATE_TX_DATA_PARAM,
		[SPI_STATE_TX_DATA_PARAM_START]	= &&SPI_STATE_TX_DATA_PARAM_START,
		[SPI_STATE_TX_DATA_PARAM_SUCCESS]= &&SPI_STATE_TX_DATA_PARAM_SUCCESS,
		[SPI_STATE_RX_DATA_PARAM_START]	= &&SPI_STATE_RX_DATA_PARAM_START,
		[SPI_STATE_RX_DATA_PARAM_SUCCESS]= &&SPI_STATE_RX_DATA_PARAM_SUCCESS,
		[SPI_STATE_TX_DATA_ECHO]		= &&SPI_STATE_TX_DATA_ECHO,
		[SPI_STATE_TX_DATA_ECHO_HEAD]	= &&SPI_STATE_TX_DATA_ECHO_HEAD,
		[SPI_STATE_TX_DATA_ECHO_START]	= &&SPI_STATE_TX_DATA_ECHO_START,
		[SPI_STATE_DATA_ECHO_QUEUE]		= &&SPI_STATE_DATA_ECHO_QUEUE,
		
		[SPI_STATE_WRITE_READ_DEVICE]	= &&SPI_STATE_WRITE_READ_DEVICE,
		[SPI_STATE_WRITE_READ_DEVICE_WAIT]= &&SPI_STATE_WRITE_READ_DEVICE_WAIT,
		[SPI_STATE_ECHO_READ_DATA]		= &&SPI_STATE_ECHO_READ_DATA,
		[SPI_STATE_RX_DATA_ECHO_HEAD]	= &&SPI_STATE_RX_DATA_ECHO_HEAD,
		[SPI_STATE_ECHO_READ_DATA_START]= &&SPI_STATE_ECHO_READ_DATA_START,
		
		[SPI_STATE_CLOSE_SPI_ENTER]		= &&SPI_STATE_CLOSE_SPI_ENTER,
		[SPI_STATE_CLOSE_SPI]			= &&SPI_STATE_CLOSE_SPI,
		[SPI_STATE_CLOSE_SPI_1]			= &&SPI_STATE_CLOSE_SPI_1,
		[SPI_STATE_ERROR_ECHO_QUEUE]	= &&SPI_STATE_ERROR_ECHO_QUEUE,
	};
	goto *function[Shell_Task.Register.R15_PC];
	
SPI_STATE_ARGC_COMP://第一步参数错误，报错退出
	if(argc >= 4){//spi bus cs 
		Shell_Task.Register.R15_PC = SPI_STATE_BUS_GET;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}
	return CORE_RUNNING;
SPI_STATE_BUS_GET://第二步bus错误，报错退出
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SPI_STATE_BUS_GET_SUCCESS;
	}
	return CORE_RUNNING;
SPI_STATE_BUS_GET_SUCCESS:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}else{
		Shell_Task.Register.Bus = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = SPI_STATE_BUS_NUM_COMP;
	}
	return CORE_RUNNING;
SPI_STATE_ERROR_ECHO:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO_QUEUE;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_BUS_NUM_COMP:
	if( Shell_Task.Register.Bus < SPI_BUS_SUM){
		Shell_Task.Register.R15_PC = SPI_STATE_DEVICE_GET;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}
	return CORE_RUNNING;
SPI_STATE_DEVICE_GET:
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SPI_STATE_DEVICE_GET_SUCCESS;
	}
	return CORE_RUNNING;
SPI_STATE_DEVICE_GET_SUCCESS:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}else{
		Shell_Task.Register.Device = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = SPI_STATE_DEVICE_NUM_COMP;
	}
	return CORE_RUNNING;
SPI_STATE_DEVICE_NUM_COMP:
	if( Shell_Task.Register.Device < 2){
		Shell_Task.Register.R15_PC = SPI_STATE_OPEN_DEVICE;
		Shell_Task.Register.retry  = 1000;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}
	return CORE_RUNNING;
SPI_STATE_OPEN_DEVICE:
	if( SPIx_Open( Shell_Task.Register.Bus,Shell_Task.Register.Device) == CORE_SUCCESS){
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_PARAM;
		Shell_Task.TxLen = argc - 4;
		if( Shell_Task.TxLen > 16){
			Shell_Task.TxLen = 16;
		}
		Shell_Task.Register.R6_Count = 0;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = SPI_STATE_CLOSE_SPI_1;
	}else if( Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_OPEN_DEVICE_ERROR;
	}
	return CORE_RUNNING;
SPI_STATE_OPEN_DEVICE_ERROR:
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n## Open spi bus%d timeout\r\n", Shell_Task.Register.Bus);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_CLOSE_SPI_1;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_PARAM:
	if( Shell_Task.Register.R6_Count < Shell_Task.TxLen){
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_PARAM_START;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_RX_DATA_PARAM_START;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_PARAM_START:
	string_scanf(&Shell_Task.sscanf_driver, argv[3+Shell_Task.Register.R6_Count]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_PARAM_SUCCESS;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_PARAM_SUCCESS:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}else{
		Shell_Task.TxBuf[Shell_Task.Register.R6_Count] = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_PARAM;
	}
	return CORE_RUNNING;
SPI_STATE_RX_DATA_PARAM_START:
	string_scanf(&Shell_Task.sscanf_driver, argv[argc-1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SPI_STATE_RX_DATA_PARAM_SUCCESS;
	}
	return CORE_RUNNING;
SPI_STATE_RX_DATA_PARAM_SUCCESS:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SPI_STATE_ERROR_ECHO;
	}else{
		Shell_Task.RxLen = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.RxLen > 16){
			Shell_Task.RxLen = 16;
		}
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_ECHO_HEAD;
		Shell_Task.Register.R6_Count = 0;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_ECHO:
	if( Shell_Task.Register.R6_Count < Shell_Task.TxLen){
		Shell_Task.Register.R15_PC = SPI_STATE_TX_DATA_ECHO_START;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_WRITE_READ_DEVICE;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_ECHO_HEAD:
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n## SPI Bus = %d, Dev = %d, Tx Length = %d, Rx Length = %d\r\n## Tx Data : ",Shell_Task.Register.Bus,Shell_Task.Register.Device, Shell_Task.TxLen, Shell_Task.RxLen);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_TX_DATA_ECHO;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_TX_DATA_ECHO_START:
	string_printf(&Shell_Task.fmt_driver, "0x%2x ",Shell_Task.TxBuf[Shell_Task.Register.R6_Count]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_TX_DATA_ECHO;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_DATA_ECHO_QUEUE:	
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
SPI_STATE_WRITE_READ_DEVICE:
	SPIx_WriteThenRead( Shell_Task.Register.Bus,Shell_Task.TxBuf,Shell_Task.TxLen, Shell_Task.RxBuf,Shell_Task.RxLen);
	Shell_Task.Register.R15_PC = SPI_STATE_WRITE_READ_DEVICE_WAIT;
	return CORE_RUNNING;
SPI_STATE_WRITE_READ_DEVICE_WAIT:
	if( SPIx_Endp(Shell_Task.Register.Bus) == CORE_DONE){
		Shell_Task.Register.R15_PC = SPI_STATE_RX_DATA_ECHO_HEAD;
		Shell_Task.Register.R6_Count = 0;
	}
	return CORE_RUNNING;
SPI_STATE_ECHO_READ_DATA:
	if( Shell_Task.Register.R6_Count < Shell_Task.RxLen){
		Shell_Task.Register.R15_PC = SPI_STATE_ECHO_READ_DATA_START;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_CLOSE_SPI_ENTER;
	}
	return CORE_RUNNING;
SPI_STATE_RX_DATA_ECHO_HEAD:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Rx Data : ");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_ECHO_READ_DATA;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_ECHO_READ_DATA_START:
	string_printf(&Shell_Task.fmt_driver, "0x%2x ",Shell_Task.RxBuf[Shell_Task.Register.R6_Count]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_ECHO_READ_DATA;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R6_Count++;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_CLOSE_SPI_ENTER:
	string_printf(&Shell_Task.fmt_driver, "\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R14_LR = SPI_STATE_CLOSE_SPI;
		Shell_Task.Register.R15_PC = SPI_STATE_DATA_ECHO_QUEUE;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SPI_STATE_CLOSE_SPI:
	SPIx_Close(Shell_Task.Register.Bus, Shell_Task.Register.Device);
	Shell_Task.Register.R15_PC = SPI_STATE_ARGC_COMP;
	return CORE_DONE;
SPI_STATE_CLOSE_SPI_1:
	Shell_Task.Register.R15_PC = SPI_STATE_ARGC_COMP;
	return CORE_DONE;
SPI_STATE_ERROR_ECHO_QUEUE:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = SPI_STATE_ARGC_COMP;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}



int do_ymodem(int argc,char *argv[])
{	
	enum
	{
		Ymodem_INIT = 0,
		PARSE_ARGV1,
		PARSE_ARGV2,
		WAIT_SPI_BUS,
		WAIT_I2C_BUS,
		YMODEM_PROC,
		YMODEM_PROC_SUM,
	};
	static const void *function[YMODEM_PROC_SUM] = {
		[Ymodem_INIT] 	= &&Ymodem_INIT,
		[PARSE_ARGV1] 	= &&PARSE_ARGV1,
		[PARSE_ARGV2] 	= &&PARSE_ARGV2,
		[WAIT_SPI_BUS] 	= &&WAIT_SPI_BUS,
		[WAIT_I2C_BUS] 	= &&WAIT_I2C_BUS,
		[YMODEM_PROC] 	= &&YMODEM_PROC,
	};
	goto *function[Shell_Task.Register.R15_PC];
	
Ymodem_INIT:	
	if( argc != 3){
		printf("\r\n## Argc Error\r\n");
		return CORE_DONE;
	}
PARSE_ARGV1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP){
		goto PARSE_ARGV1;
	}
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = Ymodem_INIT;
		printf("\r\n## Argv[1] Error\r\n");
		return CORE_DONE;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
	}
PARSE_ARGV2:	
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP){
		goto PARSE_ARGV2;
	}
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = Ymodem_INIT;
		printf("\r\n## Argv[2] Error\r\n");
		return CORE_DONE;
	}else{
		Shell_Task.Register.Address = Shell_Task.sscanf_driver.Register.R0_Result;
	}
	
	ymodem.Init_addr = Shell_Task.Register.Address;
	Shell_Task.Register.retry = 1000;
	if( Shell_Task.Register.R5_Object == 0){
		ymodem.Erase = FlashErase;
		ymodem.Write = FlashWrite;
		goto YMODEM_PROC;
	}else if(Shell_Task.Register.R5_Object == 1){
		ymodem.Erase = w25qxx_ymodem_erase;
		ymodem.Write = w25qxx_ymodem_write;
		goto WAIT_SPI_BUS;
	}else if(Shell_Task.Register.R5_Object == 2){
		ymodem.Erase = NULL;
		ymodem.Write = EEPROM_ymodem_write;
		goto WAIT_I2C_BUS;
	}else{
		printf("\r\n## Object Error\r\n");
		Shell_Task.Register.R15_PC = Ymodem_INIT;
		return CORE_DONE;
	}
WAIT_SPI_BUS:	
	if( SPIx_Open(W25qx.Bus,W25qx.BUS_ID) == CORE_SUCCESS){
		SPIx_Close(W25qx.Bus,W25qx.BUS_ID);
		goto YMODEM_PROC;
	}else if(Shell_Task.Register.retry > 0){
		Shell_Task.Register.retry--;
	}else{
		printf("\r\n## open bus timeout!!!\r\n");
		Shell_Task.Register.R15_PC = Ymodem_INIT;
		return CORE_DONE;
	}
	return CORE_RUNNING;
WAIT_I2C_BUS:
	if( I2Cx_Open(EEPROM_Driver.BUS) == CORE_SUCCESS){
		I2Cx_Close(EEPROM_Driver.BUS);
		goto YMODEM_PROC;
	}else{
		Shell_Task.Register.R15_PC = WAIT_I2C_BUS;
	}
	return CORE_RUNNING;
YMODEM_PROC:
	ymodem_Task(&Shell_Device.Shell_Driver.Shell_Queue);
	Shell_Task.Register.R15_PC = Ymodem_INIT;
	return CORE_DONE;
}

int do_GPIOget(int argc,char *argv[])
{
	enum
	{
		GPIOget_PROC_GROUP0 = 0,
		GPIOget_PROC_GROUP1,
		GPIOget_PROC_GROUP2,
		GPIOget_PROC_GROUP3,
		GPIOget_PROC_GROUP4,
		GPIOget_PROC_GROUP5,
		GPIOget_PROC_GROUP6,
		GPIOget_PROC_GROUP7,
		GPIOget_PROC_GROUP7_1,
		GPIOget_PROC_GROUP9,
		GPIOget_PROC_GROUP10,
		GPIOget_STATE_SUM,
	};
	static const void *function[GPIOget_STATE_SUM] = {
		[GPIOget_PROC_GROUP0] 	= &&GPIOget_PROC_GROUP0,
		[GPIOget_PROC_GROUP1] 	= &&GPIOget_PROC_GROUP1,
		[GPIOget_PROC_GROUP2]   = &&GPIOget_PROC_GROUP2,
		[GPIOget_PROC_GROUP3]   = &&GPIOget_PROC_GROUP3,
		[GPIOget_PROC_GROUP4] 	= &&GPIOget_PROC_GROUP4,
		[GPIOget_PROC_GROUP5] 	= &&GPIOget_PROC_GROUP5,
		[GPIOget_PROC_GROUP6] 	= &&GPIOget_PROC_GROUP6,
		[GPIOget_PROC_GROUP7] 	= &&GPIOget_PROC_GROUP7,
		[GPIOget_PROC_GROUP7_1] = &&GPIOget_PROC_GROUP7_1,
		[GPIOget_PROC_GROUP9] 	= &&GPIOget_PROC_GROUP9,
		[GPIOget_PROC_GROUP10] 	= &&GPIOget_PROC_GROUP10,
	};
	goto *function[Shell_Task.Register.R15_PC];
GPIOget_PROC_GROUP0:
	if( argc == 3){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP10;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP2;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP10;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP3;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object == 0x0A){
		Shell_Task.PORT = GPIOA;
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 0x0B){
		Shell_Task.PORT = GPIOB;
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 0x0C){
		Shell_Task.PORT = GPIOC;
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP10;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP4:
	string_scanf(&Shell_Task.sscanf_driver,argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP5;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP5:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP10;
	}else{
		Shell_Task.Register.R4_Status = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.Register.R4_Status > 15){
			Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP10;
		}else{
			Shell_Task.PIN = ((uint16_t)0x0001<<Shell_Task.Register.R4_Status);
			Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP6;
		}
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP6:	
	Shell_Task.Register.R0_Result = GPIO_GET_STATE(Shell_Task.PORT,Shell_Task.PIN);
	Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP7;
	return CORE_RUNNING;
GPIOget_PROC_GROUP7:
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n## get GPIO%s_%d state: %d\r\n", argv[1], Shell_Task.Register.R4_Status, Shell_Task.Register.R0_Result);
	Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP7_1;
	return CORE_RUNNING;
GPIOget_PROC_GROUP7_1:
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}else{
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP7;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP9:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
GPIOget_PROC_GROUP10:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOget_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}


int do_GPIOset(int argc,char *argv[])
{
	enum
	{
		GPIOSet_PROC_GROUP0 = 0,
		GPIOSet_PROC_GROUP1,
		GPIOSet_PROC_GROUP2,
		GPIOSet_PROC_GROUP3,
		GPIOSet_PROC_GROUP4,
		GPIOSet_PROC_GROUP5,
		
		GPIOSet_PROC_GROUP4_1,
		GPIOSet_PROC_GROUP5_1,
		
		
		GPIOSet_PROC_GROUP6,
		GPIOSet_PROC_GROUP7,
		GPIOSet_PROC_GROUP7_1,
		GPIOSet_PROC_GROUP9,
		GPIOSet_PROC_GROUP10,
		GPIOSet_STATE_SUM,
	};
	static const void *function[GPIOSet_STATE_SUM] = {
		[GPIOSet_PROC_GROUP0] 	= &&GPIOSet_PROC_GROUP0,
		[GPIOSet_PROC_GROUP1] 	= &&GPIOSet_PROC_GROUP1,
		[GPIOSet_PROC_GROUP2]   = &&GPIOSet_PROC_GROUP2,
		[GPIOSet_PROC_GROUP3]   = &&GPIOSet_PROC_GROUP3,
		[GPIOSet_PROC_GROUP4] 	= &&GPIOSet_PROC_GROUP4,
		[GPIOSet_PROC_GROUP5] 	= &&GPIOSet_PROC_GROUP5,
		
		[GPIOSet_PROC_GROUP4_1] 	= &&GPIOSet_PROC_GROUP4_1,
		[GPIOSet_PROC_GROUP5_1] 	= &&GPIOSet_PROC_GROUP5_1,
		
		[GPIOSet_PROC_GROUP6] 	= &&GPIOSet_PROC_GROUP6,
		[GPIOSet_PROC_GROUP7] 	= &&GPIOSet_PROC_GROUP7,
		[GPIOSet_PROC_GROUP7_1] = &&GPIOSet_PROC_GROUP7_1,
		[GPIOSet_PROC_GROUP9] 	= &&GPIOSet_PROC_GROUP9,
		[GPIOSet_PROC_GROUP10] 	= &&GPIOSet_PROC_GROUP10,
	};
	goto *function[Shell_Task.Register.R15_PC];
GPIOSet_PROC_GROUP0:
	if( argc == 4){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP2;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP3;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object == 0x0A){
		Shell_Task.PORT = GPIOA;
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 0x0B){
		Shell_Task.PORT = GPIOB;
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 0x0C){
		Shell_Task.PORT = GPIOC;
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP4:
	string_scanf(&Shell_Task.sscanf_driver,argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP5;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP5:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
	}else{
		Shell_Task.Register.R4_Status = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.Register.R4_Status > 15){
			Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
		}else{
			Shell_Task.PIN = ((uint16_t)0x0001<<Shell_Task.Register.R4_Status);
			Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP4_1;
		}
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP4_1:
	string_scanf(&Shell_Task.sscanf_driver,argv[3]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP5_1;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP5_1:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP10;
	}else{
		Shell_Task.Register.R8_Flag = Shell_Task.sscanf_driver.Register.R0_Result;
		if( Shell_Task.Register.R8_Flag == 0){
			GPIO_SET_LOW(Shell_Task.PORT,Shell_Task.PIN);
		}else{
			Shell_Task.Register.R8_Flag = 1;
			GPIO_SET_HIGH(Shell_Task.PORT,Shell_Task.PIN);
		}
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP6;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP6:
	Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP7;
	return CORE_RUNNING;
GPIOSet_PROC_GROUP7:
	string_printf(&Shell_Task.fmt_driver, "\r\n\r\n## set GPIO%s_%d state: %d\r\n", argv[1], Shell_Task.Register.R4_Status, Shell_Task.Register.R8_Flag);
	Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP7_1;
	return CORE_RUNNING;
GPIOSet_PROC_GROUP7_1:
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}else{
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP7;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP9:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
GPIOSet_PROC_GROUP10:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = GPIOSet_PROC_GROUP9;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
}


int do_w25q_read(int argc,char *argv[])
{
	enum
	{
		W25Q_PROC_GROUP0 = 0,
		W25Q_PROC_GROUP1,
		W25Q_PROC_GROUP2,
		W25Q_PROC_GROUP3,
		W25Q_PROC_GROUP4,
		W25Q_PROC_GROUP5,
		W25Q_PROC_GROUP6,
		W25Q_PROC_GROUP7,
		W25Q_STATE_SUM,
	};
	static const void *function[W25Q_STATE_SUM] = {
		[W25Q_PROC_GROUP0] 	= &&W25Q_PROC_GROUP0,
		[W25Q_PROC_GROUP1] 	= &&W25Q_PROC_GROUP1,
		[W25Q_PROC_GROUP2]  = &&W25Q_PROC_GROUP2,
		[W25Q_PROC_GROUP3]  = &&W25Q_PROC_GROUP3,
		[W25Q_PROC_GROUP4] 	= &&W25Q_PROC_GROUP4,
		[W25Q_PROC_GROUP5] 	= &&W25Q_PROC_GROUP5,
		[W25Q_PROC_GROUP6] 	= &&W25Q_PROC_GROUP6,
		[W25Q_PROC_GROUP7] 	= &&W25Q_PROC_GROUP7,
	};
	goto *function[Shell_Task.Register.R15_PC];
W25Q_PROC_GROUP0:
	if(argc == 2){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP5;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP2;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP2:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP5;
	}else{
		Shell_Task.Register.Address = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP3;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP3:
	if( W25QXX_Read(&W25qx, Shell_Task.RxBuf,Shell_Task.Register.Address,1) == CORE_DONE){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP4;
	}
W25Q_PROC_GROUP4:
	string_printf(&Shell_Task.fmt_driver, "\r\n## read addr = 0x%2x -> data = 0x%2x\r\n",Shell_Task.Register.Address, Shell_Task.RxBuf[0]);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP6;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP5:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP6;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP6:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = W25Q_PROC_GROUP7;
	}
	return CORE_RUNNING;
W25Q_PROC_GROUP7:
	Shell_Task.Register.R15_PC = W25Q_PROC_GROUP0;
	return CORE_DONE;
}	

int do_w25q_write(int argc,char *argv[])
{
	enum
	{
		W25Q_WRITE_PROC_GROUP0 = 0,
		W25Q_WRITE_PROC_GROUP1,
		W25Q_WRITE_PROC_GROUP2,
		W25Q_WRITE_PROC_GROUP3,
		W25Q_WRITE_PROC_GROUP3_1,
		W25Q_WRITE_PROC_GROUP4,
		W25Q_WRITE_PROC_GROUP4_1,
		W25Q_WRITE_PROC_GROUP5,
		W25Q_WRITE_PROC_GROUP6,
		W25Q_WRITE_PROC_GROUP7,
		W25Q_WRITE_STATE_SUM,
	};
	static const void *function[W25Q_WRITE_STATE_SUM] = {
		[W25Q_WRITE_PROC_GROUP0] 	= &&W25Q_WRITE_PROC_GROUP0,
		[W25Q_WRITE_PROC_GROUP1] 	= &&W25Q_WRITE_PROC_GROUP1,
		[W25Q_WRITE_PROC_GROUP2]  	= &&W25Q_WRITE_PROC_GROUP2,
		[W25Q_WRITE_PROC_GROUP3]  	= &&W25Q_WRITE_PROC_GROUP3,
		[W25Q_WRITE_PROC_GROUP3_1]  = &&W25Q_WRITE_PROC_GROUP3_1,
		[W25Q_WRITE_PROC_GROUP4] 	= &&W25Q_WRITE_PROC_GROUP4,
		[W25Q_WRITE_PROC_GROUP4_1] 	= &&W25Q_WRITE_PROC_GROUP4_1,
		[W25Q_WRITE_PROC_GROUP5] 	= &&W25Q_WRITE_PROC_GROUP5,
		[W25Q_WRITE_PROC_GROUP6] 	= &&W25Q_WRITE_PROC_GROUP6,
		[W25Q_WRITE_PROC_GROUP7] 	= &&W25Q_WRITE_PROC_GROUP7,
	};
	goto *function[Shell_Task.Register.R15_PC];
W25Q_WRITE_PROC_GROUP0:
	if(argc == 3){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP5;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver, argv[1]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP2;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP2:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP5;
	}else{
		Shell_Task.Register.Address = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP3;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP3:
	string_scanf(&Shell_Task.sscanf_driver, argv[2]);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
			Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP5;
		}else{
			Shell_Task.Register.R2_cin = Shell_Task.sscanf_driver.Register.R0_Result;
			Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP3_1;
		}
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP3_1:
	if( W25QXX_Write(&W25qx,&Shell_Task.Register.R2_cin,Shell_Task.Register.Address,1) == CORE_DONE){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP4;
	}else if( KeyboardInterrupt(&Shell_Device) == 1){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP0;
		return CORE_DONE;
	}else if( W25qx.ret == CORE_ERROR){
		W25qx.ret = CORE_SUCCESS;
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP4_1;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP4:
	string_printf(&Shell_Task.fmt_driver, "\r\n## write addr = 0x%2x -> data = 0x%2x success\r\n",Shell_Task.Register.Address, Shell_Task.Register.R2_cin);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP6;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP4_1:
	string_printf(&Shell_Task.fmt_driver, "\r\n## write addr = 0x%2x -> data = 0x%2x timeout\r\n",Shell_Task.Register.Address, Shell_Task.Register.R2_cin);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP6;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;	
W25Q_WRITE_PROC_GROUP5:
	string_printf(&Shell_Task.fmt_driver, SHELL_CMD_ERROR);
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP6;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP6:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP7;
	}
	return CORE_RUNNING;
W25Q_WRITE_PROC_GROUP7:
	Shell_Task.Register.R15_PC = W25Q_WRITE_PROC_GROUP0;
	return CORE_DONE;
}


int do_canstd(int argc,char *argv[])
{
	if (argc < 3 || argc > 11) {
        printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
    }
	do{
		string_scanf(&Shell_Task.sscanf_driver, argv[1]);//解析ID
	}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
	}
	
	do{
		string_scanf(&Shell_Task.sscanf_driver, argv[2]);//解析长度
	}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
	}else{
		Shell_Task.Value_Length = Shell_Task.sscanf_driver.Register.R0_Result;
	}
	
	for( Shell_Task.Register.R1_Index = 0;Shell_Task.Register.R1_Index < 8;Shell_Task.Register.R1_Index++)
	{
		if( Shell_Task.Register.R1_Index < Shell_Task.Value_Length && Shell_Task.Register.R1_Index <( argc - 3))
		{
			do{
				string_scanf(&Shell_Task.sscanf_driver, argv[3+Shell_Task.Register.R1_Index]);//解析长度
			}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
			
			if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
				printf("%s", SHELL_CMD_ERROR);
				return CORE_DONE;
			}else{
				Shell_Task.TxBuf[Shell_Task.Register.R1_Index] = Shell_Task.sscanf_driver.Register.R0_Result;
			}
		}else{
			Shell_Task.TxBuf[Shell_Task.Register.R1_Index] = 0;
		}
	}
	printf("\r\n\r\nCAN Send: ExdId=0x%x,DLC=%d,Data:",Shell_Task.Dev_Addr,Shell_Task.Value_Length);
	for(int i = 0; i<8;i++){
		printf("0x%x ",Shell_Task.TxBuf[i]);
	}
	printf("\r\n");
	CAN_Standard_Send_Msg(&CAN1_BUS, Shell_Task.Dev_Addr,Shell_Task.TxBuf,Shell_Task.Value_Length);
	return CORE_DONE;
}

int do_canexd(int argc,char *argv[])
{
	if (argc < 3 || argc > 11) {
        printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
    }
	do{
		string_scanf(&Shell_Task.sscanf_driver, argv[1]);//解析ID
	}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
	}else{
		Shell_Task.Dev_Addr = Shell_Task.sscanf_driver.Register.R0_Result;
	}
	
	do{
		string_scanf(&Shell_Task.sscanf_driver, argv[2]);//解析长度
	}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		printf("%s", SHELL_CMD_ERROR);
        return CORE_DONE;
	}else{
		Shell_Task.Value_Length = Shell_Task.sscanf_driver.Register.R0_Result;
	}
	
	for( Shell_Task.Register.R1_Index = 0;Shell_Task.Register.R1_Index < 8;Shell_Task.Register.R1_Index++)
	{
		if( Shell_Task.Register.R1_Index < Shell_Task.Value_Length && Shell_Task.Register.R1_Index <( argc - 3))
		{
			do{
				string_scanf(&Shell_Task.sscanf_driver, argv[3+Shell_Task.Register.R1_Index]);//解析长度
			}while(Shell_Task.sscanf_driver.Register.R15_PC != CALL_SSCANF_PROC_ENDP);
			
			if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
				printf("%s", SHELL_CMD_ERROR);
				return CORE_DONE;
			}else{
				Shell_Task.TxBuf[Shell_Task.Register.R1_Index] = Shell_Task.sscanf_driver.Register.R0_Result;
			}
		}else{
			Shell_Task.TxBuf[Shell_Task.Register.R1_Index] = 0;
		}
	}
	printf("\r\n\r\nCAN Send: ExdId=0x%x,DLC=%d,Data:",Shell_Task.Dev_Addr,Shell_Task.Value_Length);
	for(int i = 0; i<8;i++){
		printf("0x%x ",Shell_Task.TxBuf[i]);
	}
	printf("\r\n");
	CAN_Extended_Send_Msg(&CAN1_BUS, Shell_Task.Dev_Addr,Shell_Task.TxBuf,Shell_Task.Value_Length);
	return CORE_DONE;
}

int do_canrecv(int argc,char *argv[])
{
	if(argc != 1){
		printf(SHELL_CMD_ERROR);
		return CORE_DONE;
	}
	for(;;){
		if( KeyboardInterrupt(&Shell_Device) == 1){
			return CORE_DONE;
		}
		if( CAN1_BUS.RX_Flag == 1){
			CAN1_BUS.RX_Flag = 0;
			if(CAN1_BUS.RX_Message.IDE == CAN_ID_STD){
				printf("CAN Recv: StdId=0x%x,",CAN1_BUS.RX_Message.StdId);
			}else{
				printf("CAN Recv: ExtId=0x%x,",CAN1_BUS.RX_Message.ExtId);
			}
			printf("RTR=%d,",CAN1_BUS.RX_Message.RTR);
			printf("DLC=%d,Data:",CAN1_BUS.RX_Message.DLC);
			
			for(int i = 0; i<8;i++){
				printf("0x%x ",CAN1_BUS.RX_Message.Data[i]);
			}
			printf("\r\n");
		}
		return CORE_RUNNING;
	}
}



int do_servo(int argc,char *argv[])
{	
	enum
	{
		SERVO_PROC_GROUP0,
		SERVO_PROC_GROUP1,
		SERVO_PROC_GROUP2,
		SERVO_PROC_GROUP3,
		SERVO_PROC_GROUP4,
		SERVO_PROC_GROUP5,
		SERVO_PROC_GROUP6,
		SERVO_PROC_GROUP7,
		SERVO_PROC_GROUP8,
		SERVO_PROC_GROUP13,
		SERVO_PROC_GROUP14,
		SERVO_STATE_SUM,
	};
	static const void *function[SERVO_STATE_SUM] = {
		[SERVO_PROC_GROUP0] 	= &&SERVO_PROC_GROUP0,
		[SERVO_PROC_GROUP1] 	= &&SERVO_PROC_GROUP1,
		[SERVO_PROC_GROUP2] 	= &&SERVO_PROC_GROUP2,
		[SERVO_PROC_GROUP3] 	= &&SERVO_PROC_GROUP3,
		[SERVO_PROC_GROUP4] 	= &&SERVO_PROC_GROUP4,
		[SERVO_PROC_GROUP5] 	= &&SERVO_PROC_GROUP5,
		[SERVO_PROC_GROUP6] 	= &&SERVO_PROC_GROUP6,
		[SERVO_PROC_GROUP7] 	= &&SERVO_PROC_GROUP7,
		[SERVO_PROC_GROUP8] 	= &&SERVO_PROC_GROUP8,
		[SERVO_PROC_GROUP13] 	= &&SERVO_PROC_GROUP13,
		[SERVO_PROC_GROUP14] 	= &&SERVO_PROC_GROUP14,

	};
	
	goto *function[Shell_Task.Register.R15_PC];
	
SERVO_PROC_GROUP0:
	if( argc == 4 || argc == 3){
		Shell_Task.sscanf_driver.str = argv[1];
		Shell_Task.Register.R14_LR = SERVO_PROC_GROUP2;
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP1;
	}else{
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP13;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP1:
	string_scanf(&Shell_Task.sscanf_driver,NULL);
	if( Shell_Task.sscanf_driver.Register.R15_PC == CALL_SSCANF_PROC_ENDP){
		Shell_Task.Register.R15_PC = Shell_Task.Register.R14_LR;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP2:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP13;
	}else{
		Shell_Task.Register.R5_Object = Shell_Task.sscanf_driver.Register.R0_Result;
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP3;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP3:
	if( Shell_Task.Register.R5_Object == 0){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 1){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP4;
	}else if( Shell_Task.Register.R5_Object == 2){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP4;
	}else{
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP13;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP4:
	Shell_Task.sscanf_driver.str = argv[2];
	Shell_Task.Register.R14_LR = SERVO_PROC_GROUP5;
	Shell_Task.Register.R15_PC = SERVO_PROC_GROUP1;
	return CORE_RUNNING;
SERVO_PROC_GROUP5:
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP13;
	}else{
		if( Shell_Task.Register.R5_Object == 0){
			sim_servo_angle_set(&Servo_Output0, Shell_Task.sscanf_driver.Register.R0_Result);
		}else if( Shell_Task.Register.R5_Object == 1){
			pwm_servo_angle_set(&Servo_Output1, Shell_Task.sscanf_driver.Register.R0_Result);
		}else if( Shell_Task.Register.R5_Object == 2){
			pwm_servo_angle_set(&Servo_Output2, Shell_Task.sscanf_driver.Register.R0_Result);
		}
		if( argc == 4){
			Shell_Task.Register.R15_PC = SERVO_PROC_GROUP6;
		}else{
			if( Shell_Task.Register.R5_Object == 0){
				Servo_Output0.Timer.TickPeroid = 1000;
			}else if( Shell_Task.Register.R5_Object == 1){
				Servo_Output1.Timer.TickPeroid = 1000;
			}else if( Shell_Task.Register.R5_Object == 2){
				Servo_Output2.Timer.TickPeroid = 1000;
			}
			Shell_Task.Register.R15_PC = SERVO_PROC_GROUP8;
		}
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP6:
	Shell_Task.sscanf_driver.str = argv[3];
	Shell_Task.Register.R14_LR = SERVO_PROC_GROUP7;
	Shell_Task.Register.R15_PC = SERVO_PROC_GROUP1;
	return CORE_RUNNING;
SERVO_PROC_GROUP7:	
	if( Shell_Task.sscanf_driver.Register.R9_Error == CORE_ERROR){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP13;
	}else{
		if( Shell_Task.sscanf_driver.Register.R0_Result <= 0){
			Shell_Task.sscanf_driver.Register.R0_Result = 2000;
		}
		if( Shell_Task.Register.R5_Object == 0){
			Servo_Output0.Timer.TickPeroid = Shell_Task.sscanf_driver.Register.R0_Result;
		}else if( Shell_Task.Register.R5_Object == 1){
			Servo_Output1.Timer.TickPeroid = Shell_Task.sscanf_driver.Register.R0_Result;
		}else if( Shell_Task.Register.R5_Object == 2){
			Servo_Output2.Timer.TickPeroid = Shell_Task.sscanf_driver.Register.R0_Result;
		}
			
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP8;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP8:
	if( Shell_Task.Register.R5_Object == 0){
		string_printf(&Shell_Task.fmt_driver, "\r\n## Success servo %d duty %d, angle delay %d.\r\n", Shell_Task.Register.R5_Object,Servo_Output0.target_duty,Servo_Output0.Timer.TickPeroid);
	}else if( Shell_Task.Register.R5_Object == 1){
		string_printf(&Shell_Task.fmt_driver, "\r\n## Success servo %d duty %d, angle delay %d.\r\n", Shell_Task.Register.R5_Object,Servo_Output1.target_duty,Servo_Output1.Timer.TickPeroid);
	}else if( Shell_Task.Register.R5_Object == 2){
		string_printf(&Shell_Task.fmt_driver, "\r\n## Success servo %d duty %d, angle delay %d.\r\n", Shell_Task.Register.R5_Object,Servo_Output2.target_duty,Servo_Output2.Timer.TickPeroid);
	}
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP14;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP13:
	string_printf(&Shell_Task.fmt_driver, "\r\n## Unexpected exception\r\n");
	if( Shell_Task.fmt_driver.Register.R15_PC == CALL_SPRINTF_PROC_ENDP){
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP14;
		Shell_Task.Register.R1_Index = 0;
	}
	return CORE_RUNNING;
SERVO_PROC_GROUP14:
	if( Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index] != '\0'){
		QueueDataIn(&Shell_Device.Shell_Print_Queue, (uint8_t*)&Shell_Task.fmt_driver.str[Shell_Task.Register.R1_Index]);
		Shell_Task.Register.R1_Index++;
	}else{
		Shell_Task.Register.R15_PC = SERVO_PROC_GROUP0;
		return CORE_DONE;
	}
	return CORE_RUNNING;
}



void shell_Cmd_Init(Shell_Core_Class_t *Shell_Device, Usart_Bus_e Bus)
{
//方括号[]表示可选参数
//尖括号<>表示必选参数
	Shell_Device->Bus = Bus;
	if( Shell_Device->Bus == USART1_BUS){
		Shell_Device->USARTx= USART1;
		Shell_Device->Get  	= shell_Getchar_IT;
		Shell_Device->Put  	= shell_Putchar;
	}else if( Shell_Device->Bus == SIMUL1_BUS){
		Shell_Device->SIM_UART_Driver = &SIM_UART;
		Shell_Device->Get  	= shell_SIM_Uart_Getchar;
		Shell_Device->Put  	= shell_SIM_Uart_Putchar;
	}
	
	INIT_LIST_HEAD(&Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(clear,"\r\nclear - empty screen",do_clear);
	cmd_list_linked_list_tail(clear,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(help,"\r\nhelp - echo help info",do_help);
	cmd_list_linked_list_tail(help,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(led,"\r\nled - led dev mode",do_leds);
    cmd_list_linked_list_tail(led,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(top,"\r\ntop - cpu memory usage",do_top);
	cmd_list_linked_list_tail(top,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cdetect,"\r\ni2cdetect - +bus",do_i2cdetect);
	cmd_list_linked_list_tail(i2cdetect,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cdump,"\r\ni2cdump - +bus +addr",do_i2cdump);
	cmd_list_linked_list_tail(i2cdump,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cget,"\r\ni2cget - +bus +addr +reg +reg_length +read_length",do_i2cget);
	cmd_list_linked_list_tail(i2cget,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cset,"\r\ni2cset - +bus +addr +dev +reg_length +reg_value",do_i2cset);
	cmd_list_linked_list_tail(i2cset,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cwrite,"\r\ni2cwrite - +bus +addr +value *n",do_i2cwrite);
	cmd_list_linked_list_tail(i2cwrite,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(i2cread,"\r\ni2cread - +bus +addr +value *n",do_i2cread);
	cmd_list_linked_list_tail(i2cread,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(reboot,"\r\nreboot - soft reboot",do_reboot);
	cmd_list_linked_list_tail(reboot,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(adc,"\r\nadc - PB0 PB1 Volt",do_adc);
	cmd_list_linked_list_tail(adc,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(spi,"\r\nspi - +bus +cs tx buf*n rx_length",do_spi);
	cmd_list_linked_list_tail(spi,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(ymodem,"\r\nymodem 0 irom flash and 1 spi flash and 2 eeprom",do_ymodem);
	cmd_list_linked_list_tail(ymodem,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(gpioget,"\r\ngpioget - get gpio pin state e.g. gpioget A",do_GPIOget);
	cmd_list_linked_list_tail(gpioget,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(gpioset,"\r\ngpioset - set gpio pin state e.g. gpioset A 1",do_GPIOset);
	cmd_list_linked_list_tail(gpioset,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(w25qread,"\r\nw25qread - addr",do_w25q_read);
	cmd_list_linked_list_tail(w25qread,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(w25qwrite,"\r\nw25qwrite - addr data",do_w25q_write);
	cmd_list_linked_list_tail(w25qwrite,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(canstd,"\r\ncanstd - +id +len data*n",do_canstd);
	cmd_list_linked_list_tail(canstd,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(canexd,"\r\ncanexd - +id +len data*n",do_canexd);
	cmd_list_linked_list_tail(canexd,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(canrecv,"\r\ncanrecv - +canrecv data*n",do_canrecv);
	cmd_list_linked_list_tail(canrecv,Shell_Device->Shell_List_Header);
	
	cmd_list_create_node(servo,"\r\nservo - servo +id +angle +speed",do_servo);
	cmd_list_linked_list_tail(servo,Shell_Device->Shell_List_Header);
}
