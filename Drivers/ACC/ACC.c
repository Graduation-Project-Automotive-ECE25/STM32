/*
 * ACC.c
 *
 *  Created on: Dec 10, 2024
 *      Author: assem
 */

/*========INCLUDES========*/
#include "USART.h"
#include "GPIO.h"
/*======================*/

/*==========Genaric variables===========*/
uint16_t data;
uint8_t moving_Forward;
uint8_t moving_Left;
/*=====================================*/


void Receive2();

void Forward(){

	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	moving_Forward = 1;
	moving_Left = 0;
}

void Backward(){

	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	moving_Forward = 0;
	moving_Left = 0;
}

void Left(){

	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	moving_Forward = 0;
	moving_Left = 1;
}

void Right(){

	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	moving_Forward = 0;
	moving_Left = 0;
}


void Stop(){

	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	MCAL_GPIOx_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	moving_Forward = 0;
	moving_Left = 0;
}

void Bluetooth_Init(){

	USART_Config_t usart_cfg;

	usart_cfg.BaudRate = USART_BaudRate_9600;
	usart_cfg.HwFlowCtl = USART_HwFlowCtl_NONE;
	usart_cfg.IRQ_Enable = USART_IRQ_Enable_RXNE;
	usart_cfg.P_IRQ_CallBack = Receive2;
	usart_cfg.Parity = USART_Parity_NONE;
	usart_cfg.Payload_Length = USART_Payload_Length_8B;
	usart_cfg.StopBits = USART_StopBits_1;
	usart_cfg.USART_Mode = USART_Mode_Rx;

	MCAL_USART_Init(USART2, &usart_cfg);
	MCAL_UASRT_GPIO_Set_Pins(USART2);

}

void H_Bridge_Init(){

	uint16_t PIN[4] = {GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
	GPIO_PinConfig_t gpio_cfg;

	gpio_cfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	gpio_cfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;

	for(int i = 0; i<4; i++){

		gpio_cfg.GPIO_PinNumber = PIN[i];
		MCAL_GPIOx_Init(GPIOB, &gpio_cfg);

	}
}

void PWM_Init(){

	TIMER3->CR1 |= 1 << 7; // enable ARPE: Auto-reload preload
	TIMER3->CCMR2 = 0x6868; //PWM Mode1 for channel 3 and 4
	TIMER3->CCER |= 1<<12 | 1<<8; // Compare channel 3 and 4 output enable

	GPIO_PinConfig_t Pincfg;

	Pincfg.GPIO_PinNumber = GPIO_PIN_0;
	Pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
	Pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;

	MCAL_GPIOx_Init(GPIOB,&Pincfg);

	Pincfg.GPIO_PinNumber = GPIO_PIN_1;

	MCAL_GPIOx_Init(GPIOB,&Pincfg);

	TIMER3->CCR[2] = 65535;
	TIMER3->CCR[3] = 65535;

}


void Receive2(){
	//if the interrupt occurs to receive another byte
	MCAL_USART_ReceiveData(USART2, &data, disabled);
}
