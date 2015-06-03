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



// Wird jede Seunde gerufen
void Systimer_OnSecondChanged()
{
	uint64_t mstimer = Get_systime_ms();
	
	httpServer_time_handler();
	
	DualPortRAM_UpdateSecond();
	if( !(mstimer % 10000) )
	{
		DualPortRAM_UpdateDebug();
		HTMLPagesUpdate_data();
	}
}