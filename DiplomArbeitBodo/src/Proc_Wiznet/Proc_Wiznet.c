/*
* Proc_Wiznet.c
*
* Created: 24.05.2015 19:08:53
*  Author: ech0
*/

#include "Proc_Wiznet.h"
#include "string.h"
#include "malloc.h"

#include "../HTTPserver/httpServer.h"

WiznetReceiveBuffer gWizRecvbuffer[NUM_OF_WIZNET_SOCKETS];
extern uint8_t socklist[NUM_OF_WIZNET_SOCKETS];

int32_t ProcWiznet_TCPLoop(uint8_t sn, uint8_t* buf );




void ProcWiznet_Init( ProcessStruct* procStruct )
{
	procStruct->pid	= PID_Wiznet;
	procStruct->prio = Prio_Normal;
	procStruct->state = ProcState_Running;
	
	procStruct->specified = gWizRecvbuffer;
	
	for( int i = 0; i < NUM_OF_WIZNET_SOCKETS; i++ )
	{
			ProcWiznet_InitRecvBuffer( &gWizRecvbuffer[i] );
			socklist[i] = i; 
	}
}

void ProcWiznet_InitRecvBuffer( WiznetReceiveBuffer* recvBuffer )
{
	memset( recvBuffer->buffer, 0, DATA_BUF_SIZE );
	recvBuffer->nextWrite = 0;
}


void ProcWiznet( ProcessStruct* procStruct )
{
	SOCKET sock;
	
	for( sock = 0; sock < NUM_OF_WIZ_TCP_SOCKETS; sock++ )
	{
		ProcWiznet_TCPLoop( sock, gWizRecvbuffer->buffer );
	}
}


int32_t ProcWiznet_TCPLoop(uint8_t sn, uint8_t* buf )
{
	int32_t ret; // return value for SOCK_ERRORs
	uint16_t size = 0, sentsize=0;

	// aktueller Zustand im socket
	switch( getSn_SR(sn) )
	{
		// Bestehende Verbindung 
		case SOCK_ESTABLISHED:
		{
			// Verbindung gerade augebaut
			if(getSn_IR(sn) & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
			{
				// printf( "%d connection established \n", sn );
				setSn_IR(sn, Sn_IR_CON);  // this interrupt should be write the bit cleared to '1'
			}
			
			// Abhängig vom Socket HTTP verearbeitung 
			// oder eigene Verarbeitung 
			switch( getSn_PORT(sn) ) 
			{
				case WIZNET_HTTP_PORT:
					HttpServer_ProcessRequest( sn );
				break; 
				
				// weitere Protokolle hier 
				
				default: 
				break; 
			}
			
		}break;

		// Verbindung wird beendet 
		case SOCK_CLOSE_WAIT:
		{	
			if((ret=disconnect(sn)) != SOCK_OK) return ret;
		
			printf("log %d:Socket Closed\r\n", sn);
			
		}break;

		// Entweder verb. zu server aufbauen 
		// oder auf eingehende Verb. warten 
		case SOCK_INIT:
		{
			if( (ret = listen(sn)) != SOCK_OK) return ret;
			
			printf("log %d: Listening on port %d \n", sn, getSn_PORT( sn ) );
			
		}break;

		// keine Verbindung: socket neu öffnen  
		case SOCK_CLOSED:
		{
			uint16_t port; 
			
			if( sn < WIZNET_HTTP_SOCKET_COUNT )
			{
				port = WIZNET_HTTP_PORT;
			}
			else 
			{
				port = WIZNET_CUSTOM_PORT;
			}
			
			close(sn);
			if((ret=socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret; 
			printf( "log %d socket started \n", sn );
			
		}break;
		
		
		default:
		{
			
		}break;
	}
	return 1;
}

