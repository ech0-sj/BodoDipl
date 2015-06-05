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
uint8_t gHttpTxBuffer[DATA_BUF_SIZE];






void DoSimpleHttp( SOCKET sock )
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
	
	
	
	//Aufbau einer unterstützten Anfrage:
	// GET /url HTTP/1.1 HOST: ip ....
	
/*	
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
					TCP_send( sock, outbuffer, len );
				}
				
			}
			
		}
	}
	
	
	*/
	
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
	uint8_t url[200];
	uint32_t urllen = 200;
	
	uint32_t pagelen = 0; 
	
	if( HTTP_ParseURL(httpMsg, msglen, url, &urllen ) != 0 ) 
	{
		HTTP_SendNotFound( sock ); 
		return -1; 	
	}
	
	
	// URL string nach Seiten durchsuchen und 
	// die entsprechende Seite aufrufen 
	
	
	if( ( strcmp( url, "/") == 0 )
	||	( strstr( url, "/index.html") != NULL ) )
	{
		// index. html gewählt 
		pagelen = strlen( "/index.html");
		HTTP_DoPage_IndexHTML( sock, &url[pagelen], urllen - pagelen ); 	
	}
	else if( strstr( url, "/info.html") != NULL )
	{
		// index. html gewählt
		pagelen = strlen( "/info.html");
		HTTP_DoPage_InfoHTML( sock, &url[pagelen], urllen - pagelen );
	}
	else if( strstr( url, "/setting.html") != NULL )
	{
		// index. html gewählt
		pagelen = strlen( "/setting.html");
		HTTP_DoPage_SettingHTML( sock, &url[pagelen], urllen - pagelen );
	}
	else if( strstr( url, "/data.html") != NULL )
	{
		// index. html gewählt
		pagelen = strlen( "/data.html");
		HTTP_DoPage_DataHTML( sock, &url[pagelen], urllen - pagelen );
	}
	else if( strstr( url, "/takeSetup.html") != NULL )
	{
		// index. html gewählt
		pagelen = strlen( "/takeSetup.html");
		HTTP_DoPage_TakesetupHTML( sock, &url[pagelen], urllen - pagelen );
	}
	else
	{
		HTTP_SendNotFound( sock ); 
	}
	
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




#include "../HTTPserver/html_pages.h"





void HTTP_DoPage_IndexHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	uint32_t sendlen; 
	HTMLPageDef* html;
	uint8_t* pagePtr; 
	
	HTMLPagesCreate_index();
	html = HTMLPagesGet_index();
	sendlen = sprintf( gHttpTxBuffer,
					"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n %s",
					strlen(html->PageContent),
					html->PageContent
					);
	
	pagePtr = &gHttpTxBuffer;
	TCP_send( sock, gHttpTxBuffer, sendlen ); 
}


void HTTP_DoPage_DataHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	uint32_t sendlen;
	HTMLPageDef* html;
	uint8_t* pagePtr;
	
	HTMLPagesCreate_data();
	html = HTMLPagesGet_data();
	sendlen = sprintf( gHttpTxBuffer,
	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n %s",
	strlen(html->PageContent),
	html->PageContent
	);
	
	pagePtr = &gHttpTxBuffer;
	TCP_send( sock, gHttpTxBuffer, sendlen );
}



void HTTP_DoPage_InfoHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	uint32_t sendlen;
	HTMLPageDef* html;
	uint8_t* pagePtr;
	
	HTMLPagesCreate_index();
	html = HTMLPagesGet_index();
	sendlen = sprintf( gHttpTxBuffer,
	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n %s",
	strlen(html->PageContent),
	html->PageContent
	);
	
	pagePtr = &gHttpTxBuffer;
	TCP_send( sock, gHttpTxBuffer, sendlen );
}


void HTTP_DoPage_SettingHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	uint32_t sendlen;
	HTMLPageDef* html;
	uint8_t* pagePtr;
	
	HTMLPagesCreate_setting();
	html = HTMLPagesGet_setting();
	sendlen = sprintf( gHttpTxBuffer,
	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n %s",
	strlen(html->PageContent),
	html->PageContent
	);
	
	pagePtr = &gHttpTxBuffer;
	TCP_send( sock, gHttpTxBuffer, sendlen );
}



void HTTP_DoPage_TakesetupHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen )
{
	uint32_t sendlen;
	HTMLPageDef* html;
	uint8_t* pagePtr;
	
	sendlen = sprintf( gHttpTxBuffer,
	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: 79 \r\n\r\n"
	"<HTML>\r\n<BODY>\r\nDie Aenderungen uebernommen\r\n</BODY>\r\n</HTML>\r\n\0"
	);
	
	pagePtr = &gHttpTxBuffer;
	TCP_send( sock, gHttpTxBuffer, sendlen );
}