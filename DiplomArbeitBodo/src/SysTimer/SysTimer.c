/*
 * SysTickHandler.c
 *
 * Created: 05.05.15 11:33:49
 *  Author: selim
 */ 
#include "asf.h"
#include "SysTimer.h"
#include "../HTTPserver/httpServer.h"
#include "../DualportRAM/Dualportram_Storage.h"
#include "../HTTPserver/html_pages.h"

/* Systick Counter */
static volatile uint64_t g_ul_ms_ticks = 0U;

void Systime_Second_Timer();

void SysTick_Handler(void)
{
	/* Increment counter necessary in delay(). */
	g_ul_ms_ticks++;
	
	// sekundenzähler HTTP
	if( !(g_ul_ms_ticks % 1000 ) )
		Systime_Second_Timer();
}


uint64_t Get_systime_ms()
{
	return g_ul_ms_ticks;
}

/**
 * \brief Delay number of tick Systicks (happens every 1 ms).
 */
__inline void Delay_ms(uint32_t delayInMs)
{
	volatile uint32_t ul_cur_ticks;

	ul_cur_ticks = Get_systime_ms();
	while ((Get_systime_ms() - ul_cur_ticks) < delayInMs) {
	}
}


// Wird jede Seunde gerufen 
void Systime_Second_Timer()
{
	httpServer_time_handler();
	
	if( !(g_ul_ms_ticks % 10000) )
	{
		DualPortRAM_UpdateDebug();
		HTMLPages_UpdateDataPage();
	}
}


