/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "asf.h"
#include "string.h"
#include "conf_uart_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

#include "Systimer/Systimer.h"
#include "SPI/mySPI.h"
#include "Usart/myUsart.h"
#include "Wiznet/wizchip_conf.h"

#include "Proc_Serial/Proc_Serial.h"
#include "Schedule/Scheduler.h"


void Test_SPI( void );
void SetupNetSetting( wiz_NetInfo* wiznetInfo );
__INLINE void SwitchOnLED0( void );
__INLINE void SwitchOffLED0( void );
__INLINE static void led_config(void);


#define LED0_PIO		PIOB
#define LED0_MASK		(0x01 << 27)


wiz_NetInfo gWIZNETINFO = {
	.mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
	.ip = {192, 168, 1, 12},
	.sn = {255, 255, 255, 0},
	.gw = {192, 168, 1, 1},
	.dns = {0, 0, 0, 0},
.dhcp = NETINFO_STATIC };

extern uint32_t gDEBUGVAR;

int main (void)
{
	uint32_t consoleBaudrate = CONS_USART_BAUD;
	uint32_t wifiBaudrate = WIFI_USART_BAUD;
	eSPIClockConfig spiclock = SPICLK_500kHz;
	
	ProcessStruct procSerial; 
	
	
	uint8_t usartMsg[100]; 
	uint32_t usartMsglen; 
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000);
	
	/* init.c ... Die Pins für MISO, MOSI, CLK, 
	und USART RX USART TX werden per gpio() eingestellt */ 
	board_init();
	ioport_init();
	
	// SPI initialisieren 
	SPIMaster_Init( spiclock );
			
	// Usarts initialisieren
	USARTWifi_Init( wifiBaudrate );
	USARTCons_Init( consoleBaudrate );
	
	// init Status LEDs 
	led_config();


	// ProzessManager anlegen 
	Scheduler_Init( );

	// den seriellen Empf. Prozess anlegen 
	ProcSerial_Init( &procSerial );
	Scheduler_Register( &procSerial, ProcSerial ); 
	
	// init fertig -> welcome zeigen 
	printf( "Bodo Janssen ... arduino due...\n" );
	printf( "Welcome \n");
	printf( "init done, start application \n" );
		
		
		
	// Applikation starten 	
	Scheduler_Schedule();	
		
		
		return ;
		
		
		
	while( 1 )
	{
		SwitchOnLED0();
		Delay_ms( 100 );
		/*
		strcpy( usartMsg, "AT+GMR\r\n");
		usartMsglen = strlen( usartMsg );
		
		 USARTWifi_Write( usartMsg, usartMsglen );
		// usart_putchar( WIFI_USART, 'a');
		// SetupNetSetting( &gWIZNETINFO ); 
		// W5500_Init( &gWIZNETINFO ); 
		// Test_SPI();
		
		
		ProcSerial( &procSerial );
		*/ 
		
		
		SwitchOffLED0();
		Delay_ms( 100 );
	}
}


void Test_SPI( void )
{	
	uint8_t buffer[100];
	uint32_t buflen = 100; 
	volatile uint32_t i; 
	
	for( i = 0; i < buflen; i++ )
	{
		buffer[i] = i; 
	}
	
	SPIMaster_Transfer( buffer, buflen ); 
	
}


__INLINE static void led_config(void)
{
	/* Set up LED pins. */
	LED0_PIO->PIO_PER = LED0_MASK;
	LED0_PIO->PIO_OER = LED0_MASK;
	LED0_PIO->PIO_PUDR = LED0_MASK;
}

__INLINE void SwitchOnLED0( void )
{
	LED0_PIO->PIO_SODR = LED0_MASK;
}

__INLINE void SwitchOffLED0( void )
{
	LED0_PIO->PIO_CODR = LED0_MASK;
}


void SetupNetSetting( wiz_NetInfo* wiznetInfo )
{
	wiznetInfo->mac[0] = 0x00;
	wiznetInfo->mac[1] = 0x08;
	wiznetInfo->mac[2] = 0xdc;
	wiznetInfo->mac[3] = 0xdc;
	wiznetInfo->mac[4] = 0xef;
	
	wiznetInfo->ip[0] = 192;
	wiznetInfo->ip[1] = 168;
	wiznetInfo->ip[2] = 1;
	wiznetInfo->ip[3] = 12;
	
	wiznetInfo->sn[0] = 255;
	wiznetInfo->sn[1] = 255;
	wiznetInfo->sn[2] = 255;
	wiznetInfo->sn[3] = 0;
	
	wiznetInfo->gw[0] = 192;
	wiznetInfo->gw[1] = 168;
	wiznetInfo->gw[2] = 1;
	wiznetInfo->gw[3] = 1;
	
	wiznetInfo->dns[0] = 0;
	wiznetInfo->dns[1] = 0;
	wiznetInfo->dns[2] = 0;
	wiznetInfo->dns[3] = 0;
	wiznetInfo->dhcp = NETINFO_STATIC;
}
