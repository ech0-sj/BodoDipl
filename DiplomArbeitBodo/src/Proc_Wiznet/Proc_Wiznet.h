/*
 * Proc_Wiznet.h
 *
 * Created: 24.05.2015 19:08:42
 *  Author: ech0
 */ 


#ifndef PROC_WIZNET_H_
#define PROC_WIZNET_H_

#include "asf.h"
#include "../Schedule/Scheduler.h"
#include "../Wiznet/wizchip_conf.h"
#include "../Wiznet/socket.h"


#define NUM_OF_WIZNET_SOCKETS	_WIZCHIP_SOCK_NUM_
#define NUM_OF_WIZ_TCP_SOCKETS	(NUM_OF_WIZNET_SOCKETS -1)


#define WIZNET_HTTP_SOCKET_COUNT	3				// die ersten 3 Sockets sollen für HTTP festgelegt werden
#define WIZNET_HTTP_PORT			80				// Port für HTTP 
#define WIZNET_MODBUS_PORT			502				// Port für Modbus TCP
#define WIZNET_CUSTOM_PORT			1234			// Port für eigene Protokolle 

#define DATA_BUF_SIZE				2048			// Buffer size 


typedef struct  
{
	uint8_t buffer[DATA_BUF_SIZE];
	uint32_t nextWrite;
		
}WiznetReceiveBuffer;


void ProcWiznet( ProcessStruct* procStruct );
void ProcWiznet_Init( ProcessStruct* procStruct );
void ProcWiznet_InitRecvBuffer( WiznetReceiveBuffer* recvBuffer );

void DoSimpleTelnet( SOCKET sock );
void HTTP_DoSimpleHTTP( SOCKET sock ); 

#endif /* PROC_WIZNET_H_ */