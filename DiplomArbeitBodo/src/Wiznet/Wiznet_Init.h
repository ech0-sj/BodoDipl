/*
 * Wiznet_Init.h
 *
 * Created: 16.05.2015 10:52:10
 *  Author: ech0
 */ 


#ifndef WIZNET_INIT_H_
#define WIZNET_INIT_H_

#include "asf.h"
#include "../config/conf_board.h"
#include "wizchip_conf.h"
#include "../SPI/myspi.h"
#include "../config/conf_spi_master.h"
#include "../SysTimer/SysTimer.h"


InitResult_t W5500_Init( void );
void W5500_ResetHW(void);
void W5500_ConfigureIOPins( void );

void SetupNetSetting( wiz_NetInfo* wiznetInfo );
wiz_NetInfo* GetWiznetInfo();

#endif /* WIZNET_INIT_H_ */