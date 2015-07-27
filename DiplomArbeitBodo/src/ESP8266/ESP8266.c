/*
 * ESP8266.c
 *
 * Created: 15.07.2015 12:16:34
 *  Author: ech0
 */ 

#include "ESP8266.h"
#include "string.h"
#include "config/conf_defaults.h"
#include "Usart/myUsart.h"
#include "SysTimer/SysTimer.h"


ESPAsyncMsgBuffer gESPSendBuffer; 


void ESP_InitAsyncMsgBuffer( ESPAsyncMsgBuffer* asyncBuffer )
{
	memset( asyncBuffer->buffer, 0, DATA_BUF_SIZE_TX ); 
	asyncBuffer->bufferLen = 0; 
	asyncBuffer->nextWrite = 0;
	asyncBuffer->lastSentTime = 0; 
	asyncBuffer->sendToID = 0; 
}



int ESP_EncodeRestart( uint8_t* msg, uint32_t* msglen )
{
	*msglen = sprintf( msg, "%s%s", (uint8_t*)ESPCMD_REBOOT, ESPCMD_CMDEND );
	return 0;
}

int ESP_EncodeGetVersion( uint8_t* msg, uint32_t* msglen )
{
	*msglen = sprintf( msg, "%s%s", (uint8_t*)ESPCMD_FWVERSION, ESPCMD_CMDEND ); 		
	return 0; 
}


int ESP_EncodeSetMode( uint8_t* msg, uint32_t* msglen, int mode /* = 2 */  )
{
	*msglen = sprintf( msg, "%s%i%s", (uint8_t*)ESPCMD_MODE, mode, ESPCMD_CMDEND ); 	
	return 0;
}


int ESP_EncodeSetupAP( uint8_t* msg, uint32_t* msglen, uint8_t* ssid, uint8_t* password )
{
	int channel = 5; 
	
	// Wenn kein oder zu kurzes PW gesetzt ist gar kein PW verlangen 
	int passtype = passtype_wpa2_psk;
	if( strlen( password) < 8 )	
		passtype = passtype_open; 	
				
#ifdef ESP_DO_NOT_USE_PASSWORD			
			passtype = passtype_open;					
#endif /* ESP_DO_NOT_USE_PASSWORD */ 	
			
	*msglen = sprintf( msg, "%s\"%s\",\"%s\",%i,%i%s", (uint8_t*)ESPCMD_SOFTAP, ssid, password,channel,passtype, ESPCMD_CMDEND );
	return 0;
}

int ESP_EncodeSetIP( uint8_t* msg, uint32_t* msglen, uint32_t* ipaddress )
{
	*msglen = sprintf( msg, "%s\"%i.%i.%i.%i\"%s", (uint8_t*)ESPCMD_SETIP, ipaddress[0],ipaddress[1], ipaddress[2], ipaddress[3], ESPCMD_CMDEND );
	return 0;	
}

int ESP_EncodeSetMultiCon( uint8_t* msg, uint32_t* msglen, uint8_t enableMulti )
{
	*msglen = sprintf( msg, "%s%i%s", (uint8_t*)ESPCMD_MULTICONN, enableMulti, ESPCMD_CMDEND );
	return 0;	
}


int ESP_EncodeServerMode( uint8_t* msg, uint32_t* msglen, uint8_t enableServer, uint16_t port )
{
	*msglen = sprintf( msg, "%s%i,%i%s", (uint8_t*)ESPCMD_SETSERVER, enableServer, port, ESPCMD_CMDEND );
	return 0;
}








int ESP_SendCompleteMessage( uint8_t id, uint8_t* msg, uint32_t msglen ) 
{
	uint8_t tempbuf[20]; 
	uint32_t len; 
	uint32_t offset; 
	uint32_t blocklen; 
	
	len = sprintf( tempbuf, "%s%i,%i,", ESPCMD_SENDMESSAGE, id, msglen );
			
	USARTWifi_Write( tempbuf, len ); 
	USARTWifi_Write( "\r\n", 2); 
	Delay_ms( 20); 
	
	
	offset = 0; 
	blocklen = ( msglen - offset ) % 2048; 
	while( blocklen > 0 )
	{			
		USARTWifi_Write( &msg[offset], blocklen ); 
		USARTWifi_Write( "\r\n", 2); 
		Delay_ms( 20); 
		offset += blocklen; 
		blocklen = ( msglen - offset ) % 2048; 
	}

	ESP_EncodeClose( id, tempbuf, &len ); 
	USARTWifi_Write( msg, len ); 
	Delay_ms( 20); 

	USARTCons_Write( tempbuf, len ); 
	USARTCons_Write( msg, msglen ); 
	
	return 0;
}


int ESP_BeginSendMessage( uint8_t id, uint8_t* msg, uint32_t msglen )
{	
	ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer(); 
	
	ESP_InitAsyncMsgBuffer( asyncBuffer ); 
	
	memcpy( asyncBuffer->buffer, msg, msglen ); 
	asyncBuffer->bufferLen = msglen;
	asyncBuffer->sendToID = id; 
}

int ESP_EncodeSendHead( uint8_t* msg, uint32_t* msglen, ESPAsyncMsgBuffer* asyncBuffer )
{	
	// maximal 2048 byte in einem Durchlauf senden
	asyncBuffer->bytesThisRun = asyncBuffer->bufferLen - asyncBuffer->nextWrite; 
	asyncBuffer->bytesThisRun %= ESP_MAX_BLOCK_SIZE;	
	
	*msglen = sprintf( msg, "%s%i,%i%s", ESPCMD_SENDMESSAGE, asyncBuffer->sendToID, asyncBuffer->bytesThisRun, ESPCMD_CMDEND );
	return 0; 
}

int ESP_SendNext( ESPAsyncMsgBuffer* asyncBuffer )
{
	USARTWifi_Write( &asyncBuffer->buffer[asyncBuffer->nextWrite], asyncBuffer->bytesThisRun ); 
	USARTWifi_Write( "\r\n", 2 );
	
	asyncBuffer->nextWrite += asyncBuffer->bytesThisRun; 
	asyncBuffer->bytesThisRun = 0;
	asyncBuffer->lastSentTime = Get_systime_ms();
	return 0; 
}

int ESP_EncodeClose( uint8_t id,  uint8_t* msg, uint32_t* msglen )
{
	*msglen = sprintf( msg, "%s%i%s", (uint8_t*)ESPCMD_CLOSECONN, id, ESPCMD_CMDEND );
	return 0;
}



ESPAsyncMsgBuffer* ESP_GetAsyncBuffer( )
{
	return &gESPSendBuffer; 
}



// EOF 



