/*
 * SysTickHandler.c
 *
 * Created: 05.05.15 11:33:49
 *  Author: selim
 */ 
#include "asf.h"
#include "SysTimer.h"
#include "../HTTPserver/httpServer.h"

/* Systick Counter */
static volatile uint64_t g_ul_ms_ticks = 0U;

void SysTick_Handler(void)
{
	/* Increment counter necessary in delay(). */
	g_ul_ms_ticks++;
	
	// sekundenzähler HTTP
	if( !(g_ul_ms_ticks % 1000 ) )
		httpServer_time_handler();
	
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


