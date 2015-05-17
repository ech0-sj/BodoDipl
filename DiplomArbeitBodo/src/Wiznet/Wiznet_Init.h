/*
 * Wiznet_Init.h
 *
 * Created: 16.05.2015 10:52:10
 *  Author: ech0
 */ 


#ifndef WIZNET_INIT_H_
#define WIZNET_INIT_H_


#include "wizchip_conf.h"
#include "../SPI/myspi.h"
#include "../config/conf_spi_master.h"
#include "../SysTimer/SysTimer.h"


InitResult_t W5500_Init(wiz_NetInfo* NetworkConfig);
void W5500_ResetHW(void);
void W5500_ConfigureResetPin( void );

#endif /* WIZNET_INIT_H_ */