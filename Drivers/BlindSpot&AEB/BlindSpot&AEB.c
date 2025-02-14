/*
 * BlindSpot&AEB.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Yomna Awny Salh
 */
#include <BlindSpot&AEB.h>

void Blind_spot_right(void){

	distance2 = calculateDistance2();

	if(distance2 >= 65)
	{

		Green_Led(1);
		Red_Led(0);
		Yellow_Led(0);
	}
	else if(distance2>=25 && distance2<65)
	{

		Yellow_Led(1);
		Green_Led(0);
		Red_Led(0);

		//Uart1_send("Closer Right");

	}
	else if( distance2 < 25){
		Red_Led(1);
		Green_Led(0);
		Yellow_Led(0);

		//Uart1_send("Danger Right");

	}
}



/*LED*/
void Led_Init(){
	//Red Led PA12
	GPIO_PinConfig_t pincfg;
	pincfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	pincfg.GPIO_PinNumber = Red_LED_Pin;
	pincfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10M;

	MCAL_GPIOx_Init(Red_LED_Port, &pincfg);

	//Yellow Led PA11
	pincfg.GPIO_PinNumber = Yellow_LED_Pin;
	MCAL_GPIOx_Init(Yellow_LED_Port, &pincfg);

	//Green Led PB6
	pincfg.GPIO_PinNumber = Green_LED_Pin;
	MCAL_GPIOx_Init(Green_LED_Port, &pincfg);

	// PC13
	pincfg.GPIO_PinNumber = GPIO_PIN_13;
	MCAL_GPIOx_Init(GPIOC, &pincfg);

}

void Red_Led(uint8_t x){

	MCAL_GPIOx_WritePin(Red_LED_Port, Red_LED_Pin,x);
}

void Yellow_Led(uint8_t x){

	MCAL_GPIOx_WritePin(Yellow_LED_Port, Yellow_LED_Pin,x);
}

void Green_Led(uint8_t x){

	MCAL_GPIOx_WritePin(Green_LED_Port, Green_LED_Pin,x);
}

void Built_Led(uint8_t x){
	MCAL_GPIOx_WritePin(GPIOC, GPIO_PIN_13,x);
}
