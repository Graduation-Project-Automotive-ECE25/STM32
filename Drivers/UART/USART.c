/*
 * USART.c
 *
 *  Created on: Aug 10, 2023
 *      Author: assem
 */

/* =========================== Includes =========================== */
#include "USART.h"
#include "RCC.h"
#include "GPIO.h"
/*================================================================*/

/*=========================Generic Variables=====================*/
USART_Config_t Global_USART_Config[3] = {0};
/*================================================================*/



/* ================================================================
 * @Fn				- MCAL_USART_Init
 * @brief			- Initialize USART Asynchronous  only
 * @param [in] 		- USARTx: where x can be (1..3 depending on the device used)
 * @param [in] 		- USART_Config: All USART configuration
 * @retval 			- none
 * Note				- support for now Asynchronous mode & clock 8 MHz
 */
void MCAL_USART_Init(USARTx_REG* USARTx,USART_Config_t* USART_Config){

	uint32_t pclk, baud;

	/* 1. Enable the clock for the given USART peripheral */
	if(USARTx == USART1){

		USART1_CLK_EN();
		Global_USART_Config[0] = *USART_Config;

	}
	else if (USARTx == USART2){

		USART2_CLK_EN();
		Global_USART_Config[1] = *USART_Config;

	}
	else{

		USART3_CLK_EN();
		Global_USART_Config[2] = *USART_Config;

	}

	/* 2. Enable USART Module */
	USARTx->CR1 = 1<<13;

	/* 3. Enable USART TX/RX engines according to the USART_Mode configuration item */
	USARTx->CR1 |= USART_Config->USART_Mode;

	/* 4. Pay-load width */
	USARTx->CR1 |= USART_Config->Payload_Length;

	/* 5. configuration of parity control bit field */
	USARTx->CR1 |= USART_Config->Parity;

	/* 6. Configuration the no. of stop bits */
	USARTx->CR2 |= USART_Config->StopBits;

	/* 7. USART HW Flow Control */
	USARTx->CR3 |= USART_Config->HwFlowCtl;

	/* 8. Configuration of BRR (baud-rate register) */
	if(USARTx == USART1)
		pclk = MCAL_RCC_GetPCLK2();
	else
		pclk = MCAL_RCC_GetPCLK1();

	baud = USART_Config->BaudRate;
	USARTx->BRR = USART_BBR(pclk,baud);

	/* 9. ENABLE / DISABLE Interrupt */
	if(USART_Config->IRQ_Enable != USART_IRQ_Enable_NONE){

		USARTx->CR1 |= USART_Config->IRQ_Enable;

		/* Enable NVIC for USARTx IRQ */
		if(USARTx == USART1)
			NVIC_IRQ37_USART1_Enable();
		else if (USARTx == USART2)
			NVIC_IRQ38_USART2_Enable();
		else
			NVIC_IRQ39_USART3_Enable();
	}
}

/* ================================================================
 * @Fn				- MCAL_USART_DeInit
 * @brief			- DEInit USART Asynchronous only
 * @param [in] 		- USARTx where x can be (1..3 depending on the device used)
 * @param [in] 		- pTxBuffer buffer
 * @param [in] 		- Polling Enable or disable
 * @retval 			- none
 * Note				- Initialize USART first
 */
void MCAL_USART_DeInit(USARTx_REG* USARTx){

	if(USARTx == USART1){

		RCC->APB2RSTR |= 1<<14;
		NVIC_IRQ37_USART1_Disable();

	}else if(USARTx == USART2){

		RCC->APB1RSTR |= 1<<17;
		NVIC_IRQ38_USART2_Disable();

	}else{

		RCC->APB1RSTR |= 1<<18;
		NVIC_IRQ39_USART3_Disable();

	}
}

/* ================================================================
 * @Fn				- MCAL_USART_SendData
 * @brief			- Send buffer on USART
 * @param [in] 		- USARTx where x can be (1..3 depending on the device used)
 * @param [in] 		- pTxBuffer buffer
 * @param [in] 		- Polling Enable or disable
 * @retval 			- none
 * Note				- Initialize USART first
 * 					- when transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register)
 * 					- the value written in the MSB ( bit 7 or bit 8 depending on the data length ) has no effect
 * 					- because the parity replaces it
 * 					- when receiving with the parity enabled the value read in the MSB bit is the received parity bit
 */

