/*
 * Wiznet_Init.c
 *
 * Created: 16.05.2015 10:52:25
 *  Author: ech0
 */ 

#include "Wiznet_Init.h"
#include "../Persistent/PersistentItems.h"


wiz_NetInfo gWIZNETINFO = {
	.mac = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
	.ip = {192, 168, 1, 12},
	.sn = {255, 255, 255, 0},
	.gw = {192, 168, 1, 1},
	.dns = {0, 0, 0, 0},
.dhcp = NETINFO_STATIC };


wiz_NetInfo* GetWiznetInfo()
{
	return &gWIZNETINFO;
}


void SetupNetSetting( wiz_NetInfo* wiznetInfo )
{
	/*
	wiznetInfo->mac[0] = 0x00;
	wiznetInfo->mac[1] = 0x08;
	wiznetInfo->mac[2] = 0xdc;
	wiznetInfo->mac[3] = 0xdc;
	wiznetInfo->mac[4] = 0xef;
	
	wiznetInfo->ip[0] = 192;
	wiznetInfo->ip[1] = 168;
	wiznetInfo->ip[2] = 1;
	wiznetInfo->ip[3] = 111;
	
	wiznetInfo->sn[0] = 255;
	wiznetInfo->sn[1] = 255;
	wiznetInfo->sn[2] = 255;
	wiznetInfo->sn[3] = 0;
	*/
	
	LoadMACWiznet( wiznetInfo->mac ); 
	LoadSubnetWiznet( wiznetInfo->sn ); 
	LoadIPWiznet( wiznetInfo->ip ); 
	
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


InitResult_t W5500_Init( void )
{
	 wiz_NetInfo* pNetInfo = GetWiznetInfo();
	uint8_t tmp;
	uint8_t memsize[2][8] = {
		{2,2,2,2,2,2,2,2} /* Configure RX Socket Size in kByte*/
		,{2,2,2,2,2,2,2,2} /* Configure TX Socket Size in kByte*/
	};
	
	// w5500_init_io();		//Set the additional IOs to the correct value and direction
	
	// spi_init_sw();			// Software-SPI-Treiber laden
	/* Critical section callback - No use in this example */
	//reg_wizchip_cris_cbfunc(0, 0);
	
	
	SetupNetSetting( pNetInfo ); 
	
	
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
	
	ctlnetwork(CN_SET_NETINFO,pNetInfo);	//Write Config in Chip
	
	ctlnetwork(CN_GET_NETINFO,pNetInfo);	//Read Config Back
	
	
	return Okay;								//Return Okay
	//Black Magic done :-), as you given a pointer to your Network-Config, you'll get the current config back from the chip
}
