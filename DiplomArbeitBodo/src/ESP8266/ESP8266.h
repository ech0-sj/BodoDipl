/*
 * ESP8266.h
 *
 * Created: 15.07.2015 12:16:25
 *  Author: ech0
 */ 


#ifndef ESP8266_H_
#define ESP8266_H_

// Das ESP - Protokoll wird hier gekapselt.

#include <asf.h>
#include "conf_defaults.h"


/* 
Grundlage dieser Befehle 
https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/#AT+GMR

*/ 


#define ESP_MAX_BLOCK_SIZE		2048
#define ESP_SEND_DELAY_TIME_MS	40

#define ESPCMD_CMDEND		"\r\n\0"
#define ESPCMD_REBOOT		"AT+RST\0"
#define ESPCMD_FWVERSION	"AT+GMR\0"
#define ESPCMD_MODE			"AT+CWMODE=\0"
#define ESPCMD_SOFTAP		"AT+CWSAP=\0" /* AT+CWSAP=ssid,pwd,channel,securitytype*/ 
#define ESPCMD_SETIP		"AT+CIPAP=\0" /* AT+CIPAP="192.168.5.1"*/
#define ESPCMD_MULTICONN	"AT+CIPMUX=\0"
#define ESPCMD_SETSERVER	"AT+CIPSERVER=\0" /* AT+CIPSERVER=mode[,port] */ 
#define ESPCMD_SENDMESSAGE	"AT+CIPSEND=\0"
#define ESPCMD_CLOSECONN	"AT+CIPCLOSE=\0"

typedef enum
{
	passtype_open,
	passtype_wep,
	passtype_wpa_psk,
	passtype_wpa2_psk,
	passtype_wpa_wpa2_psk, 
}eESPPasstype;


typedef struct  
{
	uint8_t buffer[DATA_BUF_SIZE_TX];
	uint32_t bufferLen; 	
	
	uint32_t bytesThisRun; 
	uint32_t nextWrite; 
		
	uint8_t sendToID; 
	uint8_t spare[3];
		
	uint64_t lastSentTime; 	
}ESPAsyncMsgBuffer;

void ESP_InitAsyncMsgBuffer( ESPAsyncMsgBuffer* asyncBuffer ); 


int ESP_EncodeRestart( uint8_t* msg, uint32_t* msglen );
int ESP_EncodeGetVersion( uint8_t* msg, uint32_t* msglen );
int ESP_EncodeSetMode( uint8_t* msg, uint32_t* msglen, int mode /* = 2 */  );
int ESP_EncodeSetupAP( uint8_t* msg, uint32_t* msglen, uint8_t* ssid, uint8_t* password );
int ESP_EncodeSetIP( uint8_t* msg, uint32_t* msglen, uint32_t* ipaddress );
int ESP_EncodeSetMultiCon( uint8_t* msg, uint32_t* msglen, uint8_t enableMulti );
int ESP_EncodeServerMode( uint8_t* msg, uint32_t* msglen, uint8_t enableServer, uint16_t port );


int ESP_SendCompleteMessage( uint8_t id, uint8_t* msg, uint32_t msglen );

int ESP_BeginSendMessage( uint8_t id, uint8_t* msg, uint32_t msglen );
int ESP_EncodeSendHead( uint8_t* msg, uint32_t* msglen, ESPAsyncMsgBuffer* asyncBuffer );
int ESP_SendNext( ESPAsyncMsgBuffer* asyncBuffer );
int ESP_EncodeClose( uint8_t id,  uint8_t* msg, uint32_t* msglen );


ESPAsyncMsgBuffer* ESP_GetAsyncBuffer( ); 
#endif /* ESP8266_H_ */