/*
 * HTML_DataPage.c
 *
 * Created: 08.06.2015 20:25:15
 *  Author: ech0
 */ 


#include "HtmlPages.h"
#include "string.h"
#include "Tools/Tools.h"
#include "DataAccess/DataAccess.h"
#include "HtmlTools.h"



int Extract_FirstLast( Stringlist* paramlist, int16_t* first, int16_t* last );
uint32_t AppendDataNotFound( uint8_t* outPage );
uint32_t AppendDataItems( uint8_t* outPage, int16_t* first, int16_t* last );

HtmlPageDescriptor HTML_CreatePage_Data(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "data.html" );
	descr.HtmlPageDescription = HTML_DataPage;
	
	return descr;
}

void HTML_DataPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	len = HTML_GetHead( outPage_z, "Bodo Janssen - Daten" );
	
	len += sprintf( &outPage_z[len],
	
	"<form action=\"datarange.html\" method=\"GET\"> "
		"Select range to show (max 100 items per request)<p>"
		"<table>"
			"<colgroup><col width=\"*\"><col width=\"*\"><col width=\"50\"><col width=\"*\"><col width=\"*\"><col width=\"50\"><col width=\"*\"> </colgroup>"
			"<tr>"
				"<td><label>from: </label></td>"
				"<td><input type=\"number\" name=\"first\"></td>"
				"<td><label>to: </label></td>"
				"<td><input type=\"number\" name=\"last\"></td>"
				"<td><button type=\"submit\">submit</button></td>"
			"</tr>"
		"</table>"
	"</form>"
	);
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0; 
}




HtmlPageDescriptor HTML_CreatePage_DataRange(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "datarange.html" );
	descr.HtmlPageDescription = HTML_DataRangePage;
	
	return descr;
}

void HTML_DataRangePage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	
	uint8_t paramsFound = false; 
	int16_t value = 0;
	int16_t first = 1;
	int16_t last = 1;

	Stringlist* paramlist = Html_ExtractParametersFromUrl( completeUrl_z ); 
	printf( "data page params ");
	Stringlist_Print(paramlist ); 
	printf( "\n");
	
	if( !Extract_FirstLast( paramlist, &first, &last ) )
	{
		if( first <= last )	
			paramsFound = true;  
	}
	Stringlist_Delete( paramlist );
	
	
	len = HTML_GetHead( outPage_z, "Bodo Janssen - Daten Anzeigen" );
	
	if( paramsFound )
	{
		len += AppendDataItems( &outPage_z[len], first, last ); 
	}
	else 
	{
		len += AppendDataNotFound( &outPage_z[len] );
	}
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0;
}


uint32_t AppendDataNotFound( uint8_t* outPage )
{
	return sprintf(  outPage, "<p>Sorry, items not found");
}


uint32_t AppendDataItems( uint8_t* outPage, int16_t* first, int16_t* last )
{
	int count; 
	int16_t i; 
	uint32_t len = 0; 
	int16_t value; 
	
	len += sprintf( &outPage[len], "<ul>");
	
	for( i = first, count = 0; (i <= last) && (count < HTML_MAX_DATA_PER_REQ); i++, count++ )
	{
		if( DataAccess_GetElementAt( i, &value ) )
			len += sprintf( &outPage[len], "<li> %i: %i</li>", i, value );
		else 
			len += sprintf( &outPage[len], "<li> %i: ---</li>", i );
	}
	len += sprintf( &outPage[len], "</ul>");
	
	return len; 
}



int Extract_FirstLast( Stringlist* paramlist, int16_t* first, int16_t* last )
{
	uint8_t* pos; 
	uint8_t itemsToGo = 2;
	
	while( paramlist != NULL )
	{
		if( strstr(paramlist->string, "first") != NULL ) 
		{
			pos = Html_GetParamValueStartPtr( paramlist->string ); 
			if( pos )
			{
				*first = atoi( pos); 
				itemsToGo -= 1; 
			}
		}
		if( strstr(paramlist->string, "last") != NULL )
		{
			pos = Html_GetParamValueStartPtr( paramlist->string );
			if( pos )
			{
				*last = atoi( pos);
				itemsToGo -= 1;
			}
		}
		paramlist = paramlist->pNext;
	}
	return itemsToGo; 
}
