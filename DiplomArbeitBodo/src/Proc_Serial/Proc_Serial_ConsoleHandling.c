/*
 * Proc_Serial_ConsoleHandling.c
 *
 * Created: 15.05.2015 08:47:44
 *  Author: ech0
 */ 
#include "Proc_Serial.h"


enum status_code ProcSerial_DoConsole( SerialCmdBuffer* cmdbuf )
{
	uint32_t pos = cmdbuf->nextWrite;
	
	// korrektur: pos steht schon auf nächsem Feld
	if( cmdbuf->buffer[pos -1] == PROCSER_CONS_ENDBYTE )
	{
		USARTCons_Write( cmdbuf->buffer, pos );
		ProcSerial_InitCmdBuf( cmdbuf );
	}
	return STATUS_OK;
}
