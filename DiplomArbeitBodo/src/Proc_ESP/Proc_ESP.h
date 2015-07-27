/*
 * Proc_ESP.h
 *
 * Created: 21.07.2015 19:46:59
 *  Author: ech0
 */ 


#ifndef PROC_ESP_H_
#define PROC_ESP_H_

#include "asf.h"
#include "Schedule/Scheduler.h"
#include "Usart/myUsart.h"
#include "stdio.h"
#include "ESP8266/ESP8266.h"

typedef enum
{
	ESPState_start,
	ESPState_start_res,
	
	ESPState_getVersion,				// 2
	ESPState_getVersion_res,
	
	ESPState_setMode,					// 4
	ESPState_setMode_res,
		
	ESPState_setSoftAP,					// 6
	ESPState_setSoftAP_res,

	ESPState_softAPReboot,				// 8
	ESPState_softAPReboot_res,
	
	ESPState_SetupIP,					// 10
	ESPState_SetupIP_res,
	
	ESPState_setMultiConnection,		// 12
	ESPState_setMultiConnection_res,

	ESPState_ServerMode,				// 14
	ESPState_ServerMode_res,
		
	ESPState_Server,					// 16
	ESPState_Server_running,		
	
	ESPState_SendAsyncHead,				// 18
	ESPState_SendAsyncHead_res,
	
	ESPState_SendAsyncPart,				// 20
	ESPState_SendAsyncPart_res,
	
	ESPState_SendAsyncClose,			// 22
	ESPState_SendAsyncClose_res,
	
	ESPState_max
}ePRocEspState;


typedef union
{
	uint32_t asInt;
	
	uint8_t	major; 
	uint8_t minor; 
	uint16_t build;
}uVersion;


typedef struct  
{
	ePRocEspState procState; 
	ePRocEspState lastState;
	uVersion ESPVersion;
	
}ProcEspStruct;

void ProcESP( ProcessStruct* procStruct );
eProcResult ProcESP_Receive( ProcEspStruct* espStruct, uint8_t* inBuffer, uint32_t inBuflen);
eProcResult ProcESP_DoReceive( uint8_t* msg, uint32_t len );

void ProcESP_Init( ProcessStruct* procStruct );
void ProcESP_InitProcstruct( ProcEspStruct* espStruct );

ProcEspStruct* ProcESP_getProcptr( );
void ProcEsp_UpdateState( ePRocEspState newState );




#endif /* PROC_ESP_H_ */