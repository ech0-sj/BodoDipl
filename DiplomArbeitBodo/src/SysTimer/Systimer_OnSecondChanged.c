/*
 * Systimer_OnSecondChanged.c
 *
 * Created: 25.05.2015 14:05:56
 *  Author: ech0
 */ 


#include "SysTimer.h"
#include "../HTTPserver/httpServer.h"
#include "../DualportRAM/Dualportram_Storage.h"
#include "../HTTPserver/html_pages.h"

void Systimer_UpdateRunningtime( void );


// Wird jede Seunde gerufen
void Systimer_OnSecondChanged()
{
	uint64_t mstimer = Get_systime_ms();
	
	Systimer_UpdateRunningtime(  ); 
	
	
	if( !(mstimer % 10000) )
	{
		DualPortRAM_UpdateDebug();
	}
}



void Systimer_UpdateRunningtime(  )
{
	TimeStruct* timer = Systimer_GetRunningTime();
	
	timer->second += 1;
	if( timer->second >= 60 )
	{
		timer->minute += 1;
		timer->second = 0;
	}
	
	if( timer->minute >= 60 )
	{
		timer->hour += 1;
		timer->minute = 0;
	}
	
	if( timer->hour >= 24 )
	{
		timer->day += 1;
		timer->hour = 0;
	}
}

