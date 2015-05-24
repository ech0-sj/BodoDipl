/*
 * Tools.c
 *
 * Created: 24.05.2015 18:37:33
 *  Author: ech0
 */ 
#include "Tools.h"

#include "string.h"


uint32_t ParseIpToArray( char* ipAstring, uint8_t** ipArray )
{
	int i;
	int oktettAsInt;
	char* ptr = NULL;
	
	ptr = strtok( ipAstring, ".");
	for( i = 0; (i < 4) && (ptr != NULL); i++ )
	{
		oktettAsInt = atoi( ptr );
		ipArray[i] = (uint8_t)oktettAsInt & 0xFF;
		ptr = strtok(NULL, ".");
	}
	if( i > 3 )
	{
		return 0; 
	}
	else
	{
		return -1;
	}
	
}
