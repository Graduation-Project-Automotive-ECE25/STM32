/*
 * Scheduler.c
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */
#include "Scheduler.h"
#include "MY_RTOS_FIFO.h"

struct{
	Task_ref* OS_Tasks[100]; /* scheduler table */
	uint32_t _S_MSP;
	uint32_t _E_MSP;
	uint32_t PSP_Task_locator;
	uint8_t num_of_tasks;
	Task_ref* CurrentTask;
	Task_ref* NextTask;
	enum{
		os_suspended,os_running
	}state;
}OS_Control;

FIFO_t Ready_queue;
Task_ref* ready_queue_buf[100];
Task_ref IDLE_Task;
uint8_t IdleTaskLED;

void MY_RTOS_Idle_Task(){

	while(1){
		IdleTaskLED ^= 1;
		__asm("wfe");
	}
}

void BubbleSort(){

	Task_ref* temp;
	uint32_t n = OS_Control.num_of_tasks;

	for(uint32_t i = 0; i< n-1; i++){
		for(uint32_t j = 0; j< n-i-1; j++){
			if(OS_Control.OS_Tasks[j]->priority > OS_Control.OS_Tasks[j+1]->priority){
				temp = OS_Control.OS_Tasks[j];
				OS_Control.OS_Tasks[j] = OS_Control.OS_Tasks[j+1];
				OS_Control.OS_Tasks[j+1] = temp;
			}
		}
	}
}

void update_schedulerTable_and_readyQueue(){

	//bubble sort scheduler table from highest priority to the lowest
	BubbleSort();

	//free ready queue
	Task_ref* temp = NULL ;
	while(FIFO_dequeue(&Ready_queue, &temp) != FIFO_Empty);

	//update ready queue
	uint32_t i = 0;
	Task_ref *pTask,*pNextTask;

	while(i < OS_Control.num_of_tasks){

		pTask = OS_Control.OS_Tasks[i];
		pNextTask = OS_Control.OS_Tasks[i+1];

		if(pTask->task_state != suspended){
			//in case we reached to the end of available os tasks
			if(pNextTask->task_state == suspended){

				FIFO_enqueue(&Ready_queue, pTask);
				pTask->task_state = ready;
				break;

			}
			// if the current task has higher priority than the next task
			else if(pTask->priority < pNextTask->priority){

				FIFO_enqueue(&Ready_queue, pTask);
				pTask->task_state = ready;
				break;
				// if the current task has same priority as the next task
			}else if(pTask->priority == pNextTask->priority){

				FIFO_enqueue(&Ready_queue, pTask);
				pTask->task_state = ready;

			}
		}
		i++;
	}
}

void decide_whatNext(){

	//if ready queue is empty && current task != suspended
	if(Ready_queue.count == 0 && OS_Control.CurrentTask->task_state != suspended){

		OS_Control.CurrentTask->task_state = running;
		//add the current task again (round robin)
		FIFO_enqueue(&Ready_queue, OS_Control.CurrentTask);
		OS_Control.NextTask = OS_Control.CurrentTask;

	}else{

		FIFO_dequeue(&Ready_queue,&OS_Control.NextTask);
		OS_Control.NextTask->task_state = running;

		//update ready queue (to maintain round robin happening)
		if(OS_Control.CurrentTask->priority == OS_Control.NextTask->priority && OS_Control.CurrentTask->task_state != suspended){
			FIFO_enqueue(&Ready_queue,OS_Control.CurrentTask);
			OS_Control.CurrentTask->task_state = ready;

		}
	}
}

//To execute specific OS service
void OS_SVC(uint32_t* Stack_Frame){

	uint8_t  SVC_ID = ((char*) Stack_Frame[6])[-2];

	switch(SVC_ID){

	//Activate Task
	case 0:
		//Terminate Task
	case 1:

		update_schedulerTable_and_readyQueue();

		if(OS_Control.state == os_running){
			if(strcmp(OS_Control.CurrentTask->TaskName,"Idle Task")){
				/*not idle task*/

				decide_whatNext();

				trigger_OS_PendSV();
			}
		}

		break;

	case 2 :
		update_schedulerTable_and_readyQueue();
		break;

	case 3 :

		break;

	case 4 :

		break;
	}
}

void OS_SVC_Set(Service_Type  SVC_ID){

	switch(SVC_ID){

	case Activate_Task:

		__asm("SVC #0 \n\t");

		break;

	case Terminate_Task:

		__asm("SVC #1 \n\t");

		break;

	case Task_WaitingTime:

		__asm("SVC #2 \n\t");

		break;

	case Acquire_Mutex:

		__asm("SVC #3 \n\t");

		break;

	case Release_Mutex:

		__asm("SVC #4 \n\t");

		break;
	}
}

