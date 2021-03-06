/*
 * Ethernet.c
 *
 * Created: 05.06.2015 15:56:16
 *  Author: ech0
 */ 


#include "Ethernet.h"
#include "ESP8266/ESP8266.h"

uint16_t Socket_GetRxCount(SOCKET sn)
{
	return getSn_RX_RSR( sn);
}

int32_t TCP_send(SOCKET sn, uint8_t * buf, uint16_t len)
{
	if( sn < 8 )
		return send( sn, buf, len );
	
	return ESP_SendCompleteMessage( sn-8, buf, len ); 	
	// return ESP_BeginSendMessage( sn-8, buf, len ); 
	// return 0; 
}

int32_t TCP_recv(SOCKET sn, uint8_t * buf, uint16_t len)
{
	return recv( sn, buf, len );
}

int32_t UDP_sendto(SOCKET sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port)
{
	return sendto( sn, buf, len, addr, port );
}

int32_t UDP_recvfrom(SOCKET sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port)
{
	return recvfrom( sn, buf, len, addr, port );
}

