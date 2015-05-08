/*
 * myUsart.c
 *
 * Created: 08.05.2015 14:22:29
 *  Author: ech0
 */ 
#include "myUsart.h"

//PDC* gUSART0_PDC = 0;

uint32_t gDEBUGVAR = 0; 


void USARTWifi_Init( uint32_t baudrate )
{
	sam_usart_opt_t usart_console_settings = {
		baudrate,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		0
	};
	
	sysclk_enable_peripheral_clock( WIFI_USART_ID );
	usart_init_rs232( WIFI_USART, &usart_console_settings, sysclk_get_main_hz());
	
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);
	// usart_enable_interrupt( WIFI_USART, US_IER_TXRDY);
	// usart_enable_interrupt( WIFI_USART, US_IER_RXBUFF ); 
	
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}


void USARTWifi_Clear( void )
{
	usart_reset_rx( WIFI_USART ); 
	usart_reset_tx( WIFI_USART ); 
	
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
}

void WIFI_USART_IRQHANDLER( void )
{
	// kam er an 
	volatile uint32_t ul_status = usart_get_status(WIFI_USART);
	uint32_t readChar = 0xFF; 
	
	if (ul_status & US_CSR_RXRDY) 
	{
		usart_getchar(WIFI_USART, (uint32_t *)&readChar);
		gDEBUGVAR = 1; 
		// usart_write(WIFI_USART, readChar);
	}
	if( ul_status & US_CSR_TXRDY )
	{
		gDEBUGVAR = 10; 
	}
}
