/*
 * simpleTelnet.c
 *
 * Created: 03.06.2015 18:44:12
 *  Author: ech0
 */ 


#include "Proc_Wiznet.h"
#include "string.h"
#include "../DualportRAM/Dualportram_Storage.h"
#include "../Tools/Tools.h"


void DoSimpleTelnet( SOCKET sock )
{
	uint16_t maxlen = 100; 
	uint16_t len; 
	uint8_t msgBuffer[100];
	uint8_t outbuffer[100]; 
	
	// Nachsehen, wieviele Daten angekommen sind
	len = getSn_RX_RSR(sock); 
	if( len < 1 )
		return; 
		
	// nicht mehr abholen, als Daten da sind 
	if (len > maxlen) 
		len = maxlen;
	
	// daten aus dem wiznet buffer holen 
	len = recv(sock, msgBuffer, len);
	printf( "telnet recv sock %i, len %i \n", sock, len ); 
	
	
	if( strstr( msgBuffer, "get ") )
	{
		char* ptr; 
		char delim[] = ",\n\r";
		uint16_t item = 0;
		uint16_t uIndex = 0; 
		int index = 0;
		
		ptr = &msgBuffer[4];
		memset( outbuffer, 0, 100 ); 
		ptr = strwrd( ptr, outbuffer, len, delim );
		while( ptr != NULL )
		{
			index = atoi( outbuffer ); 
			if( DualPortRAM_GetItemAt( index, &item ) ) 
			{
				len = sprintf( outbuffer, "%i = %i\n", index, item  );	
			}
			else
			{
				len = sprintf( outbuffer, "0\n" );
			}
			
			send( sock, outbuffer, len );
			
			printf( "%s", outbuffer );
			
			memset( outbuffer, 0, 100 );
			ptr = strwrd( ptr, outbuffer, len, delim );
		}
		 
	}
	
}