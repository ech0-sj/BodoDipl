/*
 * Scheduler.h
 *
 * Created: 06.05.2015 18:13:35
 *  Author: ech0
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <asf.h>

typedef enum
{
	PID_Unknown,
	PID_Start = PID_Unknown,
	PID_Debug,
	PID_Serial,
	// add here
	
	PID_Count
	
}ProcessIdentifier;

typedef enum
{
	Prio_Low,
	Prio_Normal,
	Prio_High,
	
}ProcessPrio;

typedef enum
{
	ProcState_Unknown,
	ProcState_Paused, 
	ProcState_Running,
}ProcessState;

typedef struct  
{
	ProcessIdentifier pid;
	ProcessPrio prio;
	ProcessState state;
	
	void* specified;		// Process specifig Datastructure
	
}ProcessStruct;



typedef struct 
{
	void (*ProcFunc)(ProcessStruct* procFunc);
	ProcessStruct* procStruct;
		
}ScheduleStruct;

void Scheduler_Init( void );
void Scheduler_Register( ProcessStruct* procStruct, void (*ProcFunc)(ProcessStruct* procFunc) );
void Scheduler_Schedule( void ); 




#endif /* SCHEDULER_H_ */