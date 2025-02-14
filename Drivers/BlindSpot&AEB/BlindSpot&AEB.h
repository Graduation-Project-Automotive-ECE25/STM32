/*
 * BlindSpot&AEB.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Yomna Awny Salh
 */

#ifndef BlindSPOT_AEB_H_
#define BlindSPOT_AEB_H_

#include "Ultrasonic.h"

/* ================== Settings ===========================*/
#define Green_LED_Port  			GPIOB
#define Green_LED_Pin				GPIO_PIN_6
#define Yellow_LED_Port				GPIOA
#define Yellow_LED_Pin				GPIO_PIN_11
#define Red_LED_Port				GPIOA
#define Red_LED_Pin					GPIO_PIN_12
/*======================================================*/

extern volatile float distance1;
extern volatile float distance2;
extern volatile float distance3;

/* ================ APIs ================*/


void Led_Init();
void Red_Led(uint8_t);
void Yellow_Led(uint8_t);
void Green_Led(uint8_t);
void Built_Led(uint8_t);


#endif /* BlindSPOT_AEB_H_ */
