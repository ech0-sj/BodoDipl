/*
 * html_pages.c
 *
 * Created: 25.05.2015 00:09:20
 *  Author: ech0
 */ 

#include "html_pages.h"
#include "string.h"
#include "malloc.h"
#include "../DualportRAM/Dualportram_Storage.h"


uint32_t HTML_GetHead( char* pageDef, char* title )
{
	uint32_t len = 
	sprintf( pageDef, 
		"<html>"
			"<head>"
				"<style type=\"text/css\">"
					"body { background-color:#E0E0E0; font-weight:bold; font-family:Arial; font-size:16px; }"
				"</style>"
				"<title>%s</title>"
			"</head>"
			"<body>"
			
	, title );
	
	
	return len; 
}



uint32_t HTML_GetFoot( char* pageDef )
{
	uint32_t len = 
	sprintf( pageDef, 
			"</body>"
		"</html>"
		);	
	return len; 
}




HTMLPageDef gIndexHTML;

HTMLPageDef* HTMLPagesGet_index()
{
	return &gIndexHTML;
}

void HTMLPagesCreate_index()
{
	gIndexHTML.PageName = "index.html";
	HTMLPagesUpdate_index();
}

void HTMLPagesUpdate_index()
{
	uint32_t len; 
	len = HTML_GetHead( gIndexHTML.PageContent, "Bodo Janssen - Willkommen" );
	
	len += sprintf( &gIndexHTML.PageContent[len], 
		"Willkommen auf der Webseite <p>"
	);
	
	HTML_GetFoot( &gIndexHTML.PageContent[len] );
}







HTMLPageDef gDataHTML;

HTMLPageDef* HTMLPagesGet_data( void )
{
	return &gDataHTML;	
}

void HTMLPagesCreate_data( void )
{
	gDataHTML.PageName = "data.html";
	HTMLPagesUpdate_data();
	
}

void HTMLPagesUpdate_data( void )
{
	uint32_t len;
	int16_t itemVal; 
	uint8_t err[] = "---\0";
	
	len = HTML_GetHead( gDataHTML.PageContent, "Bodo Janssen - Daten" );
	
	
	len += sprintf( &gDataHTML.PageContent[len], 
			"An dieser Stelle finden Sie die live Daten <p>"
			"<ul>" );
			
	for( int i = 0; i < 10; i++ )
	{
		if( DualPortRAM_GetItemAt(i, itemVal ) )
			len += sprintf(&gDataHTML.PageContent[len], "<li>%i</li> \r\n", itemVal );		
		else
			len += sprintf(&gDataHTML.PageContent[len], "<li>%s</li> \r\n", err );
	}
	len += sprintf( &gDataHTML.PageContent[len], "<ul> \r\n" ); 
				
	HTML_GetFoot( &gDataHTML.PageContent[len] );
}




HTMLPageDef gSettingHTML;

HTMLPageDef* HTMLPagesGet_setting( void )
{
	return &gSettingHTML;
}

void HTMLPagesCreate_setting( void )
{
	gSettingHTML.PageName = "setting.html";
	HTMLPagesUpdate_setting();
	
}

void HTMLPagesUpdate_setting( void )
{
	uint32_t len;
	
	len = HTML_GetHead( gSettingHTML.PageContent, "Bodo Janssen - Einstellungen" );
	
	
	len += sprintf( &gSettingHTML.PageContent[len],
	
		 "<form action=\"takeSetup.html\" method=GET >"
		 "<label for=\"ip1\">IP Address</label>"
		 "<input type=\"value\" name=\"ip1\" maxlength=\"3\" max=\"254\">"
		 "<input type=\"value\" name=\"ip2\" maxlength=\"3\" max=\"254\">"
		 "<input type=\"value\" name=\"ip3\" maxlength=\"3\" max=\"254\">"
		 "<input type=\"value\" name=\"ip4\" maxlength=\"3\" max=\"254\"><br>"
		 "<input type=\"value\" name=\"port\" maxlength=\"3\" max=\"65000\"><br>"
		 "<button type=\"submit\">send</button>"
		 "</form>"
	
	 );
	
	
	HTML_GetFoot( &gSettingHTML.PageContent[len] );
}





