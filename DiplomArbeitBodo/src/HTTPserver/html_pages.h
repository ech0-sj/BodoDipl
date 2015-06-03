/*
 * html_pages.h
 *
 * Created: 25.05.2015 00:09:42
 *  Author: ech0
 */ 


#ifndef HTML_PAGES_H_
#define HTML_PAGES_H_

#define HTMLPAGE_DEFUALT_SIZE	512

typedef struct  
{
	char* PageName; 
	char PageContent[HTMLPAGE_DEFUALT_SIZE]; 
}HTMLPageDef;


HTMLPageDef* HTMLPagesGet_index( void );
void HTMLPagesCreate_index( void );
void HTMLPagesUpdate_index( void );

HTMLPageDef* HTMLPagesGet_data( void );
void HTMLPagesCreate_data( void );
void HTMLPagesUpdate_data( void );


HTMLPageDef* HTMLPagesGet_setting( void );
void HTMLPagesCreate_setting( void );
void HTMLPagesUpdate_setting( void );


#endif /* HTML_PAGES_H_ */