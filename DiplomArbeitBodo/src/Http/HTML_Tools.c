/*
 * HTML_Tools.c
 *
 * Created: 13.06.2015 12:54:36
 *  Author: ech0
 */ 

#include "asf.h"
#include "HtmlTools.h"
#include "Tools/Tools.h"
#include "string.h"



// format: 
// index.html?par1=abc&para2=def  

Stringlist* Html_ExtractParametersFromUrl( uint8_t* completeUrl )
{
	uint8_t* ptr = completeUrl; 
	uint8_t foundString[100];
	
	Stringlist* paramList = NULL; 
	Stringlist* actList = NULL;
	
	// 1. Adres teil 
	ptr = strwrd( ptr, foundString, 100, "?" );
	if( !ptr )
		return NULL;
	
	// Falls der Ptr noch auf dem ? steht
	// eine Pos weiter schieben
	if( ptr[0] == '?')
		ptr++; 
	
	// alle Einträge, getrennt durch & finden 
	// und in eine Liste eintragen
	while( ptr != NULL ) 
	{
		memset( foundString, 0, 100 ); 
		ptr = strwrd( ptr, foundString, 100, "&" );
		
		if( foundString[0] == 0 )
			break; 
		
		// beim 1. Durchlauf das allererste element anlegen
		// bei jedem weiteren den Next Ptr verwenden
		if( !paramList )
		{
			paramList = Stringlist_CreateItem();
			actList = paramList;
		}
		else
		{
			actList->pNext = Stringlist_CreateItem();
			actList = actList->pNext;
		}
		
		
		
		actList->strlen = strlen( foundString );
		actList->string = malloc( actList->strlen );
		strncpy( actList->string, foundString, actList->strlen+1 );	
	}
	
	return paramList;
}



uint8_t* Html_GetParamValueStartPtr( uint8_t* paramString )
{
	// von rechts nach dem trenner '=' suchen
	// und die nächstePos zurückgeben.
	// kann auch nulll sein 
	uint8_t* foundpos = strrchr( paramString, '=');
	foundpos++;
	return foundpos;
}















Stringlist* Stringlist_CreateItem( )
{
	Stringlist* entry = (Stringlist*)malloc( sizeof(Stringlist) );
	if( !entry )
	return  NULL;
	
	entry->pNext = NULL;
	return entry;
}

Stringlist* Stringlist_NextItem( Stringlist* act )
{
	return act->pNext;
}

uint8_t Stringlist_IsLast( Stringlist* act )
{
	if( act->pNext == NULL )
	return true;
	
	return false;
}


Stringlist* Stringlist_SeekEnd( Stringlist* act )
{
	while( !Stringlist_IsLast(act))
	act = Stringlist_NextItem(act);
	
	return act;
}


void Stringlist_Delete( Stringlist* start )
{
	Stringlist* act; 
	if( !start )
		return; 
		
	while( start != NULL )
	{
		act = start; 
		start = start->pNext;
		
		free( act->string ); 
		act->strlen = 0;
		free( act ); 
		act = NULL; 
	}
}

void Stringlist_Print( Stringlist* list )
{
	while( list != NULL )
	{
		printf( "[%s] ", list->string );
		list = Stringlist_NextItem(list); 	
	}
}