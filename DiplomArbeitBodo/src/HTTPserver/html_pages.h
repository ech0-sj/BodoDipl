/*
 * html_pages.h
 *
 * Created: 25.05.2015 00:09:42
 *  Author: ech0
 */ 


#ifndef HTML_PAGES_H_
#define HTML_PAGES_H_

typedef struct  
{
	char* PageName; 
	char* PageContent; 
}HTMLPageDef;


void HTMLPages_InitDataPage();
void HTMLPages_UpdateDataPage();

#endif /* HTML_PAGES_H_ */