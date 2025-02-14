/*
 * TIM2to4.c
 *
 *  Created on: Oct 11, 2024
 *      Author: Yomna Awny Salh
 */

#include <BlindSpot&AEB.h>
#include "TIM2to4.h"

#define CLR_BIT(reg,bitN) (reg&= ~(1<<bitN))

vuint32_t risingEdgeTime1 = 0; // Time captured on the rising edge
vuint32_t fallingEdgeTime1 = 0; // Time captured on the falling edge
vuint8_t captureState1 = 0; // 0 for rising edge, 1 for falling edge
volatile float distance1;

vuint32_t risingEdgeTime2 = 0;
vuint32_t fallingEdgeTime2 = 0;
vuint8_t captureState2 = 0;
volatile float distance2;

vuint32_t risingEdgeTime3 = 0;
vuint32_t fallingEdgeTime3 = 0;
vuint8_t captureState3 = 0;
volatile float distance3;


void TIM2TO4_Init_InputCaptureMode(void){
	//Enable Timers
	TIMER2_CLK_EN();
	TIMER3_CLK_EN();
	TIMER4_CLK_EN();

	//Choose the mapping of ICx on TIx (TI2 on IC1).
	//Important to miss with CCxS before setting CCxE
	US_1->CCMR1 |=(1<<CC1S_TI1);
	US_2->CCMR2 |=(1<<CC4S_TI4);
	US_3->CCMR1 |=(1<<CC2S_TI2);


	US_1->CCMR1 &=~(1<<CC1S_TI2);
	US_2->CCMR2 &=~(1<<CC4S_TI3);
	US_3->CCMR1 &=~(1<<CC2S_TI1);

	//Enable Input Capture Mode
	US_1->CCER |=(1<<CC1E);
	US_2->CCER |=(1<<CC4E);
	US_3->CCER |=(1<<CC2E);


	//Enable Counters
	US_1->CR1 |=(1<<0);
	US_2->CR1 |=(1<<0);
	US_3->CR1 |=(1<<0);



	//Choose the filter (No filter)
	US_1->CCMR1 &=~(1<<IC1F1);
	US_1->CCMR1 &=~(1<<IC1F2);
	US_1->CCMR1 &=~(1<<IC1F3);
	US_1->CCMR1 &=~(1<<IC1F4);

	US_2->CCMR1 &=~(1<<IC4F1);
	US_2->CCMR1 &=~(1<<IC4F2);
	US_2->CCMR1 &=~(1<<IC4F3);
	US_2->CCMR1 &=~(1<<IC4F4);

	US_3->CCMR1 &=~(1<<IC2F1);
	US_3->CCMR1 &=~(1<<IC2F2);
	US_3->CCMR1 &=~(1<<IC2F3);
	US_3->CCMR1 &=~(1<<IC2F4);



	//Choose prescaler (none)
	US_1->CCMR1 &=~(1<<IC1P1);
	US_1->CCMR1 &=~(1<<IC1P2);

	US_2->CCMR1 &=~(1<<IC4P1);
	US_2->CCMR1 &=~(1<<IC4P2);

	US_3->CCMR1 &=~(1<<IC2P1);
	US_3->CCMR1 &=~(1<<IC2P2);


	US_1->PSC = 7;
	US_1->ARR = 0xFFFF;

	US_2->PSC = 7;
    US_2->ARR = 0xFFFF;

    US_3->PSC = 7;
    US_3->ARR = 0xFFFF;


	//Define trigger
	US_1->CCER &=~(1<<CC1P);
	US_2->CCER &=~(1<<CC4P);
	US_3->CCER &=~(1<<CC2P);


	//Enable Interrupt(for IC1)
	US_1->DIER |=(1<<CC1IE);
	NVIC_IRQ29_TIMER3_Enable();

	US_2->DIER |=(1<<CC4IE);
	NVIC_IRQ30_TIMER4_Enable();

	US_3->DIER |=(1<<CC2IE);
	NVIC_IRQ28_TIMER2_Enable();



}

void TIM2_IRQHandler(void){

        if (captureState3 == 0) {
            risingEdgeTime3 = US_3->CCR[1];  // Read the captured value for the rising edge
            US_3->CCER |= (1<<CC2P);   // Set to capture falling edge next
            captureState3 = 1;               // Change state to falling edge
        } else {
            fallingEdgeTime3 = US_3->CCR[1];  // Read the captured value for the falling edge
            US_3->CCER &=~ (1<<CC2P);  // Set to capture rising edge next
            captureState3 = 0;               // Change state to rising edge
            distance3 = calculateDistance3();
        }

        CLR_BIT(US_3->SR,CC2IF); // Clear the interrupt flag
}

void TIM3_IRQHandler(void){

        if (captureState1 == 0) {
            risingEdgeTime1 = US_1->CCR[0];  // Read the captured value for the rising edge
            US_1->CCER |= (1<<CC1P);   // Set to capture falling edge next
            captureState1 = 1;               // Change state to falling edge
        } else {
            fallingEdgeTime1 = US_1->CCR[0];  // Read the captured value for the falling edge
            US_1->CCER &=~ (1<<CC1P);  // Set to capture rising edge next
            captureState1 = 0;               // Change state to rising edge
            distance1 = calculateDistance1();
        }

        CLR_BIT(US_1->SR,CC1IF); // Clear the interrupt flag
}


void TIM4_IRQHandler(void){

        if (captureState2 == 0) {
            risingEdgeTime2 = US_2->CCR[3];  // Read the captured value for the rising edge
            US_2->CCER |= (1<<CC4P);   // Set to capture falling edge next
            captureState2 = 1;               // Change state to falling edge
        } else {
            fallingEdgeTime2 = US_2->CCR[3];  // Read the captured value for the falling edge
            US_2->CCER &=~ (1<<CC4P);  // Set to capture rising edge next
            captureState2 = 0;               // Change state to rising edge
            distance2 = calculateDistance2();
        }

        CLR_BIT(US_2->SR,CC4IF); // Clear the interrupt flag
}

void delayTimer_init(void)
{
	TIMER1_CLK_EN();  			 //Enable CLK for TIMER1

	TIMER1->PSC = 7;            //Clk_input=(8M/(7+1))=1MHZ
	TIMER1->ARR = 0xC350;        //to make interrupt after 50000 tick(50000*10^-6)= 0.05s
	TIMER1->CR1 |=(1<<0);		//Enable timer

}

void dus(int us)
{
	TIMER1->CNT=0;
	while((TIMER1->CNT)<us);
}

void dms(int ms)
{
	int i=0;
	for(i=0;i<ms;i++)
	{
		dus(1000);
	}
}
