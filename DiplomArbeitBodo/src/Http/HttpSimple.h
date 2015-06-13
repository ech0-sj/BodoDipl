/*
 * HttpSimple.h
 *
 * Created: 05.06.2015 15:31:37
 *  Author: ech0
 */ 


#ifndef HTTPSIMPLE_H_
#define HTTPSIMPLE_H_

#include <asf.h>
#include "../Ethernet/Ethernet.h"


#define HTTP_HEAD "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\nContent-Length: %i \r\n\r\n\0"



#define DATA_BUF_SIZE 2048
#define DATA_BUF_SIZE_TX (4096)

#define HTML_MAX_DATA_PER_REQ 100

typedef enum 
{
	HTTP_GET, 
	HTTP_POST, 
	HTTP_unsupported,
}eHTTPType;


#define HTTP_PAGENAME_MAXLEN	30 
typedef struct
{
	uint8_t PageName[HTTP_PAGENAME_MAXLEN];		// z.B.: index.html , ... 
	 void (*HtmlPageDescription)( uint8_t* completeUrl_z, uint8_t* outPage_z );
}HtmlPageDescriptor;


typedef struct HTML_PAGE_STRUCT
{
	HtmlPageDescriptor page;
	struct HTML_PAGE_STRUCT* pNext;
}HtmlPagesList;


void HTTP_RegisterPage( HtmlPageDescriptor* description ); 
HtmlPageDescriptor* HTTP_FindPageByName( uint8_t* pageName_z);
HtmlPagesList* HTTP_GetPagesList();


void HTTP_DoSimpleHTTP( SOCKET sock );

eHTTPType HTTP_ParseType( uint8_t* httpMsg, uint32_t msglen ); 

int HTTP_ParseURL( uint8_t* httpMsg, uint32_t msglen, uint8_t* completeURL, uint32_t* urllen );

int HTTP_ProcessGETRequest( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_SendNotFound( SOCKET sock ); 


#endif /* HTTPSIMPLE_H_ */