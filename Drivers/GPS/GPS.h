/*
 * GPS.h
 *
 *  Created on: Oct 18, 2024
 *      Author: assem
 */

#ifndef GPS_H_
#define GPS_H_

#include "USART.h"
#include <string.h>

/*======== Settings ========*/
#define USARTx 			USART1    // Choose between USART1 , USART2 , USART3
/*==========================*/

uint8_t location_data[40],done;

/*=================== APIs ========================*/
extern void GPS_Init();
extern void Get_Location(float*, float*);  //Arguments are the addresses of Latitude and Longitude Variables

#endif /* GPS_H_ */
