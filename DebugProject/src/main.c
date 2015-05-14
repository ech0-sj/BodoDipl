/**
 *
 * Aus dem Beispiel 
 *	http://asf.atmel.com/docs/latest/sam.drivers.usart.usart_synchronous_example.sam3u_ek/html/usart_use_case_2.html
 * uebernommen
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_uart_serial.h"

int main (void)
{
	
	uint32_t readChar = 0; 
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	
	const sam_usart_opt_t usart_console_settings = {
		USART_SERIAL_BAUDRATE,
		USART_SERIAL_CHAR_LENGTH,
		USART_SERIAL_PARITY,
		USART_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};
	
	sysclk_enable_peripheral_clock(USART_SERIAL_ID);
	
	usart_init_rs232(USART_SERIAL, &usart_console_settings, sysclk_get_main_hz());
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
	usart_enable_interrupt(USART_SERIAL, US_IER_RXRDY);
	NVIC_EnableIRQ(USART_SERIAL_IRQ);
	
	/* Insert application code here, after the board has been initialized. */
	
	usart_putchar( USART_SERIAL, 'a' );
	
	while( 1 ) 
	{
		if( usart_is_tx_empty( USART_SERIAL ) )
		{
			usart_putchar( USART_SERIAL, 'a' );			
		}
		
		readChar = 0; 
		if( usart_is_rx_ready( USART_SERIAL ) )
		{
			usart_getchar( USART_SERIAL, &readChar );
		}
	}
}


void USART_SERIAL_ISR_HANDLER(void)
{
	uint32_t dw_status = usart_get_status(USART_SERIAL);
	if (dw_status & US_CSR_RXRDY) 
	{
		uint32_t received_byte;
		usart_read(USART_SERIAL, &received_byte);
		usart_write(USART_SERIAL, received_byte);
	}
}