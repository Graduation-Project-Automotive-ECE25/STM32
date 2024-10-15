/*
 * RCC.h
 *
 *  Created on: Aug 10, 2023
 *      Author: assem
 */

#ifndef RCC_H_
#define RCC_H_
/*======Includes======*/
#include "STM32F103C6.h"

/*========= CLKs Macros=======*/
#define HSI_RC_CLK 8000000
#define HSE_CLK    16000000//User should specify it
/*======APIs Supported by "MCAL RCC DRIVER====*/
extern uint32_t MCAL_RCC_GetSYSCLK();
extern uint32_t MCAL_RCC_GetHCLK();
extern uint32_t MCAL_RCC_GetPCLK1();
extern uint32_t MCAL_RCC_GetPCLK2();
extern uint32_t MCAL_RCC_TIMXCLK();

#endif /* RCC_H_ */