void MCAL_USART_SendData(USARTx_REG* USARTx,uint16_t* pTxbuffer,Polling_Mechanism Polling){

	/* 1. Wait until TXE flag is set in the SR */
	if(Polling == enable)
		while(!(USARTx->SR & 1<<7));

	/* 2. Check the USART_WordLength item for 9-bit or 8-bit in a frame */
	if(USARTx == USART1){

		switch(Global_USART_Config[0].Payload_Length){

		case USART_Payload_Length_9B:
			USARTx->DR = *pTxbuffer & 0x1ff;
			break;

		case USART_Payload_Length_8B:
			USARTx->DR = *pTxbuffer & 0xff;
			break;

		}
	}

	else if(USARTx == USART2){

		switch(Global_USART_Config[1].Payload_Length){

		case USART_Payload_Length_9B:
			USARTx->DR = *pTxbuffer & 0x1ff;
			break;

		case USART_Payload_Length_8B:
			USARTx->DR = *pTxbuffer & 0xff;
			break;

		}
	}

	else if(USARTx == USART3){

		switch(Global_USART_Config[2].Payload_Length){

		case USART_Payload_Length_9B:
			USARTx->DR = *pTxbuffer & 0x1ff;
			break;

		case USART_Payload_Length_8B:
			USARTx->DR = *pTxbuffer & 0xff;
			break;

		}
	}
}

/* ================================================================
 * @Fn				- MCAL_USART_GPIO_Set_Pins
 * @brief			- Initializes GPIO Pins
 * @param[in]		- USARTx: where x can be (1..3 depending on the device used)
 * @retval 			- None
 * @Notes			- Should enable the corresponding ALT & GPIO in RCC clock . Also called after MCAL_USART_Init()
 */
void MCAL_UASRT_GPIO_Set_Pins(USARTx_REG* USARTx){
	GPIO_PinConfig_t pincfg;

	if(USARTx == USART1){

		GPIOx_CLK_EN('A');
		
		//TX PA9
		pincfg.GPIO_PinNumber = GPIO_PIN_9;
		pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
		MCAL_GPIOx_Init(GPIOA,&pincfg);

		//RX PA10
		pincfg.GPIO_PinNumber = GPIO_PIN_10;
		pincfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIOx_Init(GPIOA,&pincfg);

		//CTS PA11
		if(Global_USART_Config[0].HwFlowCtl == USART_HwFlowCtl_CTS || Global_USART_Config[0].HwFlowCtl == USART_HwFlowCtl_RTS_CTS){
			pincfg.GPIO_PinNumber = GPIO_PIN_11;
			pincfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
			MCAL_GPIOx_Init(GPIOA, &pincfg);
		}

		//RTS PA12
		if(Global_USART_Config[0].HwFlowCtl == USART_HwFlowCtl_RTS || Global_USART_Config[0].HwFlowCtl == USART_HwFlowCtl_RTS_CTS){
			pincfg.GPIO_PinNumber = GPIO_PIN_12;
			pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
			MCAL_GPIOx_Init(GPIOA,&pincfg);
		}
	}

	else if(USARTx == USART2){

		GPIOx_CLK_EN('A');
		
		//TX PA2
		pincfg.GPIO_PinNumber = GPIO_PIN_2;
		pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
		MCAL_GPIOx_Init(GPIOA,&pincfg);

		//RX PA3
		pincfg.GPIO_PinNumber = GPIO_PIN_3;
		pincfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIOx_Init(GPIOA,&pincfg);

		//CTS PA0
		if(Global_USART_Config[1].HwFlowCtl == USART_HwFlowCtl_CTS || Global_USART_Config[1].HwFlowCtl == USART_HwFlowCtl_RTS_CTS){
			pincfg.GPIO_PinNumber = GPIO_PIN_0;
			pincfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
			MCAL_GPIOx_Init(GPIOA, &pincfg);
		}

		//RTS PA1
		if(Global_USART_Config[1].HwFlowCtl == USART_HwFlowCtl_RTS || Global_USART_Config[1].HwFlowCtl == USART_HwFlowCtl_RTS_CTS){
			pincfg.GPIO_PinNumber = GPIO_PIN_1;
			pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
			MCAL_GPIOx_Init(GPIOA,&pincfg);
		}
	}

	else if(USARTx == USART3){

		GPIOx_CLK_EN('B');
		
		//TX PB10
		pincfg.GPIO_PinNumber = GPIO_PIN_10;
		pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;
		MCAL_GPIOx_Init(GPIOB,&pincfg);

		//RX PB11
		pincfg.GPIO_PinNumber = GPIO_PIN_11;
		pincfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
		MCAL_GPIOx_Init(GPIOB,&pincfg);

	}
}
/* ================================================================
 * @Fn				- MCAL_USART_ReceiveData
 * @brief			- Receive buffer on USART
 * @param [in] 		- USARTx where x can be (1..3 depending on the device used)
 * @param [in] 		- pRxBuffer buffer
 * @param [in] 		- Polling Enable or disable
 * @retval 			- none
 * Note				- Initialize USART first
 * 					- when receiving with the parity enabled (PCE bit set to 1 in the USART_CR1 register)
 * 					- the value written in the MSB ( bit 7 or bit 8 depending on the data length ) has no effect
 * 					- because the parity replaces it
 * 					- when receiving with the parity enabled the value read in the MSB bit is the received parity bit
 */
