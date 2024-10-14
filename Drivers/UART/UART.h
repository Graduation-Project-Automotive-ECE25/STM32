/*
 * USART.h
 *
 *  Created on: Aug 10, 2023
 *      Author: assem
 */

#ifndef USART_H_
#define USART_H_

/* =========================== Includes =========================== */
#include "GPIO.h"
#include "RCC.h"

/* =====================Configuration Structure==================== */
typedef struct{
	uint8_t  USART_Mode;		//Specifies Tx/Rx Enable/Disable
							    //This parameter must be set based on @ref USART_Mode_define

	uint32_t BaudRate;			// This member to configure the Baud Rate
								// This parameters must be set based on @ref USART_BaudRate_define

	uint16_t Payload_Length;	//Specifies the number of data bits transmitted or received in a frame
								//This parameters must be set based on @ref USART_Payload_Length_define

	uint16_t Parity;			//Specifies the parity mode
								//This parameters must be set based on @ref USART_Parity_define

	uint16_t StopBits;			//Specifies the number of stop bits
								//This parameters must be set based on @ref USART_StopBits_define

	uint16_t HwFlowCtl;			// Specifies whether the hardware flow control mode is enabled or disabled
								// This parameters must be set based on @ref USART_HwFlowCtl_define

	uint16_t IRQ_Enable;		//Enable or disable USART IRQ
								//This parameters must be set based on @ref USART_IRQ_Enable_define

	void(*P_IRQ_CallBack)();	//Set the C function which will be called once the IRQ happens

}USART_Config_t;

/*---------------------------------------------------------------------------*/
/* =====================Configuration References Macros ==================== */
/*-------------------------------------------------------------------------*/
//@ref USART_Mode_define
#define USART_Mode_Rx 		1<<2
#define USART_Mode_Tx		1<<3
#define USART_Mode_Tx_Rx	0b11<<2

//@ref USART_BaudRate_define
#define USART_BaudRate_2400			2400
#define USART_BaudRate_9600			9600
#define USART_BaudRate_19200		19200
#define USART_BaudRate_57600		57600
#define USART_BaudRate_115200		115200
#define USART_BaudRate_230400		230400
#define USART_BaudRate_460800		460800
#define USART_BaudRate_921600		921600
#define USART_BaudRate_225000		2250000
#define USART_BaudRate_4500000		4500000

//@ref USART_Payload_Length_define
#define USART_Payload_Length_8B		0
#define USART_Payload_Length_9B		1<<12

//@ref USART_Parity_define
#define USART_Parity_NONE		0
#define USART_Parity_EVEN		1<<10
#define USART_Parity_ODD		(1<<9 | 1<<10)

//@ref USART_StopBits_define
#define USART_StopBits_half		1<<12
#define USART_StopBits_1		0
#define USART_StopBits_1_half	3<<12
#define USART_StopBits_2		2<<12

//@ref USART_HwFlowCtl_define
#define USART_HwFlowCtl_NONE		0
#define USART_HwFlowCtl_RTS			1<<8	// Bit 8 RTSE: RTS enable
#define USART_HwFlowCtl_CTS			1<<9	// Bit 9 CTSE: CTS enable
#define USART_HwFlowCtl_RTS_CTS		(1<<8 | 1<<9)

//@ref USART_IRQ_Enable_define
#define USART_IRQ_Enable_NONE		0
#define USART_IRQ_Enable_TXE		1<<7
#define USART_IRQ_Enable_TC			1<<6
#define USART_IRQ_Enable_RXNE		1<<5
#define USART_IRQ_Enable_PE			1<<8


typedef enum{
	disable,
	enable
}Polling_Mechanism;

/*================== Baud rate Calculations ================*/
#define USARTDIV(_PCLK_,_BAUD_)			  (uint32_t)(_PCLK_ / (16 * _BAUD_))
#define USARTDIV_MUL100(_PCLK_,_BAUD_)	  (uint32_t)(25 * _PCLK_ / (4 * _BAUD_))
#define Mantissa(_PCLK_,_BAUD_)			  USARTDIV(_PCLK_,_BAUD_)
#define Mantissa_MUL100(_PCLK_,_BAUD_)	  USARTDIV(_PCLK_,_BAUD_) * 100
#define	DIV_Fraction(_PCLK_,_BAUD_)		  ((USARTDIV_MUL100(_PCLK_,_BAUD_) - Mantissa_MUL100(_PCLK_,_BAUD_)) * 16) / 100
#define USART_BBR(_PCLK_,_BAUD_)		  (Mantissa(_PCLK_,_BAUD_) << 4) | (DIV_Fraction(_PCLK_,_BAUD_) & 0xf)

/* ============== APIs Supported by "MCAL USART DRIVER" =========== */
extern void MCAL_USART_Init(USARTx_REG*,USART_Config_t*);
extern void MCAL_USART_DeInit(USARTx_REG*);
extern void MCAL_UASRT_GPIO_Set_Pins(USARTx_REG*);
extern void MCAL_USART_SendData(USARTx_REG*,uint16_t*,Polling_Mechanism);
extern void MCAL_USART_ReceiveData(USARTx_REG*,uint16_t*,Polling_Mechanism);
extern void MCAL_USART_WAIT_TC(USARTx_REG*);

#endif /* USART_H_ */
