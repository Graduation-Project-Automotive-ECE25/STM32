/*
 * GPS.c
 *
 *  Created on: Oct 18, 2024
 *      Author: assem
 */

#include "GPS.h"

void Receive(){

	uint8_t ch;
	static uint8_t serial_flag,i,data_flag,comma_count,gprmc_data[40];

	MCAL_USART_ReceiveData(USARTx, &ch, disable);

	if(data_flag == 0){
		if(ch == '$'){
			serial_flag = 1;
			done = 0;
			i = 0;
		}

		if(serial_flag == 1){
			gprmc_data[i++] = ch;

			if(ch == ','){
				if(strncmp(gprmc_data,"$GPRMC,",7) == 0){
					data_flag = 1;
				}
				serial_flag = 0;
				i = 0;
			}
		}
	}else{
		gprmc_data[i++] = ch;
		if(ch == ','){
			comma_count++;
			if(comma_count == 5){
				strcpy(location_data,gprmc_data);
				comma_count = 0;
				data_flag = 0;
				done = 1;
			}
		}
	}

}

void GPS_Init(){

	USART_Config_t usart_cfg;

	usart_cfg.BaudRate = USART_BaudRate_9600;
	usart_cfg.HwFlowCtl = USART_HwFlowCtl_NONE;
	usart_cfg.IRQ_Enable = USART_IRQ_Enable_RXNE;
	usart_cfg.P_IRQ_CallBack = Receive;
	usart_cfg.Parity = USART_Parity_NONE;
	usart_cfg.Payload_Length = USART_Payload_Length_8B;
	usart_cfg.StopBits = USART_StopBits_1;
	usart_cfg.USART_Mode = USART_Mode_Rx;

	MCAL_UASRT_GPIO_Set_Pins(USARTx);
	MCAL_USART_Init(USARTx, &usart_cfg);

}


void Get_Location(float* ptr2Latitude, float* ptr2Longitude ){
	//Get the first token
	char* token = strtok(location_data, ",");

	//walk through other tokens
	uint8_t i = 0;
	char* tokens[5];

	while(token != NULL){
		tokens[i++] = token;
		token = strtok(NULL,",");
	}

	float Latitude=atof(tokens[2]);
	int Latitude_Degree = (int)Latitude/100;
	float Latitude_Minutes = Latitude - Latitude_Degree * 100;
	*ptr2Latitude = Latitude_Degree + Latitude_Minutes / 60;


	float Longitude=atof(tokens[4]);
	int Longitude_Degree = (int)Longitude/100;
	float Longitude_Minutes = Longitude - Longitude_Degree * 100;
	*ptr2Longitude = Longitude_Degree + Longitude_Minutes / 60;

	done = 0;
}
