/*
 * HtmlPages.c
 *
 * Created: 06.06.2015 14:34:26
 *  Author: ech0
 */ 

#include "HtmlPages.h"
#include "string.h"
#include "Tools/Tools.h"
#include "DataAccess/DataAccess.h"



HtmlPageDescriptor HTML_CreatePage_Index(  )
{
	HtmlPageDescriptor descr; 
	strcpy( descr.PageName, "index.html" ); 
	descr.HtmlPageDescription = HTML_IndexPage;
	
	return descr;
}

void HTML_IndexPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	len = HTML_GetHead( outPage_z, "Bodo Janssen - Willkommen" );
	
	len += sprintf( &outPage_z[len],
	"Willkommen auf der Webseite <p>"
	);
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0;
}





HtmlPageDescriptor HTML_CreatePage_Info(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "info.html" );
	descr.HtmlPageDescription = HTML_InfoPage;
	
	return descr;
}

void HTML_InfoPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	len = HTML_GetHead( outPage_z, "Bodo Janssen - Informationen" );
	
	len += sprintf( &outPage_z[len],
	"An dieser Stelle werden Sie Informationen zum Produkt finden <p>"
	);
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0;
}








uint32_t HTML_GetHead( char* pageDef, char* title )
{
	uint32_t len =
	sprintf( pageDef,
"<html>"
	"<head>"
		"<style type=\"text/css\">"
			"body { background-color:#E0E0E0; font-weight:bold; font-family:Arial; font-size:16px; }"
			"#menu { margin-left: auto; margin-right: 0px; text-align: center; width: 600px; min-height: 50px; "
				"padding: 1em;  margin-bottom: 2em; background-color: orange; "
				"border-bottom: solid darkorange 15px; border-bottom-left-radius: 70px;}"
			"#menu H1 { color:white; text-align: center; padding-left: 1em;}"
			"#menu ul {list-style:none; margin: 0; padding-top: 2em; }"
			"#menu ul li { display: inline-block; padding: 2px; margin-left:1em; margin-right: 1em }"
			"#menu a:hover { color: black; text-decoration: overline underline; font-weight: bold}"
			"#menu a { color: black; text-decoration: none }"
			"#content { margin-left: auto; margin-right: auto; text-align: center; width: 600px; min-height 500px; "
				"padding: 1em;  margin-bottom: 2em; background-color: cornsilk; color: black;"
				"border: darkorange 2px solid; border-top: darkorange 30px solid; border-top-left-radius: 10px; border-top-right-radius: 10px; }"
			"#content ul { text-align: left; }"
			"#content ui li {text-align: left; }"
		"</style>"
		"<title>%s</title>"
	"</head>"
	"<body>"
		"<div id=\"menu\">"
			"<H1>%s</H1>"
			"<ul >"
				"<li> <a href=\"index.html\">Main</a></li>"
				"<li> <a href=\"info.html\">Information</a></li>"
				"<li> <a href=\"data.html\">Data</a></li>"
				"<li> <a href=\"setting.html\">Settings</a></li>"
			"</ul>"
		"</div>"
		"<div id=\"content\">"
		, title, title );
	
	
	return len;
}


uint32_t HTML_GetFoot( char* pageDef )
{
	uint32_t len =
	sprintf( pageDef,
			"</div>"
		"</body>"
	"</html>"
	);
	return len;
}

