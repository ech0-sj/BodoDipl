/*
 * HtmlTools.h
 *
 * Created: 13.06.2015 13:07:11
 *  Author: ech0
 */ 


#ifndef HTMLTOOLS_H_
#define HTMLTOOLS_H_

#include "asf.h"


typedef struct STRING_LIST
{
	uint16_t strlen; 
	uint8_t* string; 
	
	struct STRING_LIST* pNext; 
}Stringlist;

Stringlist* Html_ExtractParametersFromUrl( uint8_t* completeUrl );
uint8_t* Html_GetParamValueStartPtr( uint8_t* paramString );


void Stringlist_Delete( Stringlist* start );
Stringlist* Stringlist_SeekEnd( Stringlist* act );
uint8_t Stringlist_IsLast( Stringlist* act );
Stringlist* Stringlist_NextItem( Stringlist* act );
Stringlist* Stringlist_CreateItem( );
void Stringlist_Print( Stringlist* list );

#endif /* HTMLTOOLS_H_ */