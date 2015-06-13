/*
 * HtmlPages.h
 *
 * Created: 06.06.2015 14:34:58
 *  Author: ech0
 */ 


#ifndef HTMLPAGES_H_
#define HTMLPAGES_H_

#include "HttpSimple.h"


// void (*HtmlPageDescription)( uint8_t* completeUrl_z, uint8_t* outPage_z );

HtmlPageDescriptor HTML_CreatePage_Index( );
void HTML_IndexPage( uint8_t* completeUrl_z, uint8_t* outPage_z );


HtmlPageDescriptor HTML_CreatePage_Info( );
void HTML_InfoPage( uint8_t* completeUrl_z, uint8_t* outPage_z );


HtmlPageDescriptor HTML_CreatePage_Data( );
void HTML_DataPage( uint8_t* completeUrl_z, uint8_t* outPage_z );

HtmlPageDescriptor HTML_CreatePage_DataRange( );
void HTML_DataRangePage( uint8_t* completeUrl_z, uint8_t* outPage_z );



HtmlPageDescriptor HTML_CreatePage_Setting( );
void HTML_SettingPage( uint8_t* completeUrl_z, uint8_t* outPage_z );

HtmlPageDescriptor HTML_CreatePage_SettingEthernet( );
void HTML_SettingEthernetPage( uint8_t* completeUrl_z, uint8_t* outPage_z );


HtmlPageDescriptor HTML_CreatePage_SettingModbus( );
void HTML_SettingModbusPage( uint8_t* completeUrl_z, uint8_t* outPage_z );




uint32_t HTML_GetHead( char* pageDef, char* title );
uint32_t HTML_GetFoot( char* pageDef );



#endif /* HTMLPAGES_H_ */