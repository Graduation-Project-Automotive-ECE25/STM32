/*
 * ACC.h
 *
 *  Created on: Dec 10, 2024
 *      Author: assem
 */

#ifndef ACC_H_
#define ACC_H_

/*==========Genaric variables===========*/
extern uint16_t data;
extern uint8_t moving_Forward ;
extern uint8_t moving_Left ;
/*=====================================*/

/*==========APIs===========*/
void Forward();
void Backward();
void Left();
void Right();
void Stop();

void Bluetooth_Init();
void H_Bridge_Init();
void PWM_Init();

#endif /* ACC_H_ */
