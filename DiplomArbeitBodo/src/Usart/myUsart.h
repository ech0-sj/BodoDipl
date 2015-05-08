/*
 * myUsart.h
 *
 * Created: 08.05.2015 14:22:15
 *  Author: ech0
 
 http://asf.atmel.com/docs/3.17.0/sam3x/html/usart_use_case_2.html
 */ 


#ifndef MYUSART_H_
#define MYUSART_H_

#include <asf.h>

// Falls auf einen anderen USART gewechselt wird, 
// müssen diese Defines angepasst werden 
#define WIFI_USART				USART0
#define WIFI_USART_ID			ID_USART0
#define WIFI_USART_IRQn			USART0_IRQn
#define WIFI_USART_IRQHANDLER	USART0_Handler

#define ALL_INTERRUPT_MASK  0xffffffff

// always 8n1 by default ;) 
void USARTWifi_Init( uint32_t baudrate ); 

void USARTWifi_Clear( void ); 

#endif /* MYUSART_H_ */