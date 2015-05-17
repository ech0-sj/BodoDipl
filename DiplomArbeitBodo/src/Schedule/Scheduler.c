/*
 * Scheduler.c

 * Created: 24.04.2015 18:13:52
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

/*
void BeispielProzess( ProcessStruct* procStruct )
{
	// ich mache irgendetwas 
	// muss aber mal fertig werden 
	// damit der nächste auch ran kommt
	
	if( HasNewData() )
	{
		
		DoSomethingWithData(); 
	}
	
	finished();
}

void BeispielProzess_MitZeitschlitz( ProcessStruct* ProcessStruct)
{
	bool stopped = false; 
	
	while( !stopped )
	{
		if( HasNewData() )
		{
			DoSomethingWithData(); 
		}
		
		// prozess abbrechen 
		if( needToStop() )
			stopped = true; 
	}
}
*/

