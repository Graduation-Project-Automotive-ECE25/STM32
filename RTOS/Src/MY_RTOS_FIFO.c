/*
 * MY_RTOS_FIFO.c
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */

#include "MY_RTOS_FIFO.h"

FIFO_Status_t FIFO_init(FIFO_t* FIFO_ptr,Element_Type* buff,uint32_t length){
	if(buff == NULL) //in case of dynamic allocation
		return FIFO_Null;

	//No Error
	FIFO_ptr->base = buff;
	FIFO_ptr->head = buff;
	FIFO_ptr->tail = buff;
	FIFO_ptr->length = length;
	FIFO_ptr->count = 0;

	return FIFO_NO_Error;
}

FIFO_Status_t FIFO_enqueue(FIFO_t* FIFO_ptr,Element_Type item){
	//check is FIFO Full?
	if(FIFO_ptr->count == FIFO_ptr->length)
		return FIFO_Full;

	//enqueue
	*(FIFO_ptr->head++) = item;
	FIFO_ptr->count++;

	//if head is at the end of buffer,it returns back to the base
	if(FIFO_ptr->head == FIFO_ptr->base + FIFO_ptr->length)
		FIFO_ptr->head = FIFO_ptr->base;

	return FIFO_NO_Error;
}
FIFO_Status_t FIFO_dequeue(FIFO_t* FIFO_ptr,Element_Type* item){
	//check is FIFO Empty?
	if(FIFO_ptr->count == 0)
		return FIFO_Empty;

	//dequeue
	*item = *(FIFO_ptr->tail++);
	FIFO_ptr->count--;

	//if tail is at the end of buffer,it returns back to the base
	if(FIFO_ptr->tail == FIFO_ptr->base + FIFO_ptr->length)
		FIFO_ptr->tail = FIFO_ptr->base;

	return FIFO_NO_Error;
}
