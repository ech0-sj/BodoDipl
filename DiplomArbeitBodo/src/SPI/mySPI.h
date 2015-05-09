/*
 * mySPI.h
 *
 * Created: 06.05.2015 08:42:23
 *  Author: ech0
 */ 

/* 
http://ledcube.protospace.ca/blog/posts/arduino-due-sam3x8e-spi-slave-with-raspberry-pi-master

http://www.avrfreaks.net/forum/need-help-setting-spi-interface-lsb-first-sam3x8e-using-asf

*/ 

#ifndef MYSPI_H_
#define MYSPI_H_

#include <asf.h>
// #include <spi.h>

 #define VERSION_1


#define USED_SPI	SPI0

/* SPI clock configuration. */
static const uint32_t gs_ul_clock_configurations[] =
{ 1, 500000, 1000000, 2000000, 5000000 };



typedef enum
{
	SPICLK_1Hz,
	SPICLK_500kHz,
	SPICLK_1MHz,
	SPICLK_2MHz,
	SPICLK_5MHz,
	
}eSPIClockConfig;


#ifdef VERSION_1
 
 /** SPI base address for SPI master mode*/
 #define SPI_MASTER_BASE		USED_SPI
 

 /* Chip select. */
 #define SPI_CHIP_SEL 0
 #define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)

 /* Delay before SPCK. */
 #define SPI_DLYBS 0x40

 /* Delay between consecutive transfers. */
 #define SPI_DLYBCT 0x10



 /* Clock polarity. */
 #define SPI_CLK_POLARITY 0
 /* Clock phase. */
 #define SPI_CLK_PHASE 0


 void SPIMaster_Init(eSPIClockConfig clockConfig );

 void SPIMaster_Transfer(void *inOutBuffer, uint32_t size );

 void SPIMaster_WriteByte( uint8_t inBuf );
 uint8_t SPIMaster_ReadByte( void );

 void SPIMaster_SelectCS( void );
 void SPIMaster_DeselectCS( void );


#else 

void SPIMaster_Init(eSPIClockConfig clockConfig );

void SPIMaster_Transfer(uint8_t *inOutBuffer, uint32_t size );

void SPIMaster_WriteByte( uint8_t inBuf );
uint8_t SPIMaster_ReadByte( void );

void SPIMaster_SelectCS( void );
void SPIMaster_DeselectCS( void );

#endif



#endif /* MYSPI_H_ */