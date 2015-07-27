/*
 * Proc_Serial_WifiHandling.c
 *
 * Created: 15.05.2015 08:46:56
 *  Author: ech0
 */ 

#include "Proc_Serial.h"
#include "Proc_ESP/Proc_ESP.h"


enum status_code ProcSerial_DoWifi( SerialCmdBuffer* cmdbuf )
{
	
	ProcEspStruct* espStruct = ProcESP_getProcptr();
	int32_t pos = cmdbuf->nextWrite;
	uint8_t* ptr = cmdbuf->buffer;
		
	// Das ende Zeichen suchen 
	for( pos = cmdbuf->nextWrite; (pos > cmdbuf->lastSeen); pos-- )
	{
		if(cmdbuf->buffer[pos] == '\n')
			break;
	}
	cmdbuf->lastSeen = cmdbuf->nextWrite;
	
	
	
	// Befehle enden mit \r\n	NICHT \r\r\n
	if( (cmdbuf->buffer[pos-2] !='\r') && (cmdbuf->buffer[pos-1] =='\r') && (cmdbuf->buffer[pos] =='\n') )
	{				
		// Nachricht in die auswertung geben
		// nur wenn vollständig den Buffer löschen		
		if( ProcESP_Receive( espStruct, cmdbuf->buffer, pos ) != ProcResult_WaitForMore )
		{			
			ProcSerial_InitCmdBuf( cmdbuf );
		}
	}
	return STATUS_OK;
};