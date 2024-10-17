/*
 * MY_RTOS_FIFO.h
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */

#ifndef MY_RTOS_FIFO_H_
#define MY_RTOS_FIFO_H_

#include "Scheduler.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//user configuration
#define Element_Type Task_ref*
#define size 100

//type definitions
typedef struct{
	Element_Type* base;
	Element_Type* head;
	Element_Type* tail;
	uint32_t length;
	uint32_t count;
}FIFO_t;

typedef enum{
	FIFO_NO_Error,
	FIFO_Full,
	FIFO_Null,
	FIFO_Empty
}FIFO_Status_t;

//APIs
FIFO_Status_t FIFO_init(FIFO_t*,Element_Type*,uint32_t);
FIFO_Status_t FIFO_enqueue(FIFO_t*,Element_Type);
FIFO_Status_t FIFO_dequeue(FIFO_t*,Element_Type*);

#endif /* MY_RTOS_FIFO_H_ */
