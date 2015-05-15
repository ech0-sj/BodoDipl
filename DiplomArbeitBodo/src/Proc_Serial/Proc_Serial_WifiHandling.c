/*
 * Proc_Serial_WifiHandling.c
 *
 * Created: 15.05.2015 08:46:56
 *  Author: ech0
 */ 

#include "Proc_Serial.h"

// debug -> auf cons ausgeben
enum status_code ProcSerial_DoWifi( SerialCmdBuffer* cmdbuf )
{
	uint32_t pos = cmdbuf->nextWrite;
	
	// korrektur: pos steht schon auf nächsem Feld
	if( cmdbuf->buffer[pos -1] == PROCSER_CONS_ENDBYTE )
	{
		USARTCons_Write( cmdbuf->buffer, pos );
		ProcSerial_InitCmdBuf( cmdbuf );
	}
	return STATUS_OK;
};