/*
 * myUsart.c
 *
 * Created: 08.05.2015 14:22:29
 *  Author: ech0
 */ 
#include "myUsart.h"


void USART0_Init( uint32_t baudrate )
{
	sam_usart_opt_t usart_console_settings = {
		baudrate,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		0
	};
	
	sysclk_enable_peripheral_clock( ID_USART0 );
	usart_init_rs232( USART0, &usart_console_settings, sysclk_get_main_hz());
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
}