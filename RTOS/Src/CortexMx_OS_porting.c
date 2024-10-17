/*
 * CortexMx_OS_porting.c
 *
 *  Created on: Jul 4, 2024
 *      Author: assem
 */

#include "CortexMx_OS_porting.h"

extern void decide_whatNext();

  void HardFault_Handler(){while(1);}
  void MemManage_Handler(){while(1);}
  void BusFault_Handler(){while(1);}
  void UsageFault_Handler(){while(1);}

 //Exception handler for SVC (Supervisor Call) exception
  __attribute ((naked)) void SVC_Handler(void) {

  	__asm("TST LR,#4 \n\t"
  			"ITE EQ \n\t"
  			"MRSEQ r0,MSP \n\t"
  			"MRSNE r0,PSP \n\t"
  			"B OS_SVC");
  }

  void HW_Init(){

	  //lower PendSV interrupt priority to equal SysTick interrupt priority
	  __NVIC_SetPriority(PendSV_IRQn, 15);

  }

 uint8_t SystickLED;

 void SysTick_Handler(){

	 SystickLED ^=1;

	 Update_TasksWaitingTime();
	 decide_whatNext();
	 trigger_OS_PendSV();
  }
