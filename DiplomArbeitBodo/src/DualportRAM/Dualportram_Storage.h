/*
 * Dualportram_Storage.h
 *
 * Created: 25.05.2015 00:52:17
 *  Author: ech0
 */ 


#ifndef DUALPORTRAM_STORAGE_H_
#define DUALPORTRAM_STORAGE_H_

#include "asf.h"

// Vorgesehen: 2k Einträge a 16bit
#define DUALPORT_RAM_BUFFER_SIZE	2048


uint16_t* DualPortRAM_GetBuffer();

void DualPortRAM_UpdateDebug();

void DualPortRAM_InitDebug();





#endif /* DUALPORTRAM_STORAGE_H_ */