/*
 * Wiznet_Init.c
 *
 * Created: 16.05.2015 10:52:25
 *  Author: ech0
 */ 

#include "Wiznet_Init.h"



void W5500_ConfigureIOPins( void )
{
	ioport_set_pin_dir(	W5500_RESET_PIN, IOPORT_DIR_OUTPUT );
	ioport_set_pin_level(W5500_RESET_PIN, IOPORT_PIN_LEVEL_HIGH );
	
	ioport_set_pin_dir(	W5500_CS_PIN, IOPORT_DIR_OUTPUT );
	ioport_set_pin_level(W5500_CS_PIN, IOPORT_PIN_LEVEL_HIGH );
	
}

void W5500_ResetHW(void)
{
	ioport_set_pin_level(W5500_RESET_PIN, IOPORT_PIN_LEVEL_LOW );
	ioport_set_pin_level(W5500_CS_PIN, IOPORT_PIN_LEVEL_LOW );
	Delay_ms( 3 ); 
	ioport_set_pin_level(W5500_RESET_PIN, IOPORT_PIN_LEVEL_HIGH );
	ioport_set_pin_level(W5500_CS_PIN, IOPORT_PIN_LEVEL_HIGH );
}


InitResult_t W5500_Init(wiz_NetInfo* NetworkConfig)
{
	uint8_t tmp;
	uint8_t memsize[2][8] = {
		{2,2,2,2,2,2,2,2} /* Configure RX Socket Size in kByte*/
		,{2,2,2,2,2,2,2,2} /* Configure TX Socket Size in kByte*/
	};
	
	// w5500_init_io();		//Set the additional IOs to the correct value and direction
	
	// spi_init_sw();			// Software-SPI-Treiber laden
	/* Critical section callback - No use in this example */
	//reg_wizchip_cris_cbfunc(0, 0);
	
	
	/* Chip selection call back */
	#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc( SPIMaster_SelectCS, SPIMaster_DeselectCS );		//Register Functionpointer for Chipselect in Driver
	#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
	reg_wizchip_cs_cbfunc( SPIMaster_SelectCS, SPIMaster_DeselectCS );	//Register Functionpointer for Chipselect in Driver
	#else
	#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
	#error "Unknown _WIZCHIP_IO_MODE_"
	#else
	reg_wizchip_cs_cbfunc( SPIMaster_SelectCS, SPIMaster_DeselectCS );		//Register Functionpointer for Chipselect in Driver
	#endif
	#endif
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc( SPIMaster_ReadByte, SPIMaster_WriteByte );				//Register Functionpointer for Datatransfers in Driver
	/* Enter- und Exit-Critical, cli() and sei()...*/
	//reg_wizchip_cris_cbfunc(enter_critical,exit_critical);		///Use with care !
	////////////////////////////////////////////////////////////////////////

	
	W5500_ResetHW();
	
	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		return InitError;	//Init went wrong
	}
	
	/* PHY link status check */
	if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
	{
		return PhyError;	//Phy in suspect state
	}
	
	//This is wehre the "Black Magic" happens
	//
	
	ctlnetwork(CN_SET_NETINFO,NetworkConfig);	//Write Config in Chip
	
	ctlnetwork(CN_GET_NETINFO,NetworkConfig);	//Read Config Back
	
	
	return Okay;								//Return Okay
	//Black Magic done :-), as you given a pointer to your Network-Config, you'll get the current config back from the chip
}
