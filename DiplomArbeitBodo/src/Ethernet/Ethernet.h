/*
 * Ethernet.h
 *
 * Created: 05.06.2015 15:47:01
 *  Author: ech0
 */ 


#ifndef ETHERNET_H_
#define ETHERNET_H_


#include <asf.h>
#include "../Wiznet/socket.h"

/*
	HAL - Schicht für Ethernet Geräte, identifikation kann über SOCKET nummer erfolgen
	so können wiznet + ESP nach aussen gleich verwendet werden. 
*/ 


#ifndef SOCKET
typedef uint8_t SOCKET;
#endif



uint16_t Socket_GetRxCount(SOCKET sn);

int32_t TCP_send(SOCKET sn, uint8_t * buf, uint16_t len);

int32_t TCP_recv(SOCKET sn, uint8_t * buf, uint16_t len);

int32_t UDP_sendto(SOCKET sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port);

int32_t UDP_recvfrom(SOCKET sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port);


#endif /* ETHERNET_H_ */