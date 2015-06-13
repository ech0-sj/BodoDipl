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

#include "LED.h"
#include "Systimer/Systimer.h"
#include "Usart/myUsart.h"

#include "SPI/mySPI.h"
#include "Wiznet/wizchip_conf.h"
#include "Wiznet/Wiznet_Init.h"
#include "Wiznet/wizchip_conf.h"

#include "Schedule/Scheduler.h"
#include "Proc_Serial/Proc_Serial.h"
#include "Proc_Wiznet/Proc_Wiznet.h"

#include "Http/HttpSimple.h"
#include "Http/HtmlPages.h"


int main (void)
{
	// wiz_NetInfo* pNetInfo = GetWiznetInfo();
	uint32_t consoleBaudrate = CONS_USART_BAUD;
	uint32_t wifiBaudrate = WIFI_USART_BAUD;
	eSPIClockConfig spiclock = SPICLK_1MHz;
	
	ProcessStruct procSerial; 
	ProcessStruct procWiznet;
	
	netmode_type netMode; 
	uint32_t netModeAsint;
	
	HtmlPageDescriptor htmldescriptor;
	
// begin initphase	
	
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000);
	
	/* init.c ... Die Pins für MISO, MOSI, CLK, 
	und USART RX USART TX werden per gpio() eingestellt
	nutzt die Defines aus conf_board.h */ 
	board_init();
	ioport_init();
	
	// SPI initialisieren 
	SPIMaster_Init( spiclock );
	
	// Wiznet mit default werten belegen
	W5500_ConfigureIOPins();
	W5500_Init(  );
	
	// Usarts initialisieren
	USARTWifi_Init( wifiBaudrate );
	USARTCons_Init( consoleBaudrate );
	
	
	// init Status LEDs 
	led_config();

	// ProzessManager anlegen 
	Scheduler_Init( );

	// Wiznez Process -> Kommunikation direkt mit dem Chip 
	ProcWiznet_Init( &procWiznet ); 
	Scheduler_Register( &procWiznet, ProcWiznet ); 
	
	// den seriellen Empf. Prozess anlegen 
	ProcSerial_Init( &procSerial );
	Scheduler_Register( &procSerial, ProcSerial ); 
	
	
	// TODO weitere Prozesse hier erstellen 
	
	
	htmldescriptor = HTML_CreatePage_Index();
	HTTP_RegisterPage( &htmldescriptor ); 
	
	htmldescriptor = HTML_CreatePage_Info();
	HTTP_RegisterPage( &htmldescriptor );
	
	htmldescriptor = HTML_CreatePage_Data();
	HTTP_RegisterPage( &htmldescriptor );
	
	htmldescriptor = HTML_CreatePage_DataRange();
	HTTP_RegisterPage( &htmldescriptor );
	
	htmldescriptor = HTML_CreatePage_Setting();
	HTTP_RegisterPage( &htmldescriptor );
	htmldescriptor = HTML_CreatePage_SettingEthernet();
	HTTP_RegisterPage( &htmldescriptor );
	htmldescriptor = HTML_CreatePage_SettingModbus();
	HTTP_RegisterPage( &htmldescriptor );
	
	
	// init fertig -> welcome zeigen 
	printf( "Bodo Janssen ... arduino due...\n" );
	printf( "Welcome \n");
	printf( "init done, start application \n" );
	
	// Applikation starten 	
	Scheduler_Schedule();	
	
	return;
}



