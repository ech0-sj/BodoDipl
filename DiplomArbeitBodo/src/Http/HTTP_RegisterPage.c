/*
 * HTTP_RegisterPage.c
 *
 * Created: 06.06.2015 13:31:36
 *  Author: ech0
 */ 

#include "HttpSimple.h"
#include "malloc.h"
#include "string.h"

HtmlPagesList* gHTMLPages = NULL;



HtmlPagesList* List_CreateItem( );
HtmlPagesList* List_NextItem( HtmlPagesList* act );
uint8_t List_IsLast( HtmlPagesList* act );
HtmlPagesList* List_SeekEnd( HtmlPagesList* act );



HtmlPagesList* HTTP_GetPagesList()
{
	return gHTMLPages;
}

void HTTP_RegisterPage( HtmlPageDescriptor* description )
{
	HtmlPagesList* actitem;
	
	// Liste beim erten aufruf erzeugen 
	if( gHTMLPages == NULL )
	{
		gHTMLPages = List_CreateItem();
		actitem = HTTP_GetPagesList();
	}
	else
	{
		// Einen neuen Eintrag am Ende erzeugen
		actitem = HTTP_GetPagesList();
		actitem = List_SeekEnd(actitem );
		actitem->pNext = List_CreateItem();
		actitem = List_NextItem(actitem);
	}
	
	// Daten einkopieren 
	memcpy( actitem->page.PageName, description->PageName, HTTP_PAGENAME_MAXLEN ); 
	actitem->page.HtmlPageDescription = description->HtmlPageDescription;
}


HtmlPageDescriptor* HTTP_FindPageByName( uint8_t* pageName_z)
{
	HtmlPagesList* pages = HTTP_GetPagesList();
	
	while( pages != NULL )
	{
		if( !strncmp( pageName_z, pages->page.PageName, strlen(pages->page.PageName) ) )
		{
			return &pages->page;
		}
		pages = List_NextItem(pages);
	}
	
	return NULL; 
}




HtmlPagesList* List_CreateItem( )
{
	HtmlPagesList* entry = (HtmlPagesList*)malloc( sizeof(HtmlPagesList) );	
	if( !entry )
		return  NULL; 
	
	entry->pNext = NULL;
	return entry;
}

HtmlPagesList* List_NextItem( HtmlPagesList* act )
{
	return act->pNext;
}

uint8_t List_IsLast( HtmlPagesList* act )
{
	if( act->pNext == NULL )
		return true; 
	
	return false; 
}

HtmlPagesList* List_SeekEnd( HtmlPagesList* act )
{
	while( !List_IsLast(act))
		act = List_NextItem(act);
	
	return act; 
}
