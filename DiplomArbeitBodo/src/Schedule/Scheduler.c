/*
 * Scheduler.c
 *
 * Created: 06.05.2015 18:13:52
 *  Author: ech0
 */ 

#include "Scheduler.h"

ScheduleStruct gScheduleInfo[PID_Count];

void Schedule_EmptyFunc( ProcessStruct* procStruct )
{
	// do nothing
}

void Scheduler_Init( void )
{
	ProcessIdentifier pid = PID_Start; 
	for( pid = PID_Start; pid < PID_Count; pid++ )
	{
		gScheduleInfo[pid].ProcFunc = Schedule_EmptyFunc;
		gScheduleInfo[pid].procStruct = NULL;
	}
	
}


void Scheduler_Register( ProcessStruct* procStruct, void (*ProcFunc)(ProcessStruct* procFunc) )
{
	ProcessIdentifier pid = procStruct->pid;
	
	gScheduleInfo[pid].ProcFunc = ProcFunc; 
	gScheduleInfo[pid].procStruct = procStruct;
}

void Scheduler_Schedule( void )
{
	ProcessIdentifier pid; 
	ProcessStruct* procStruct;
	
	pid = PID_Start;
	
	// Nacheinander alle Prozesse durchlaufen 
	// derzeit werden keine Prioritäten über wacht
	// aber schon der Zustand 
	while( 1 )
	{
		procStruct =  gScheduleInfo[pid].procStruct;
		if( procStruct != NULL  )
		{
			if( procStruct->state != ProcState_Paused )
				gScheduleInfo[pid].ProcFunc( gScheduleInfo[pid].procStruct ); 
		}
		
		pid += 1; 
		if( pid >= PID_Count )
			pid = 0; 
	}
}

