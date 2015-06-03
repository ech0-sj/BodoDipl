/*
 * Proc_Serial_ConsoleHandling.c
 *
 * Created: 15.05.2015 08:47:44
 *  Author: ech0
 */ 
#include "Proc_Serial.h"
#include "string.h"
#include "Wiznet/Wiznet_Init.h"
#include "../Tools/Tools.h"
#include "../Persistent/PersistentItems.h"

void HandleOwnSerialProtocol( uint8_t* buffer, uint32_t len );



enum status_code ProcSerial_DoConsole( SerialCmdBuffer* cmdbuf )
{
	uint32_t pos = cmdbuf->nextWrite;
	
	// korrektur: pos steht schon auf nächsem Feld
	if( cmdbuf->buffer[pos -1] == PROCSER_CONS_ENDBYTE )
	{
		// verarbeiten 
		HandleOwnSerialProtocol( cmdbuf->buffer, cmdbuf->nextWrite-1 );
		
		// commandbuffer leeren 
		ProcSerial_InitCmdBuf( cmdbuf );
	}
	return STATUS_OK;
}




void HandleOwnSerialProtocol( uint8_t* buffer, uint32_t len )
{
	uint32_t idx = 0;
	
	// aus dem letzten Zeichen eine bin 0 machen ( string ende ) 
	buffer[len] = '\0';
	
	// Befehle prüfen, mindest länge 3byte
	if( len <= 4 )
	return;
	
	// help 
	if( strstr(buffer, "help") )
	{
		printf( "HELP - command \n");
		printf( "Set LAN IP:	set lan 192.168.x.y\n" );
		printf( "Get LAN IP:	get lan\n" );
		
		return;
	}
	// set lan 
	else if( strstr(buffer, "set lan ") )
	{
		wiz_NetInfo* netinfo = GetWiznetInfo();
		uint8_t ipArray[4];
		
		idx = 8; 
		if( !ParseIpToArray( &buffer[idx], &ipArray) )
		{
			SaveIPWiznet( ipArray );
			W5500_Init(  ); 
			printf( "set lan OK\n");
		}
		else 
		{
			printf( "set lan NOK\n");
		}
		
		return; 
	}
	else if( strstr(buffer, "get lan") )
	{
		wiz_NetInfo* netinfo = GetWiznetInfo();
		
		printf( "get lan IP: %i.%i.%i.%i \n", netinfo->ip[0], netinfo->ip[1], netinfo->ip[2], netinfo->ip[3] );
		return;
	}
	
	
	printf( "unknown command \n");
	return;
}