/*
 * RCC.c
 *
 *  Created on: Aug 10, 2023
 *      Author: assem
 */

#include "RCC.h"

/* ============== Generic Variables =============== */
const uint8_t APBPrescaler[8] = {1,1,1,1,2,4,8,16};
const uint16_t AHBPrescaler[16] = {1,1,1,1,1,1,1,1,2,4,8,16,64,128,256,512};

uint32_t MCAL_RCC_GetSYSCLK(){
	switch((RCC->CFGR >> 2) & 0b11){
	case 0 :
		return HSI_RC_CLK;
		break;
	case 1:
		//User should specify it
		return HSE_CLK;
		break;
	case 2:
	default:
		//To do it,PLLCLK and PLLMUL and PLL SOurce MUX should be considered
		return 16000000;
		break;
	}
}

uint32_t MCAL_RCC_GetHCLK(){
	// Bits 7:4 HPRE: AHB pre-scaler
	return MCAL_RCC_GetSYSCLK() / AHBPrescaler[(RCC->CFGR & 0xf <<4)>>4];
}

uint32_t MCAL_RCC_GetPCLK1(){
	// Bits 10:8 PPRE1: APB low-speed pre-scaler (APB1)
	return MCAL_RCC_GetHCLK() / APBPrescaler[(RCC->CFGR & 0b111<<8)>>8];
}

uint32_t MCAL_RCC_GetPCLK2(){
	// Bits 13:11 PPRE2: APB high-speed pre-scaler (APB2)
	return MCAL_RCC_GetHCLK() / APBPrescaler[(RCC->CFGR & 0b111<<11)>>11];
}

uint32_t MCAL_RCC_TIMXCLK(){

	if(APBPrescaler[(RCC->CFGR & 0b111<<8)>>8] == 1)
		return MCAL_RCC_GetPCLK1();
	else
		return MCAL_RCC_GetPCLK1() * 2;
}