void MCAL_USART_ReceiveData(USARTx_REG* USARTx,uint16_t* pRxbuffer,Polling_Mechanism Polling){

	/* 1. Wait until RXNE flag is set in the SR */
	if(Polling == enable)
		while(!(USARTx->SR & 1<<5));

	/* 2. Check the USART_WordLength item for 9-bit or 8-bit in a frame and take into consideration the parity*/
	if(USARTx == USART1){
		//Check Payload
		switch(Global_USART_Config[0].Payload_Length){
		// 9 bits data
		case USART_Payload_Length_9B:
			//Check the parity
			if(Global_USART_Config[0].Parity == USART_Parity_NONE)
				/* All 9 bits are data */
				*pRxbuffer =  USARTx->DR;
			else
				/* Just the least byte is data */
				*pRxbuffer =  USARTx->DR & 0xff;
			break;

			// 8 bits data
		case USART_Payload_Length_8B:
			//Check the parity
			if(Global_USART_Config[0].Parity == USART_Parity_NONE)
				/* All 8 bits are data */
				*pRxbuffer =  USARTx->DR & 0xff;
			else
				/* just least 7 bits are data */
				*pRxbuffer =  USARTx->DR & 0x7f;
			break;
		}
	}

	else if(USARTx == USART2){
		//Check Payload
		switch(Global_USART_Config[1].Payload_Length){
		// 9 bits data
		case USART_Payload_Length_9B:
			//Check the parity
			if(Global_USART_Config[1].Parity == USART_Parity_NONE)
				/* All 9 bits are data */
				*pRxbuffer =  USARTx->DR;
			else
				/* Just the least byte is data */
				*pRxbuffer =  USARTx->DR & 0xff;
			break;

		case USART_Payload_Length_8B:
			//Check the parity
			if(Global_USART_Config[1].Parity == USART_Parity_NONE)
				*pRxbuffer =  USARTx->DR & 0xff;
			else
				*pRxbuffer =  USARTx->DR & 0x7f;
			break;
		}
	}

	else if(USARTx == USART3){
		//Check Payload
		switch(Global_USART_Config[2].Payload_Length){
		// 9 bits data
		case USART_Payload_Length_9B:
			//Check the parity
			if(Global_USART_Config[2].Parity == USART_Parity_NONE)
				/* All 9 bits are data */
				*pRxbuffer =  USARTx->DR;
			else
				/* Just the least byte is data */
				*pRxbuffer =  USARTx->DR & 0xff;
			break;

		case USART_Payload_Length_8B:
			//Check the parity
			if(Global_USART_Config[2].Parity == USART_Parity_NONE)
				*pRxbuffer =  USARTx->DR & 0xff;
			else
				*pRxbuffer =  USARTx->DR & 0x7f;
			break;
		}
	}

}

void MCAL_USART_WAIT_TC(USARTx_REG* USARTx){
	/* Wait till TC flag is set in the SR */
	while(!(USARTx->SR & 1<<6));
}

/*==============ISR=============*/
void USART1_IRQHandler (void)
{
	Global_USART_Config[0].P_IRQ_CallBack();
}

void USART2_IRQHandler (void)
{
	Global_USART_Config[1].P_IRQ_CallBack();
}

void USART3_IRQHandler (void)
{
	Global_USART_Config[2].P_IRQ_CallBack();
}
