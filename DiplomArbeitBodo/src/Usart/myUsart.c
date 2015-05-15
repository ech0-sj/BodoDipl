/*
 * myUsart.c
 *
 * Created: 08.05.2015 14:22:29
 *  Author: ech0
 */ 
#include "myUsart.h"
#include "string.h"

//PDC* gUSART0_PDC = 0;

uint32_t gDEBUGVAR = 0; 

UsartReceiveBuffer gUsartRecvBuffer[Serport_Count];


void myUSART_Init( Usart* serPort, uint32_t serPortID, IRQn_Type irqtype,  uint32_t baudrate)
{
	sam_usart_opt_t usart_console_settings = {
		baudrate*2,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		0
	};
	
	sysclk_enable_peripheral_clock( serPortID );
	usart_init_rs232( serPort, &usart_console_settings, sysclk_get_main_hz());
	
	usart_enable_tx(serPort);
	usart_enable_rx(serPort);
	
	usart_enable_interrupt(serPort, US_IER_RXRDY);
	
	NVIC_EnableIRQ(irqtype);
	
}

void myUSART_Clear( Usart* serPort )
{
	usart_reset_rx( serPort );
	usart_reset_tx( serPort );
	
	usart_enable_tx( serPort );
	usart_enable_rx( serPort );
}

void myUSART_Write( Usart* serPort, uint8_t* buffer, uint32_t bufLen )
{
	uint32_t i;

	for( i = 0; i < bufLen; i++ )
	{
		usart_putchar( serPort, buffer[i]);
	}
}



void myUSART_InitRecvBuffer( UsartReceiveBuffer* usartbuffer )
{
	usartbuffer->bufSize = USART_RECV_LEN;
	usartbuffer->nextRead = 0;
	usartbuffer->nextWrite = 0;
	memset( usartbuffer->buffer, 0, usartbuffer->bufSize );
}

UsartReceiveBuffer* myUSART_GetRecvBuffer( eSerialPort serport )
{
	if( serport >= Serport_Count )
		return NULL;
		
	return &gUsartRecvBuffer[serport]; 
}

enum status_code myUSART_GetByteFromBuffer( eSerialPort serport, uint8_t* readChar )
{
	UsartReceiveBuffer* recvBuffer;
	
	recvBuffer = myUSART_GetRecvBuffer( serport );
	if( recvBuffer == NULL )
		return ERR_INVALID_ARG;
	
	if( recvBuffer->nextRead != recvBuffer->nextWrite )
	{
		*readChar = recvBuffer->buffer[recvBuffer->nextRead];
		recvBuffer->nextRead += 1;
		
		if( recvBuffer->nextRead >= recvBuffer->bufSize )
		recvBuffer->nextRead = 0;
		
		return STATUS_OK;
	}
	
	return ERR_BAD_DATA;
}

enum status_code myUSART_AddByteToBuffer( eSerialPort serport, uint8_t writeByte )
{
	UsartReceiveBuffer* recvBuffer;
	
	recvBuffer = myUSART_GetRecvBuffer( serport );
	if( recvBuffer == NULL )
	return ERR_INVALID_ARG;
	
	recvBuffer->buffer[recvBuffer->nextWrite] = writeByte;
	recvBuffer->nextWrite += 1;
	
	if( recvBuffer->nextWrite >= recvBuffer->bufSize )
	recvBuffer->nextWrite = 0;
	
	return STATUS_OK;
}



void USARTWifi_Init( uint32_t baudrate )
{
	myUSART_InitRecvBuffer( &gUsartRecvBuffer[Serport_Wifi] );
	return myUSART_Init( WIFI_USART, WIFI_USART_ID, WIFI_USART_IRQn, baudrate );
}

void USARTWifi_Clear( void )
{
	return myUSART_Clear( WIFI_USART );
}

void USARTWifi_Write( uint8_t* buffer, uint32_t bufLen )
{
	return myUSART_Write( WIFI_USART, buffer, bufLen ); 
}

void WIFI_USART_IRQHANDLER( void )
{
	volatile uint32_t ul_status = usart_get_status(WIFI_USART);
	uint32_t readChar = 0xFF; 
	
	if (ul_status & US_CSR_RXRDY) 
	{
		usart_getchar(WIFI_USART, (uint32_t *)&readChar);
		myUSART_AddByteToBuffer( Serport_Wifi, readChar ); 
	}
}


void USARTCons_Init( uint32_t baudrate )
{
	usart_serial_options_t usart_console_settings = {
		baudrate*2,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
	};
	
	myUSART_InitRecvBuffer( &gUsartRecvBuffer[Serport_Console] );
	
	stdio_serial_init( CONS_USART, &usart_console_settings);

	return myUSART_Init( CONS_USART, CONS_USART_ID, CONS_USART_IRQn, baudrate );
}

void USARTCons_Clear( void )
{
	return myUSART_Clear( CONS_USART );
}

void USARTCons_Write( uint8_t* buffer, uint32_t bufLen )
{
	return myUSART_Write( CONS_USART, buffer, bufLen ); 
}

void CONS_USART_IRQHANDLER( void )
{
	volatile uint32_t ul_status = usart_get_status(CONS_USART);
	uint32_t readChar = 0xFF;
	
	if (ul_status & US_CSR_RXRDY)
	{
		usart_getchar(CONS_USART, (uint32_t *)&readChar);
		// usart_putchar( CONS_USART, readChar );

		myUSART_AddByteToBuffer( Serport_Console, readChar ); 
	}
}