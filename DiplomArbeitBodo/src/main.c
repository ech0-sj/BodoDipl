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
#include <asf.h>
#include "Systimer/Systimer.h"
#include "SPI/mySPI.h"
#include "Usart/myUsart.h"
#include "Wiznet/wizchip_conf.h"


#define LED0_PIO		PIOB
#define LED0_MASK		(0x01 << 27)

__INLINE static void led_config(void)
{
	/* Set up LED pins. */
	LED0_PIO->PIO_PER = LED0_MASK;
	LED0_PIO->PIO_OER = LED0_MASK;
	LED0_PIO->PIO_PUDR = LED0_MASK;
}

__INLINE void SwitchOnLED0()
{
	LED0_PIO->PIO_SODR = LED0_MASK;
}

__INLINE void SwitchOffLED0()
{
	LED0_PIO->PIO_CODR = LED0_MASK;
}

void Test_SPI( void );


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
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	ioport_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000);
	
	led_config();
	SPIMaster_Init( SPICLK_1MHz );
	
	USARTWifi_Init( 9600 );
		
	while( 1 )
	{
		SwitchOnLED0();
		Delay_ms( 1000 );
		
		usart_putchar( WIFI_USART, 'a');
		SetupNetSetting( &gWIZNETINFO ); 
		W5500_Init( &gWIZNETINFO ); 
		
		SwitchOffLED0();
		Delay_ms( 1000 );
	}
}


void Test_SPI( void )
{	
	uint8_t buffer[100];
	uint8_t bufferIn[100];
	uint32_t buflen = 100; 
	volatile uint32_t i; 
	
	for( i = 0; i < buflen; i++ )
	{
		bufferIn[i] = i;
		buffer[i] = i; 
	}
	
	SPIMaster_Transfer( buffer, buflen ); 
	
	for( i = 0; i < buflen; i++ )
	{
		if( bufferIn[i] != buffer[i] )
		{
			// error 
			i = buflen * 2; 
		}
	}
	
}