/*
 * Dualportram_Storage.c
 *
 * Created: 25.05.2015 00:52:30
 *  Author: ech0
 */ 
#include "Dualportram_Storage.h"
#include "../SysTimer/SysTimer.h"

int16_t gDualportRAM_Buffer[DUALPORT_RAM_BUFFER_SIZE] = {0};

int16_t* DualPortRAM_GetBuffer()
{
	return 	&gDualportRAM_Buffer;
}

uint8_t DualPortRAM_GetItemAt( uint16_t index, int16_t* out )
{
	if( (index >= DUALPORT_RAM_BUFFER_SIZE) || (index == 0) )
	{
		return false;
	}
		
	*out = gDualportRAM_Buffer[index]; 
	return true;
}

void DualPortRAM_UpdateDebug()
{
	uint16_t* dualram = DualPortRAM_GetBuffer();
	
	for( int i = 2; i < DUALPORT_RAM_BUFFER_SIZE; i++ )
	{
		dualram[i] = i + (Get_systime_ms() % 32000 );
	}		
	
	// printf( "log dualram updated fst value %i\n", dualram[0]);
}

void DualPortRAM_UpdateSecond()
{
	uint16_t* dualram = DualPortRAM_GetBuffer();
	dualram[1] ++;
	if( dualram[1] > 32000 )
		dualram[1] = 0; 
}

void DualPortRAM_InitDebug()
{
	uint16_t* dualram = DualPortRAM_GetBuffer();
	
	for( int i = 0; i < DUALPORT_RAM_BUFFER_SIZE; i++ )
	{
		dualram[i] = i;
	}
}



