#include "GPS.h"
#include "BlindSpot&AEB.h"
#include "ACC.h"
#include "Scheduler.h"
#include <stdio.h>

Task_ref Task1, Task2 , Task3, Task4  ;

void GPS ()
{
	while(1)
	{
		float x,y;
		if(done == 1 ){
			Get_Location(&x,&y);
			char str[27]={0};
			snprintf(str, sizeof(str), "x;%d.%07d\ny;%d.%07d\n",(int)x,(int)(x*10000000)%10000000,(int)y,(int)(y*10000000)%10000000);
			for(int i = 0; i<26; i++){
				MCAL_USART_SendData(USART1, &str[i], enabled);
			}
		}
		MY_RTOS_Task_Wait(1000, &Task1);
	}
}

void UltraSonic_Triggering ()
{
	while(1)
	{
		MCAL_GPIOx_WritePin(UltraSonic1_Trigger_Port, UltraSonic1_Trigger_Pin,1);
		MCAL_GPIOx_WritePin(UltraSonic2_Trigger_Port, UltraSonic2_Trigger_Pin,1);
		MCAL_GPIOx_WritePin(UltraSonic3_Trigger_Port, UltraSonic3_Trigger_Pin,1);

		dus(10);

		MCAL_GPIOx_WritePin(UltraSonic1_Trigger_Port, UltraSonic1_Trigger_Pin,0);
		MCAL_GPIOx_WritePin(UltraSonic2_Trigger_Port, UltraSonic2_Trigger_Pin,0);
		MCAL_GPIOx_WritePin(UltraSonic3_Trigger_Port, UltraSonic3_Trigger_Pin,0);

		MY_RTOS_activateTask(&Task3);
		MY_RTOS_Task_Wait(50, &Task2);
	}
}

uint16_t CCR_value = 0;

void BlindSpot_and_AEB ()
{
	while(1)
	{
		/* ============= Emergency Braking ==========*/

		if(distance1 <= 25)
		{
			if(moving_Forward == 1){
				Stop();
				CCR_value = 0;
			}
			Built_Led(0);
			//Uart1_send("crash Front");
		}
		else{
			Built_Led(1);
		}
		/*===========================================*/


		/* =============== Left Blind Spot ===========*/

		if(distance3 >= 50)
		{
			Green_Led(1);
			Red_Led(0);
			Yellow_Led(0);
		}
		else if(distance3 >= 25 && distance3 < 50)
		{
			Yellow_Led(1);
			Green_Led(0);
			Red_Led(0);

			//Uart1_send("Closer Left");

		}
		else if( distance3 < 25){

			if(moving_Left == 1) Stop();

			Red_Led(1);
			Green_Led(0);
			Yellow_Led(0);

			//Uart1_send("Danger Left");
		}
		/* =========================================*/


		MY_RTOS_terminateTask(&Task3);
	}
}

void move(){

	while(1){

		switch(data){

		case 'w':
			if(distance1 > 25){
				if(CCR_value < 65535 - 100 ) CCR_value += 50;
				TIMER3->CCR[2] = CCR_value;
				TIMER3->CCR[3] = CCR_value;
				Forward();
			}
			break;

		case 's':
			Backward();
			break;

		case 'a':
			if(distance3 > 25) Left();
			break;

		case 'd':
			Right();
			break;

		case '0':
			Stop();
			TIMER3->CCR[2] = 65535;
			TIMER3->CCR[3] = 65535;
			CCR_value = 0;
			break;

		case 'f':
			CCR_value = (((((uint16_t)distance1) * 65535) / 400) + 8000 )%65535;
			TIMER3->CCR[2] = CCR_value;
			TIMER3->CCR[3] = CCR_value;
			Forward();
			break;

		case 'M':
			TIMER3->CCR[2] = 65535;
			TIMER3->CCR[3] = 65535;
			break;

		}

		char str[6]={0};
		uint8_t percentage = (CCR_value * 100) / 65535;

		if(percentage < 99){
			snprintf(str, sizeof(str), "v;%d\n", percentage);
			for(int i = 0; i<5; i++)
				MCAL_USART_SendData(USART1, &str[i], enabled);
		}
	}

}

int main(void)
{
	Error_Type error = NoError;

	HW_Init();
	GPS_Init();
	delayTimer_init();
	TIM2TO4_Init_InputCaptureMode();
	UltraSonic_Pin_Init();
	Led_Init();
	Bluetooth_Init();
	H_Bridge_Init();
	PWM_Init();
	USARTx->CR1 |= USART_Mode_Tx;

	if ( MY_RTOS_Init() != NoError )
		while(1) ;


	Task1.stack_size = 768 ;
	Task1.p_TaskEntry = GPS ;
	Task1.priority = 3 ;
	strcpy(Task1.TaskName ,"GPS");

	Task2.stack_size = 128 ;
	Task2.p_TaskEntry = UltraSonic_Triggering ;
	Task2.priority = 2 ;
	strcpy(Task2.TaskName ,"UltraSonic_Triggering");


	Task3.stack_size = 128 ;
	Task3.p_TaskEntry = BlindSpot_and_AEB ;
	Task3.priority = 1 ;
	strcpy(Task3.TaskName ,"BlindSpot_and_AEB");

	Task4.stack_size = 280 ;
	Task4.p_TaskEntry = move ;
	Task4.priority = 3 ;
	strcpy(Task4.TaskName ,"move");

	error += MY_RTOS_createTask(&Task1);
	error += MY_RTOS_createTask(&Task2);
	error += MY_RTOS_createTask(&Task3);
	error += MY_RTOS_createTask(&Task4);

	MY_RTOS_activateTask(&Task1);
	MY_RTOS_activateTask(&Task2);

	MY_RTOS_activateTask(&Task4);

	MY_RTOS_Start();


	while (1)
	{

	}

}
