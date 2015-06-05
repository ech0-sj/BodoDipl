/*
 * simpleHttp.c
 *
 * Created: 04.06.2015 20:38:51
 *  Author: ech0
 */ 

#include "Proc_Wiznet.h"
#include "../Tools/Tools.h"
#include "string.h"
#include "../HTTPserver/html_pages.h"

extern uint8_t gHttpRxBuffer[DATA_BUF_SIZE];
extern uint8_t gHttpTxBuffer[DATA_BUF_SIZE];



void DoSimpleHttp( SOCKET sock )
{
	uint16_t maxlen = DATA_BUF_SIZE;
	uint16_t len;
	uint8_t workingBuffer[100]; 
	uint8_t* msgBuffer = gHttpRxBuffer;
	uint8_t* outbuffer = gHttpTxBuffer;
	HTMLPageDef* htmlpagePtr;
	
	// Nachsehen, wieviele Daten angekommen sind
	len = getSn_RX_RSR(sock);
	if( len < 1 )
	return;
	
	// nicht mehr abholen, als Daten da sind
	if (len > maxlen)
	len = maxlen;
	
	// daten aus dem wiznet buffer holen
	len = recv(sock, msgBuffer, len);
	
	
	//Aufbau einer unterstützten Anfrage: 
	// GET /url HTTP/1.1 HOST: ip ....
	
	
	msgBuffer = strwrd( msgBuffer, workingBuffer, 100, " "); 
	if( msgBuffer )
	{
		printf( "[%s]\n", workingBuffer);
		if( !strcmp(workingBuffer, "GET") )
		{
			// Get Anfrage gefunden
			msgBuffer = strwrd( msgBuffer, workingBuffer, 100, " "); 
			if( msgBuffer)
			{
				printf( "[%s]\n", workingBuffer);
				if( workingBuffer[0] == '/' )
				{
					// eine URL gefudnen 
					htmlpagePtr = HTMLPagesGet_index();
					
					len = sprintf( outbuffer, 
						"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n %s", 
						strlen(htmlpagePtr->PageContent), 
						htmlpagePtr->PageContent
					);
					send( sock, outbuffer, len );	
				}
				
			}
			
		}
	}
	
	
	
}