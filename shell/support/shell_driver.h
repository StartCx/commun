#ifndef __SHELL_DRIVER_H
#define __SHELL_DRIVER_H

//前缀"prefix"
//后缀"suffix"
#include "stack.h"
#include "list.h"
#include "queue.h"
#include "usart.h"
#include "static_lib.h"
#include "simulated_uart.h"
typedef enum
{
	USART1_BUS,
	SIMUL1_BUS,
}Usart_Bus_e;


enum
{
	FUNC_KEY_HEAD_TURE	= 0x01,
	FUNC_KEY_HEAD1_0x1B = 0x1B,
	FUNC_KEY_HEAD2_0x5B = 0x5B,
	FUNC_KEY_UP			= 0x41,
	FUNC_KEY_DOWN		= 0x42,
	FUNC_KEY_LEFT		= 0x44,
	FUNC_KEY_RIGHT		= 0x43,
};

enum
{
	SHELL_PASSWORD_AUTH_GROUP0 = 0,
	SHELL_PASSWORD_AUTH_GROUP1,
	SHELL_PASSWORD_AUTH_GROUP2,
	SHELL_PASSWORD_AUTH_GROUP3,
	SHELL_PASSWORD_AUTH_GROUP3_1,
	SHELL_PASSWORD_AUTH_GROUP3_2,
	SHELL_PASSWORD_AUTH_GROUP4,
	SHELL_PASSWORD_AUTH_GROUP5,
	SHELL_PASSWORD_AUTH_GROUP6,
	SHELL_PASSWORD_AUTH_GROUP7,
	SHELL_PASSWORD_AUTH_GROUP8,
	
	SHELL_GET_QUEUE_DATA,
	SHELL_ARROW_KEYS_DETECT_GROUP0,
	SHELL_ARROW_KEYS_DETECT_GROUP1,
	SHELL_ARROW_KEYS_DETECT_GROUP2,
	
	SHELL_HISTORY_UP_RECALL_GROUP0,
	SHELL_HISTORY_DOWN_RECALL_GROUP0,
	SHELL_HISTORY_RECALL_GROUP1,
	SHELL_HISTORY_RECALL_GROUP2,
	SHELL_HISTORY_RECALL_GROUP3,
	SHELL_HISTORY_RECALL_GROUP4,
	SHELL_HISTORY_RECALL_GROUP5,
	SHELL_HISTORY_RECALL_GROUP6,
	SHELL_HISTORY_RECALL_GROUP7,
	
	SHELL_CURSOR_PTR_LEFT_SHIFT,
	SHELL_CURSOR_PTR_RIGHT_SHIFT,
	
	SHELL_BACKSPACE_LEFT_SHIFT_GROUP0,
	SHELL_BACKSPACE_LEFT_SHIFT_GROUP1,
	SHELL_BACKSPACE_LEFT_SHIFT_GROUP2,
	SHELL_BACKSPACE_LEFT_SHIFT_GROUP3,
	
	SHELL_PUSH_STACK_DATA,
	SHELL_PUSH_PACKET_TAIL_0D_CMP,
	
	SHELL_HISTORY_BACKUP_GROUP0,
	SHELL_HISTORY_BACKUP_GROUP1,
	SHELL_HISTORY_BACKUP_GROUP2,
	SHELL_HISTORY_BACKUP_GROUP3,
	SHELL_HISTORY_BACKUP_GROUP4,
	
	SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP0,
	SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP1,
	SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP2,
	SHELL_PUSH_STACK_RIGHT_SHIFT_GROUP3,
	
	SHELL_STRTOK_PROC_GROUP0_INIT,
	SHELL_STRTOK_PROC_GROUP0_INIT2,
	SHELL_STRTOK_PROC_GROUP0_INIT3,
	SHELL_STRTOK_PROC_GROUP0,
	SHELL_STRTOK_PROC_GROUP1,
	SHELL_STRTOK_PROC_GROUP2,
	SHELL_LIST_EACH_ENTRY_GROUP0,
	SHELL_LIST_EACH_ENTRY_GROUP1,
	SHELL_LIST_EACH_ENTRY_GROUP2,
	SHELL_LIST_EACH_ENTRY_GROUP3,
	
