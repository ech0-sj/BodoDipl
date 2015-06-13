/*
 * HttpSimple.c
 *
 * Created: 05.06.2015 15:31:49
 *  Author: ech0
 */ 


#include "HttpSimple.h"

#include "../Tools/Tools.h"
#include "string.h"
#include "../HTTPserver/html_pages.h"

uint8_t gHttpRxBuffer[DATA_BUF_SIZE];
uint8_t gHttpTxBuffer[DATA_BUF_SIZE_TX];


static const uint8_t HTTP_HEAD_1[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length:";
static const uint8_t HTTP_HEAD_2[] = "\r\n\r\n";





void HTTP_DoSimpleHTTP( SOCKET sock )
{
	uint16_t maxlen = DATA_BUF_SIZE;
	uint16_t len;
	uint8_t* msgBuffer = gHttpRxBuffer;
	uint8_t* outbuffer = gHttpTxBuffer;
	
	// Nachsehen, wieviele Daten da sind, und ggf abholen.
	if( (len=Socket_GetRxCount(sock)) < 1 )
		return; 
		
	len = ( len > maxlen ) ? maxlen : len;	
	len = TCP_recv( sock, msgBuffer, len ); 
	
	memset( gHttpTxBuffer, 0, DATA_BUF_SIZE_TX );
	
	switch( HTTP_ParseType(msgBuffer, len) )
	{
		case HTTP_GET:
		{
			HTTP_ProcessGETRequest(sock, msgBuffer, len );
			
		}break; 
		
		default:
			// derzeit nicht unterstützt
			HTTP_SendNotFound( sock ); 
		break;
	}
	
}


eHTTPType HTTP_ParseType( uint8_t* httpMsg, uint32_t msglen )
{
	uint8_t typeString[100]; 
	
	httpMsg = strwrd( httpMsg, typeString, 100, " ");
	if( typeString == NULL )
		return HTTP_unsupported;
		
	if( strstr( typeString, "GET" ) == typeString )
		return HTTP_GET; 
		
	if( strstr( typeString, "POST" ) == typeString )
	 	return HTTP_POST;
		 
	return HTTP_unsupported;
}




int HTTP_ProcessGETRequest( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	HtmlPageDescriptor* foundPage = NULL;
	
	uint8_t url[200];
	uint32_t urllen = 200;
	
	uint8_t* outbuf = gHttpTxBuffer; 
	uint32_t outlen = 0; 
	uint32_t pagelen = 0; 
	
	memset( outbuf, ' ', 105 ); 
	
	if( HTTP_ParseURL(httpMsg, msglen, url, &urllen ) != 0 ) 
	{
		HTTP_SendNotFound( sock ); 
		return -1; 	
	}
	
	// Falls nichts angegeben ist, soll die 
	// Index.html gerufen werden 
	if( strcmp( url, "/") == 0 )
	{
		strcpy( url, "/index.html\0"); 
	}
	
	// Passende Seite suchen, und bearbeiten
	if( (foundPage = HTTP_FindPageByName( &url[1]) ) != NULL )
	{
		foundPage->HtmlPageDescription( url, &outbuf[105] ); 
		
		// HTTP Header mit Länge einbauen, dann senden 
		pagelen = strlen( &outbuf[105]); 
		sprintf( outbuf, "%s %i %s", HTTP_HEAD_1, pagelen, HTTP_HEAD_2 ); 
		TCP_send( sock, outbuf, 105 + pagelen ); 
	}	
	else 
	{
		HTTP_SendNotFound(sock); 
	}	
	return 0;
}


int HTTP_ParseURL( uint8_t* httpMsg, uint32_t msglen, uint8_t* completeURL, uint32_t* urllen )
{
	uint8_t* offset; 
	uint8_t* ptr; 
	ptr = httpMsg;	
	
	// "GET " entfernen 
	ptr = strwrd( ptr, completeURL, msglen, " ");
	if( completeURL == NULL )
		return -1; 

	// url string herausholen 
	ptr = strwrd( ptr, completeURL, msglen, " ");
	if( completeURL == NULL )
		return -1; 
	
	printf( "URL [%s] \n", completeURL );
	*urllen = strlen( completeURL); 
	return 0;
}
	
		
void HTTP_SendNotFound( SOCKET sock)
{
	strcpy( gHttpTxBuffer, 
		"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nError 404 - page not found\r\n</BODY>\r\n</HTML>\r\n\0" ); 

	TCP_send( sock, gHttpTxBuffer, strlen(gHttpTxBuffer) );
}

 