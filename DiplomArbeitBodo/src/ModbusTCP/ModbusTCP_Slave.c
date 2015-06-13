/*
 * ModbusTCP_Slave.c
 *
 * Created: 05.06.2015 18:02:05
 *  Author: ech0
 */ 


/* Protokoll aufbau
	
			-> HEADER
		[0]	trans ID 
		[1]		
		[2]	immer 0 
		[3]
		[4]	
		[5]	len ab [6]
		[6]	Adresse
	
			-> MESSAGE	
		[7]	Funktion
		[8] Daten
		[n] 
			
*/
	


#include "ModbusTCP_slave.h"
#include "DataAccess/DataAccess.h"
#include "Persistent/PersistentItems.h"
#include "string.h"

uint8_t gModTCPRxBuffer[MODBUS_BUF_SIZE];
uint8_t gModTCPTxBuffer[MODBUS_BUF_SIZE];


int Modbus_OperateRequest( SOCKET sock, uint8_t* request, uint32_t len );

uint8_t _isModbusProt( uint8_t* request );

int Modbus_OperationReadRegister( uint8_t* request, uint32_t len, uint8_t* response, uint8_t* bytelen );

uint16_t Modbus_ReadjustRegister( uint16_t regid ); 


void Modbus_DoSlaveTCP( SOCKET sock )
{
	uint16_t len;
	uint16_t maxlen = MODBUS_BUF_SIZE;
	uint8_t* inbuf = gModTCPRxBuffer; 
	 
	 
	// Nachsehen, wieviele Daten da sind, und ggf abholen.
	len = Socket_GetRxCount(sock);
	if( len < 1 )
	return;
	
	// nicht mehr abholen, als Daten da sind
	if (len > maxlen)
	len = maxlen;
	
	// daten aus dem wiznet buffer holen
	len = TCP_recv(sock, inbuf, len);

	Modbus_OperateRequest( sock, inbuf, len ); 
}


int Modbus_OperateRequest( SOCKET sock, uint8_t* request, uint32_t len )
{
	uint8_t bytelen = 0; 
	uint16_t framelen = 0; 
	uint8_t* response = gModTCPTxBuffer;
	uint16_t responselen = 0;
	
	uint16_t transID; 
	uint8_t reqlen; 
	uint8_t address; 
	uint8_t function;
	uint8_t excepCode;
	uint8_t ownID; 
	
	if( _isModbusProt(request) == false )
	return -1; 	
	
	// Header decodieren 
	memset( response, 0, 20 );
	memcpy( &transID, request, 2 ); 
	reqlen = request[5];
	address = request[6];
	function = request[7];
	
	// Adressen vergleichen
	// sie muss passen
	LoadModbusID( &ownID ); 
	if( address == ownID )
	{
		// Modbus Operation durchführen 
		switch( function )
		{
			case Mod_Read_Register:
			case Mod_Read_Input:
			{	
				reqlen -= 2;
				bytelen = 0; 
				excepCode = Modbus_OperationReadRegister( &request[8], reqlen, &response[9], &bytelen);
				framelen = bytelen + 3; 
				responselen = framelen + 6;
			}break; 
		
			default:				// nicht unterstützer Fehlercode
				excepCode = 1;
			break;
		}
	}
	else
	{
		excepCode = Mod_Excep_InvAddr;
	}
	
	// Antwort generieren
	memcpy( response, &transID, 2 );
	memset( &response[2], 0, 3 );
	response[6] = address;

	// mit Daten ODER Fehler antworten 
	if( excepCode == Mod_Excep_NoError )
	{
		response[5] = framelen;				// func und address einbeziehen
		response[7] = function;
		response[8] = bytelen;				// Bytelänge der Nutzdaten
	}
	else 
	{
		response[5] = 3;					// func + Errorcode
		response[7] = function | MODBUS_ERROR_FUNC_MASK;
		response[8] = excepCode; 
		responselen = 9;
	}
	TCP_send( sock, response, responselen );
}


uint8_t _isModbusProt( uint8_t* request )
{
	if( (request[2] == 0 )
	&& (request[3] == 0 )
	&& (request[4] == 0 ) )
		return true; 
		
	return false; 
}



int Modbus_OperationReadRegister( uint8_t* request, uint32_t len, uint8_t* response, uint8_t* bytelen )
{
	uint16_t regid; 
	uint16_t count; 
	uint16_t i; 
	uint16_t value; 

	if( len < 4 )
		return -1; 
	
	// Register ID und Anzahl decodieren 
	// RegisterId normieren 	
	regid = (request[0] << 8 ) | request[1]; 
	count = (request[2] << 8 ) | request[3]; 
	regid = Modbus_ReadjustRegister( regid ); 
	
	// Alle angefragten Register auslesen und 
	// zurückliefern 
	for( i = 0; i < count; i++, regid++ )
	{
		if( DataAccess_GetElementAt( regid, &value ) )
		{
			response[i*2+1]	= (value & 0xFF);
			response[i*2]	= (value & 0xFF00) >> 8;
			*bytelen += 2; 
		}
		else
		{
			return Mod_Excep_InvAddr; 
		}
	}
	return Mod_Excep_NoError; 
}



uint16_t Modbus_ReadjustRegister( uint16_t regid )
{
	return (regid % 10000);
}