	SHELL_STRCMP_PROC_ENDP_GROUP,
	SHELL_STRCPY_PROC_ENDP_GROUP,
	
	SHELL_EXEC_ECHO_SHELL,
	SHELL_EXEC_ECHO_BACKSPACE,
	SHELL_EXEC_ECHO_ERROR,
	
	SHELL_EXEC_ECHO_PRINTF,
	
	SHELL_CALL_SLOT_FUNC_GROUP0,
	SHELL_CALL_SLOT_FUNC_GROUP1,
	SHELL_CALL_SLOT_FUNC_GROUP2,
	SHELL_PROC_ENDP,
	
	SHELL_STATE_SUM,
};

#define SHELL_HISTORY_MAX 		(8)
#define ARGC_MAX 		  		(8)
#define COUNTDOWN_TIME           3
#define SHELL_PWD_AUTH           0
#define SHELL_USER_PASSWORD      "123"
#define SHELL_USER        		 "\r\nshell:~# "
#define SHELL_CMD_ERROR 		 "\r\n\r\n## Error: Inst Unexpected exception\r\n"

typedef struct
{
	char argc;
	char *argv[ARGC_MAX];
}CmdArgs_t;

typedef struct
{
	uint8_t 	Cursor_ptr;
	uint8_t 	Cursor_offset;
	int8_t 		hist_Trace_Back;
	int8_t 		hist_Up_Down_Mark;
	int8_t 		hist_Saved;
	int8_t 		hist_Cur_IDX;
	int8_t 		hist_Prev_IDX;
	int8_t 		hist_Next_IDX;
	int8_t 		hist_End_IDX;
	Cmd_List_t  *pList;
	CmdArgs_t   Shell_Args;
	CPU_RegisterClass_t  Register;
	Stack64_t 	Shell_Stack;
	Stack64_t 	hist_Stack[SHELL_HISTORY_MAX];
	Queue1048_t Shell_Queue;
}Shell_Driver_t;


typedef struct Shell_Core_Class_t
{
	//属性
	uint8_t 			Bus;				//定义串口编号
	USART_TypeDef 		*USARTx;			//寄存器串口地址
	struct list_head 	Shell_List_Header;	//命令链表头
	Queue512_t 			Shell_Print_Queue;	//串口打印缓存
	strtok_t   			Shell_Strtok;		//自定义strtok
	strcmp_t	  		Shell_Strcmp;		//自定义strcmp
	strcpy_t	  		Shell_Strcpy;		//自定义strcmp
	fmt_t 	  			Shell_fmt;			//自定义fmt
	Shell_Driver_t 		Shell_Driver;		//数据处理属性
	CPU_RegisterClass_t Shell_putchar;		//串口输出
	Timer_t				Timer;				//时间定时器
	SIM_UART_Driver_t 	*SIM_UART_Driver;	//模拟串口
	//方法
	void (*Init)(struct Shell_Core_Class_t *Shell, Usart_Bus_e Bus);
	void (*Get)(struct Shell_Core_Class_t *Shell);
	void (*Put)(struct Shell_Core_Class_t *Shell);
	void (*Driver)(struct Shell_Core_Class_t *Shell);
}Shell_Core_Class_t;

extern Shell_Core_Class_t Shell_Device;

void shell_Driver(Shell_Core_Class_t *Shell);
void shell_Putchar(Shell_Core_Class_t *Shell);
void shell_SIM_Uart_Putchar(Shell_Core_Class_t *Shell);
void shell_SIM_Uart_Getchar(Shell_Core_Class_t *Shell);
void shell_Getchar(Shell_Core_Class_t *Shell);
void shell_Getchar_IT(Shell_Core_Class_t *Shell);
extern char KeyboardInterrupt(Shell_Core_Class_t *Shell);

#endif