__attribute ((naked)) void PendSV_Handler(){

	//=======================================
	//Save the context of the current task
	//=======================================

	//get the PSP register value after pushing stack frame of the current task
	Get_PSP(OS_Control.CurrentTask->Current_PSP);

	//using the Current_PSP pointer to store r4 ... r11
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r4" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r5" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r6" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r7" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r8" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r9" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r10" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile("mov %0 , r11" : "=r" (*(OS_Control.CurrentTask->Current_PSP)) );

	//=======================================
	//Restore the context of the next task
	//=======================================
	if(OS_Control.NextTask != NULL){
	OS_Control.CurrentTask = OS_Control.NextTask;
	OS_Control.NextTask = NULL;
	}

	__asm volatile("mov r11,%0" ::"r" (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r10,%0" ::"r" (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r9,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r8,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r7,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r6,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r5,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	__asm volatile("mov r4,%0" ::"r"  (*(OS_Control.CurrentTask->Current_PSP++)));

	//update psp register value to the end address of new task's stack frame
	Set_PSP(OS_Control.CurrentTask->Current_PSP);

	//exit
	__asm volatile("BX LR");
}

void create_TaskStack(Task_ref* ptask){

	ptask->Current_PSP = (uint32_t*)ptask->_S_PSP_Task;

	ptask->Current_PSP--;
	*(ptask->Current_PSP) = 0x01000000;  //dummy xPSR - T = 1 - to avoid bus fault

	ptask->Current_PSP--;
	*(ptask->Current_PSP) = (uint32_t)ptask->p_TaskEntry; //dummy PC

	ptask->Current_PSP--;
	*(ptask->Current_PSP) = 0xfffffffd; //dummy LR

	for(int i = 0; i<13; i++){

		ptask->Current_PSP--;
		*(ptask->Current_PSP) = 0;

	}

}

void create_MainStack(){

	OS_Control._S_MSP = (uint32_t)&_estack;
	OS_Control._E_MSP = OS_Control._S_MSP - OS_Stack_Size;
	OS_Control.PSP_Task_locator = OS_Control._E_MSP - 8;
}

Error_Type MY_RTOS_createTask(Task_ref* ptask){

	Error_Type error = NoError;

	//create task stack
	ptask->_S_PSP_Task = OS_Control.PSP_Task_locator;
	ptask->_E_PSP_Task = ptask->_S_PSP_Task - ptask->stack_size;

	//check if task stack size exceeded the stack limit
	if(ptask->_E_PSP_Task < (uint32_t)&_eheap)
		error += Task_exceeded_stacksize;

	//Align 8 bytes space between each task
	OS_Control.PSP_Task_locator =  ptask->_E_PSP_Task - 8;

	//Initialize PSP Task stack
	create_TaskStack(ptask);

	//put the task in the scheduler table
	OS_Control.OS_Tasks[OS_Control.num_of_tasks++] = ptask;

	ptask->task_state = suspended;

	return error;
}

Error_Type MY_RTOS_Init(){

	Error_Type error = NoError;
	OS_Control.state = os_suspended;

	create_MainStack();

	/* create os ready queue */
	if(FIFO_init(&Ready_queue, ready_queue_buf, 100) != FIFO_NO_Error)
		error += Ready_queue_init_error;

	/* configure idle task */
	strcpy(IDLE_Task.TaskName, "Idle Task");
	IDLE_Task.priority = 255;
	IDLE_Task.p_TaskEntry = MY_RTOS_Idle_Task;
	IDLE_Task.stack_size = 300;

	error += MY_RTOS_createTask(&IDLE_Task);

	return error;
}

void MY_RTOS_Start(){

	OS_Control.state = os_running;

	//Set Default task = idle task
	OS_Control.CurrentTask = &IDLE_Task;
	MY_RTOS_activateTask(&IDLE_Task);
	//start ticker -1ms-
	SysTick_Config(8000);
	//Set PSP to idle task current PSP
	Set_PSP(OS_Control.CurrentTask->Current_PSP);
	//Switch thread mode's stack pointer to PSP instead of MSP
	Select_PSP();
	//Switch thread mode to unprivileged access level
	Switch_to_UnPrivileged();
	//start idle task
	OS_Control.CurrentTask->p_TaskEntry();
}

void MY_RTOS_activateTask(Task_ref* ptask){

	ptask->task_state = waiting;

	OS_SVC_Set(Activate_Task);
}

void MY_RTOS_terminateTask(Task_ref* ptask){

	ptask->task_state = suspended;

	OS_SVC_Set(Terminate_Task);
}

void MY_RTOS_Task_Wait(uint32_t ticks,Task_ref* ptask){

	ptask->Waiting_Time.Blocking = enable;
	ptask->Waiting_Time.Ticks_Count = ticks;
	ptask->task_state = suspended;
	OS_SVC_Set(Terminate_Task);
}

void Update_TasksWaitingTime(){

	for(int i=0; i<OS_Control.num_of_tasks; i++){
		if(OS_Control.OS_Tasks[i]->Waiting_Time.Blocking == enable && OS_Control.OS_Tasks[i]->task_state == suspended ){

			OS_Control.OS_Tasks[i]->Waiting_Time.Ticks_Count--;

			if(OS_Control.OS_Tasks[i]->Waiting_Time.Ticks_Count == 0){

				OS_Control.OS_Tasks[i]->Waiting_Time.Blocking = disable;
				OS_Control.OS_Tasks[i]->task_state = waiting;
				OS_SVC_Set(Task_WaitingTime);

			}
		}
	}
}

Error_Type MY_RTOS_AcquireMutex(Mutex_ref* pMutex, Task_ref* pTask){

	if(pMutex->current_user == NULL){ //Mutex is not taken by any task
		pMutex->current_user = pTask;
	}else if(pMutex->Next_user == NULL){ //No pending request by another task
		pMutex->Next_user = pTask;
		//Make task state suspended until the mutex is released
		pTask->task_state = suspended;
		OS_SVC_Set(Terminate_Task);

	}else{
		return Mutex_isNot_free;
	}

	return NoError;
}

void MY_RTOS_ReleaseMutex(Mutex_ref* pMutex){

	if(pMutex->current_user != NULL){
	pMutex->current_user = pMutex->Next_user;
	pMutex->Next_user = NULL;
	pMutex->current_user->task_state = waiting;
	OS_SVC_Set(Activate_Task);
	}
}
