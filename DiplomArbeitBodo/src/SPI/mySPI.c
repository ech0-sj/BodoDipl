/*
 * mySPI.c
 *
 * Created: 06.05.2015 08:42:37
 *  Author: ech0
 */ 

#include "mySPI.h"
#include "spi_master.h"

#ifdef VERSION_1

void SPIMaster_Init( eSPIClockConfig clockConfig )
{
	uint32_t spi_clock = gs_ul_clock_configurations[clockConfig];

	#if (SAMG55)
	/* Enable the peripheral and set SPI mode. */
	flexcom_enable(BOARD_FLEXCOM_SPI);
	flexcom_set_opmode(BOARD_FLEXCOM_SPI, FLEXCOM_SPI);
	#else
	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI_MASTER_BASE);
	#endif
	spi_disable(SPI_MASTER_BASE);
	spi_reset(SPI_MASTER_BASE);
	spi_set_lastxfer(SPI_MASTER_BASE);
	spi_set_master_mode(SPI_MASTER_BASE);
	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_PCS);
	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
	SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,
	(sysclk_get_cpu_hz() / spi_clock));
	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
	SPI_DLYBCT);
	spi_enable(SPI_MASTER_BASE);

}


void SPIMaster_Transfer(void *inOutBuffer, uint32_t size)
{
	uint32_t i;
	uint8_t uc_pcs;
	static uint16_t data;

	uint8_t *p_buffer;

	p_buffer = inOutBuffer;

	for (i = 0; i < size; i++) {
		// spi_write(SPI_MASTER_BASE, p_buffer[i], 0, 0);
		SPIMaster_WriteByte( p_buffer[i]);
		p_buffer[i] = 0xFF;
		/* Wait transfer done. */
		// while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
		
		p_buffer[i] = SPIMaster_ReadByte();
		// spi_read(SPI_MASTER_BASE, &data, &uc_pcs);
		// p_buffer[i] = data;
	}
}

void SPIMaster_WriteByte( uint8_t inBuf )
{
	spi_write(SPI_MASTER_BASE, inBuf, 0, 0);
	while ((spi_read_status(SPI_MASTER_BASE) & SPI_SR_RDRF) == 0);
}

uint8_t SPIMaster_ReadByte( void )
{
	uint8_t readByte;
	uint8_t uc_pcs;
	spi_read(SPI_MASTER_BASE, &readByte, &uc_pcs);
	return readByte;
}

void SPIMaster_SelectCS( void )
{
	ioport_set_port_level( IOPORT_PIOD, PIO_PD10, IOPORT_PIN_LEVEL_LOW );
}

void SPIMaster_DeselectCS( void )
{
	ioport_set_port_level( IOPORT_PIOD, PIO_PD10, IOPORT_PIN_LEVEL_LOW );
}


#else 


// #include "spi_master.h"

struct spi_device  gSPI_ETHDEVICE = { .id = 0, };

void SPIMaster_Init(eSPIClockConfig clockConfig )
{
	gSPI_ETHDEVICE.id = 0;
	uint32_t spiClock = gs_ul_clock_configurations[clockConfig];
	 
	spi_master_init( USED_SPI );
	spi_master_setup_device( USED_SPI, &gSPI_ETHDEVICE, SPI_MODE_3, spiClock, 0 );
	spi_enable( USED_SPI );
}

void SPIMaster_Transfer(uint8_t *inOutBuffer, uint32_t size )
{
	uint32_t i; 
	volatile status_code_t writeResult; 
	volatile status_code_t readResult;
	spi_select_device( USED_SPI, &gSPI_ETHDEVICE );
	writeResult = spi_write_packet( USED_SPI, inOutBuffer, size ); 
	
	for( i = 0; i < size; i++ )
		inOutBuffer[i] = 0xFF;
	
	readResult = spi_read_packet( USED_SPI, inOutBuffer, size ); 
	spi_deselect_device( USED_SPI, &gSPI_ETHDEVICE ); 
	
	if( readResult != writeResult )
	{
		i ++; 
	}
}

void SPIMaster_WriteByte( uint8_t inBuf )
{
	spi_select_device( USED_SPI, &gSPI_ETHDEVICE );
	spi_write_single( USED_SPI, inBuf );
	spi_deselect_device( USED_SPI, &gSPI_ETHDEVICE );
}

uint8_t SPIMaster_ReadByte( void )
{
	uint8_t outData = 0xFF; 
	spi_select_device( USED_SPI, &gSPI_ETHDEVICE);
	spi_read_single( USED_SPI, &outData );
	spi_deselect_device( USED_SPI, &gSPI_ETHDEVICE );
	
	return outData;
}

void SPIMaster_SelectCS( void )
{
	ioport_set_port_level( IOPORT_PIOD, PIO_PD10, IOPORT_PIN_LEVEL_LOW );
}

void SPIMaster_DeselectCS( void )
{
	ioport_set_port_level( IOPORT_PIOD, PIO_PD10, IOPORT_PIN_LEVEL_LOW );
}
 
 #endif 