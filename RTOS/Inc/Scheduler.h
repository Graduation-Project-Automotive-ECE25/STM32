/*
 * Scheduler.h
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "CortexMx_OS_porting.h"
#include "string.h"

typedef enum {
	Activate_Task,Terminate_Task,Task_WaitingTime,Acquire_Mutex,Release_Mutex
}Service_Type;

typedef enum {
	NoError,Ready_queue_init_error,Task_exceeded_stacksize,Mutex_isNot_free
}Error_Type;

typedef struct{
	uint32_t stack_size;
	uint8_t priorrity;
	void (*p_TaskEntry)();
	uint32_t _S_PSP_Task;
	uint32_t _E_PSP_Task;
	uint32_t* Current_PSP;
	char TaskName[15];
	enum{
		suspended,waiting,ready,running
	}task_state;
	struct{
		uint32_t Ticks_Count;
		enum{
			enable,disable
		}Blocking;
	}Waiting_Time;
}Task_ref;

typedef struct{
	uint8_t* Ppayload;
	uint32_t PayloadSize;
	Task_ref* current_user;
	Task_ref* Next_user;
	char mutex_name[20];
}Mutex_ref;

/*============ APIs =============*/
extern Error_Type MY_RTOS_Init();
extern void MY_RTOS_Start();
extern Error_Type createTask(Task_ref*);
extern void activateTask(Task_ref*);
extern void terminateTask(Task_ref*);
extern void Task_Wait(uint32_t,Task_ref*);
extern Error_Type AcquireMutex(Mutex_ref*, Task_ref*);
extern void ReleaseMutex(Mutex_ref*);

#endif /* SCHEDULER_H_ */
