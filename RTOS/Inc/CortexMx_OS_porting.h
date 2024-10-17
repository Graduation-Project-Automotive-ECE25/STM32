/*
 * CortexMx_OS_porting.h
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */

#ifndef CORTEXMX_OS_PORTING_H_
#define CORTEXMX_OS_PORTING_H_

#include "ARMCM3.h"


#define trigger_OS_PendSV() 		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk
#define OS_Stack_Size       		3072

#define Switch_to_Privileged()      __asm volatile("mrs r3,CONTROL \n\t lsr r3,r3,#0x1 \n\t lsl r3,r3,#0x1 \n\t msr CONTROL,r3")
#define Switch_to_UnPrivileged()    __asm volatile("mrs r3,CONTROL \n\t orr r3,r3,#0x1 \n\t msr CONTROL,r3")

#define Select_PSP()                __asm volatile("mrs r0,CONTROL \n\t orr r0,r0,#0x2 \n\t msr CONTROL,r0")
#define Set_PSP(add)                __asm volatile("msr PSP,%0" : : "r" (add))
#define Get_PSP(add)                __asm volatile("mrs %0,PSP"  : "=r" (add))

#define Select_MSP()                __asm volatile("mrs r0,CONTROL \n\t and r0,r0,#0x5 \n\t msr CONTROL,r0")

// External definition for the stack pointer
extern int _estack;
extern int _eheap;

/*============ APIs =============*/
extern void HW_Init();

#endif /* CORTEXMX_OS_PORTING_H_ */
