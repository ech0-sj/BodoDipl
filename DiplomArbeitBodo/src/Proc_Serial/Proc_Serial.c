/*
 * Proc_Serial.c
 *
 * Created: 15.05.2015 00:43:25
 *  Author: ech0
 */ 

#include "Usart/myUsart.h"
#include "Proc_Serial.h"
#include "SysTimer/SysTimer.h"
#include "string.h"

void ProcSerial_InitCmdBuf( SerialCmdBuffer* buffer ); 
enum status_code ProcSerial_DoConsole( SerialCmdBuffer* cmdbuf );
enum status_code ProcSerial_DoWifi( SerialCmdBuffer* cmdbuf );

serialProcStruct gSerProcStruct;



void ProcSerial()
{
	uint32_t serport = 0; 
	uint8_t readChar;  
	SerialCmdBuffer* cmdBuf; 
	
	// Durch alle definierten seriellen Schnittstellen gehen 
	// und neue Daten prüfen und versuchen Nachrichten auszuwerten  
	for( serport = 0; serport < Serport_Count; serport++ )
	{
		cmdBuf = &gSerProcStruct.cmdbuf[serport]; 
		
		// alle empfangenen Bytes auswerten 
		// 1. angefangene Nachricht in Buffer merken ( immer nur eine Nachricht darin )
		// 2. empfangszeit merken 
		// 3. Auswerten 
		while( myUSART_GetByteFromBuffer(serport, &readChar ) == STATUS_OK )
		{
			cmdBuf->buffer[cmdBuf->nextWrite] = readChar; 
			cmdBuf->nextWrite += 1;
			cmdBuf->lastRecv = Get_systime_ms();
			  
			switch( serport )
			{
				case Serport_Console: 
				ProcSerial_DoConsole( cmdBuf ); 
				break; 
				
				case Serport_Wifi: 
				ProcSerial_DoWifi( cmdBuf );
				break; 
			}
		}
		
		// Falls eine gewisse Zeit keine Bytes mehr empfangen werden
		// wird der angefangene Teil verworfen
		if( (cmdBuf->lastRecv + PROCSER_CHAR_TIMEOUT) < Get_systime_ms() )
			ProcSerial_InitCmdBuf( cmdBuf );		
	}
}

void ProcSerial_Init(void)
{
	uint32_t serport; 
	
	for( serport = 0; serport < Serport_Count; serport ++ )
	{
		ProcSerial_InitCmdBuf( &gSerProcStruct.cmdbuf[serport] );
	}
}

void ProcSerial_InitCmdBuf( SerialCmdBuffer* cmdBuf )
{
	cmdBuf->lastRecv = 0; 
	cmdBuf->nextWrite = 0; 
	memset( cmdBuf->buffer, 0, CMD_BUF_MAX_LEN );	
}



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