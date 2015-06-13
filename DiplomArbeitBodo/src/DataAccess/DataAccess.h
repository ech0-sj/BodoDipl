/*
 * Registers.h
 *
 * Created: 06.06.2015 10:30:12
 *  Author: ech0
 */ 


#ifndef REGISTERS_H_
#define REGISTERS_H_

#include "asf.h"
#include "DualportRAM/Dualportram_Storage.h"


#define DATA_DUALRAM_MIN		1
#define DATA_DUALRAM_MAX		DUALPORT_RAM_BUFFER_SIZE

#define DATA_ONTIME_MIN			3000
#define DATA_ONTIME_MAX			3010

/************************************************************************
*
*	Anfragen nach Aussen ( modbus, http, telnet, ... ) über diese 
*	Funktion stellen. Sie kapselt die Anfragen und ordnet die 
*	Register den korrekten Daten zu.
*
*	registerID:		Anfrage element 0 < registerID < 10000
*	out:			das gefundene Element
*	return:			true = Ok, false = NOK 
*
************************************************************************/
int DataAccess_GetElementAt( uint16_t registerID, int16_t* out ); 



#endif /* REGISTERS_H_ */