/*
 * html_pages.c
 *
 * Created: 25.05.2015 00:09:20
 *  Author: ech0
 */ 

#include "html_pages.h"
#include "malloc.h"
#include "../DualportRAM/Dualportram_Storage.h"

HTMLPageDef gIndexHTML = {
	.PageName = "index.html",
	.PageContent = 
	"<html>"
	"<head>"
	"<style type=\"text/css\">"
	"body { background-color:#E0E0E0; font-weight:bold; font-family:Arial; font-size:120%; }"
	"</style>"
	"<title>Bodo Janssen webserver</title>"
	"</head"
	"<body>"
	"Hier folgt der Inhalt <p>"
	"<a href=\"./data.html\">zu den Daten </a>"
	"</form>"
	"</body>"
	"</html>"
};

HTMLPageDef gDataHTML = {
	.PageName = "data.html",
	.PageContent = NULL,
	}; 

void HTMLPages_InitDataPage()
{
	gDataHTML.PageContent = malloc( 1024);
}

void HTMLPages_UpdateDataPage()
{	
	uint16_t* dualram = DualPortRAM_GetBuffer();
	
	sprintf( gDataHTML.PageContent, ""
	"<html>"
	"<head>"
	"<style type=\"text/css\">"
	"body { background-color:#e00000; font-weight:bold; font-family:Arial; font-size:120%; }"
	"</style>"
	"<title>Bodo Janssen webserver</title>"
	"</head"
	"<body>"
	"Daten <p>"
	"%i, %i, %i <p>"
	"</body>"
	"</html>",
	
	dualram[0], dualram[1], dualram[2] );
	
}