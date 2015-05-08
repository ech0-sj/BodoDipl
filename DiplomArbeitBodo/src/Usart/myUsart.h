/*
 * myUsart.h
 *
 * Created: 08.05.2015 14:22:15
 *  Author: ech0
 */ 


#ifndef MYUSART_H_
#define MYUSART_H_

#include <asf.h>

#define WIFI_USART	USART0

// always 8n1 by default ;) 
void USART0_Init( uint32_t baudrate ); 


#endif /* MYUSART_H_ */