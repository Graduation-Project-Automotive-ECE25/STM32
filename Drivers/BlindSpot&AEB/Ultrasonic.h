/*
 * Ultransonic.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Yomna Awny Salh
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "Tim2to4.h"

/* =========================  Settings =======================*/
//Trigger
#define UltraSonic1_Trigger_Port			GPIOA
#define UltraSonic1_Trigger_Pin				GPIO_PIN_7
#define UltraSonic2_Trigger_Port			GPIOA
#define UltraSonic2_Trigger_Pin				GPIO_PIN_8
#define UltraSonic3_Trigger_Port			GPIOA
#define UltraSonic3_Trigger_Pin				GPIO_PIN_5
//Echo
#define UltraSonic1_Echo_Port				GPIOA
#define UltraSonic1_Echo_Pin				GPIO_PIN_6
#define UltraSonic2_Echo_Port				GPIOB
#define UltraSonic2_Echo_Pin				GPIO_PIN_9
#define UltraSonic3_Echo_Port				GPIOA
#define UltraSonic3_Echo_Pin				GPIO_PIN_1
/*===========================================================*/
extern vuint32_t risingEdgeTime1; // Time captured on the rising edge
extern vuint32_t fallingEdgeTime1; // Time captured on the falling edge
extern vuint8_t captureState1; // 0 for rising edge, 1 for falling edge
extern volatile float distance1;

extern vuint32_t risingEdgeTime2;
extern vuint32_t fallingEdgeTime2;
extern vuint8_t captureState2;
extern volatile float distance2;

extern vuint32_t risingEdgeTime3;
extern vuint32_t fallingEdgeTime3;
extern vuint8_t captureState3;
extern volatile float distance3;
/* ================ APIs ================*/
extern void UltraSonic_Pin_Init(void);

extern float calculateDistance1(void);
extern float calculateDistance2(void);
extern float calculateDistance3(void);

#endif /* ULTRASONIC_H_ */
