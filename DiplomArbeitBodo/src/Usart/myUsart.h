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
#include "status_codes.h"

// portsetup in : conf_usart.h


#define ALL_INTERRUPT_MASK  0xffffffff

// Den Buffer zeitnah durch einen Ringbuffer tauschen 
// ansonsten gehts schnell kaputt
#define USART_RECV_LEN		512	/* 1k byte groﬂ */ 
typedef struct  
{
	uint32_t bufSize;
	uint32_t nextRead; 
	uint32_t nextWrite; 
	uint8_t buffer[USART_RECV_LEN];
}UsartReceiveBuffer;

typedef enum
{
	Serport_Wifi	= 0,
	Serport_Console,
	// add here
		
	Serport_Count	
}eSerialPort;


void myUSART_Init( Usart* serPort, uint32_t serPortID, IRQn_Type irqtype,  uint32_t baudrate);
void myUSART_Clear( Usart* serPort );
void myUSART_Write( Usart* serPort, uint8_t* buffer, uint32_t bufLen );

void myUSART_InitRecvBuffer( UsartReceiveBuffer* usartbuffer ); 
UsartReceiveBuffer* myUSART_GetRecvBuffer( eSerialPort serport );
enum status_code myUSART_GetByteFromBuffer( eSerialPort serport, uint8_t* readChar );
enum status_code myUSART_AddByteToBuffer( eSerialPort, uint8_t writeByte );

// always 8n1 by default ;) 
void USARTWifi_Init( uint32_t baudrate ); 
void USARTWifi_Clear( void ); 
void USARTWifi_Write( uint8_t* buffer, uint32_t bufLen );

void USARTCons_Init( uint32_t baudrate );
void USARTCons_Clear( void );
void USARTCons_Write( uint8_t* buffer, uint32_t bufLen );


#endif /* MYUSART_H_ */