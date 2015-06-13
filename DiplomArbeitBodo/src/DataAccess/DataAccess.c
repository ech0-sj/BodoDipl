/*
 * DataAccess.c
 *
 * Created: 06.06.2015 10:31:07
 *  Author: ech0
 */ 

#include "DataAccess.h"
#include "SysTimer/SysTimer.h"

uint8_t IsDualRamArea( uint16_t regId )
{
	if( (regId < DATA_DUALRAM_MIN) || (regId > DATA_DUALRAM_MAX) )
		return false; 
		
	return true;
}



uint8_t IsOntimeArea( uint16_t regId )
{
	if( (regId < DATA_ONTIME_MIN) || (regId > DATA_ONTIME_MAX) )
	return false;
	
	return true;
}

uint8_t DataAccess_GetTimerElement( uint16_t regId, int16_t* out );


int DataAccess_GetElementAt( uint16_t registerID, int16_t* out )
{
	// Anfrage im gültigen Bereich?
	if( (registerID < 1) || (registerID >= 10000) )
		return false; 
		
	
	if( IsDualRamArea(registerID) )
	{
		return DualPortRAM_GetItemAt( registerID, out ); 
	}
	else if( IsOntimeArea(registerID) )
	{
		return DataAccess_GetTimerElement( registerID, out ); 
	}
	else 
	{
		return false; 		
	}
	
}





uint8_t DataAccess_GetTimerElement( uint16_t regId, int16_t* out )
{
	TimeStruct* timer = Systimer_GetRunningTime();
	uint64_t runningms = Get_systime_ms();
	
	switch( regId )
	{
		case 3000: 
			*out = timer->day;
			return true; 
		
		case 3001:
			*out = timer->hour;
			return true;
			
		case 3002:
			*out = timer->minute;
			return true;
			
		case 3003:
			*out = timer->second;
			return true;
			
			
		case 3004:
			*out = (int16_t)(runningms >> 48);
			return true; 
		
		case 3005:
			*out = (int16_t)(runningms >> 32);
			return true;
		
		case 3006:
			*out = (int16_t)(runningms >> 16);
			return true;
		
		case 3008:
			*out = (int16_t)(runningms & 0xFFFF);
			return true;
		
		default: return false; 
	}
}
