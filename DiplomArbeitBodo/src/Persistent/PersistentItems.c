/*
 * PersistentItems.c
 *
 * Created: 03.06.2015 17:53:46
 *  Author: ech0
 */ 

#include "PersistentItems.h"
#include "string.h"

const uint8_t DEFAULT_IP[] = { 192, 168, 1, 112 };
const uint8_t DEFAULT_SUB[] = { 255, 255, 255, 0 };
const uint8_t DEFAULT_MAC[] = {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef};
	
uint8_t gPersist_IP[4] = {0}; 
uint8_t gPersist_SUB[4] = {0};
uint8_t gPersist_MAC[6] = {0};
	

PersisentResult LoadIPWiznet( uint8_t* ip )
{
	if( gPersist_IP[0] == 0 )
		memcpy( gPersist_IP, DEFAULT_IP, 4 );
		
	memcpy( ip, gPersist_IP, 4 );	 
	return PersRes_Successful; 
}

PersisentResult SaveIPWiznet( uint8_t* ip )
{
	memcpy( gPersist_IP, ip, 4 );
	return PersRes_Successful;
}




PersisentResult LoadMACWiznet( uint8_t* mac )
{
	if( gPersist_MAC[0] == 0 )
		memcpy( gPersist_MAC, DEFAULT_MAC, 6 ); 
		
	memcpy( mac, gPersist_MAC, 6 );
	return PersRes_Successful;
}

PersisentResult SaveMACWiznet( uint8_t* mac )
{
	memcpy( gPersist_MAC, mac, 6 );
	return PersRes_Successful;
}




PersisentResult LoadSubnetWiznet( uint8_t* subnet )
{
	if( gPersist_SUB[0] == 0 )
		memcpy( gPersist_SUB, DEFAULT_SUB, 4 ); 
	
	memcpy( subnet, gPersist_SUB, 6 );
	return PersRes_Successful;
}

PersisentResult SaveSubnetWiznet( uint8_t* subnet )
{
	memcpy( gPersist_SUB, subnet, 6 );
	return PersRes_Successful;
}
