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

typedef enum 
{
	HTTP_GET, 
	HTTP_POST, 
	HTTP_unsupported,
}eHTTPType;




void HTTP_DoSimpleHTTP( SOCKET sock );

eHTTPType HTTP_ParseType( uint8_t* httpMsg, uint32_t msglen ); 

int HTTP_ParseURL( uint8_t* httpMsg, uint32_t msglen, uint8_t* completeURL, uint32_t* urllen );

int HTTP_ProcessGETRequest( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_SendNotFound( SOCKET sock ); 



void HTTP_DoPage_IndexHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_DoPage_InfoHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_DoPage_DataHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_DoPage_SettingHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

void HTTP_DoPage_TakesetupHTML( SOCKET sock, uint8_t* httpMsg, uint32_t msglen );

#endif /* HTTPSIMPLE_H_ */