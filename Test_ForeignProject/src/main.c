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
#include "spi_master.h"

int main (void)
{
	status_code_t status;
	uint8_t dataTx[] = "abcd";
	uint32_t mySPIselect = 1;
	uint32_t myBaudRate = 1;
	spi_flags_t mySPIflags = SPI_MODE_3;
	struct spi_device mySPI = {
		.id = 0
	};
	
	sysclk_init();
	board_init();
	
	spi_master_init( SPI0 );
	spi_master_setup_device(SPI0, &mySPI, mySPIflags, myBaudRate, mySPIselect);
	spi_enable( SPI0 );
	
	for (;;)
	{
		status = spi_write_packet(SPI0, &dataTx[0], sizeof(dataTx));
	}
}