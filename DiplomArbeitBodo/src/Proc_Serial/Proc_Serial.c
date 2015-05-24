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
#include "malloc.h"

enum status_code ProcSerial_InterpreteRecv( eSerialPort serport, SerialCmdBuffer* cmdBuf, uint8_t readChar );
serialProcStruct gSerProcStruct;



void ProcSerial( ProcessStruct* procStruct )
{
	eSerialPort serport = 0; 
	uint8_t readChar;  
	serialProcStruct* serprocStruct; 
	SerialCmdBuffer* cmdBuf; 
	
	serprocStruct = (serialProcStruct*)procStruct->specified;
	
	// Durch alle definierten seriellen Schnittstellen gehen 
	// und neue Daten prüfen und versuchen Nachrichten auszuwerten  
	for( serport = 0; serport < Serport_Count; serport++ )
	{
		cmdBuf = &serprocStruct->cmdbuf[serport]; 
		
		// alle empfangenen Bytes auswerten 
		// 1. angefangene Nachricht in Buffer merken ( immer nur eine Nachricht darin )
		// 2. empfangszeit merken 
		// 3. Auswerten 
		while( myUSART_GetByteFromBuffer(serport, &readChar ) == STATUS_OK )
		{
			ProcSerial_InterpreteRecv( serport, cmdBuf, readChar ); 	
		}
		
		// Falls eine gewisse Zeit keine Bytes mehr empfangen werden
		// wird der angefangene Teil verworfen
		if( (cmdBuf->lastRecv + PROCSER_CHAR_TIMEOUT) < Get_systime_ms() )
			ProcSerial_InitCmdBuf( cmdBuf );		
	}
}

void ProcSerial_Init( ProcessStruct* procStruct )
{
	uint32_t serport; 
	
	procStruct->pid	= PID_Serial;
	procStruct->prio = Prio_Normal;
	procStruct->state = ProcState_Running;
	procStruct->specified = &gSerProcStruct;		// todo: hier mit malloc erzeugen 
	
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



enum status_code ProcSerial_InterpreteRecv( eSerialPort serport, SerialCmdBuffer* cmdBuf, uint8_t readChar )
{
	cmdBuf->buffer[cmdBuf->nextWrite] = readChar; 
	cmdBuf->nextWrite += 1;
	cmdBuf->lastRecv = Get_systime_ms();
	
	// Auf den jeweiligen Schnittstellen sind 
	// die Protokolle fix. Es muss nichts geprüft werden
	switch( serport )
	{
		case Serport_Console:
		ProcSerial_DoConsole( cmdBuf );
		break;
		
		case Serport_Wifi:
		ProcSerial_DoWifi( cmdBuf );
		break;
		
		
		// weitere Protokolle hier 
		
	}
	
	return STATUS_OK;
}


