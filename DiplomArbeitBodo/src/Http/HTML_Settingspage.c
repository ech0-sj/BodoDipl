/*
 * HTML_Settingspage.c
 *
 * Created: 08.06.2015 20:23:16
 *  Author: ech0
 */ 

#include "HtmlPages.h"
#include "string.h"
#include "Tools/Tools.h"
#include "DataAccess/DataAccess.h"
#include "HtmlTools.h"
#include "Persistent/PersistentItems.h"
#include "Wiznet/Wiznet_Init.h"

int Extract_IPnSubnet( Stringlist* paramlist, uint8_t* ip, uint8_t* subnet );
int TryEnterValueInArray( uint8_t* array, uint8_t* paraStr, int index );
int Extract_Modbusaddress( Stringlist* paramlist, uint8_t* modAddr );


HtmlPageDescriptor HTML_CreatePage_Setting(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "setting.html" );
	descr.HtmlPageDescription = HTML_SettingPage;
	
	return descr;
}

void HTML_SettingPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	uint8_t ipaddr[4]; 
	uint8_t subaddr[4];
	uint8_t modaddr;
	LoadIPWiznet( ipaddr );
	LoadSubnetWiznet( subaddr);
	LoadModbusID(&modaddr); 
	
	len = HTML_GetHead( outPage_z, "Bodo Janssen - settings" );
	
	len += sprintf( &outPage_z[len],
	
	"<form action=\"settingethernet.html\" method=\"get\">"
		"<label style=\"{width=450px; text-align: center;}\">Ethernet setting</label>"
		"<table style=\"{width=450px;}\">"
			"<colgroup><col width=\"100\"><col width=\"250\"><col width=\"100\"></colgroup>"
			"<tr>"
				"<td><label>IP Address:</label></td>"
				"<td>"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wip1\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wip2\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\"   name=\"wip3\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wip4\">"
				"</td>"
				"<td rowspan=\"2\"><button type=\"submit\">update</button></td>"
			"</tr><tr>"
				"<td><label>Subnet mask:  </label></td>"
				"<td>"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wsn1\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wsn2\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wsn3\">"
					"<input type=\"number\" min=\"0\" max=\"255\" value=\"%i\" name=\"wsn4\">"
				"</td>"
			"</tr>"
		"</table>"
	"</form>"
	
	
	"<form action=\"settingmodbus.html\" method=\"get\">"
		"<label style=\"{width=450px; text-align: center;}\">Modbus setting</label>"
		"<table style=\"{width=450px;}\">"
			"<colgroup><col width=\"100\"><col width=\"250\"><col width=\"100\"></colgroup>"
			"<td>"
				"<label>Modbus address:"
			"</td><td>"
				"<input type=\"number\" min=\"0\" max=\"128\" value=\"%i\" name=\"modAdr\">"
			"</td><td>"
				"<button type=\"submit\">update</button>"
			"</td>"
		"</table>"
	"</form>"	
	,
	ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3],
	subaddr[0], subaddr[1], subaddr[2], subaddr[3],
	modaddr
	);
	
	len += HTML_GetFoot( &outPage_z[len] );
	
	return; 
}








HtmlPageDescriptor HTML_CreatePage_SettingEthernet(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "settingethernet.html" );
	descr.HtmlPageDescription = HTML_SettingEthernetPage;
	
	return descr;
}

void HTML_SettingEthernetPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint8_t IPArray[4]; 
	uint8_t SubArray[4]; 
	
	uint32_t len;
	int paramsFound = false; 
	
	Stringlist* paramlist = Html_ExtractParametersFromUrl( completeUrl_z );
	printf( "setting page params ");
	Stringlist_Print(paramlist );
	printf( "\n");
	
	if( !Extract_IPnSubnet(paramlist, IPArray, SubArray) )
		paramsFound = true; 
	
	Stringlist_Delete( paramlist );
	
	SaveIPWiznet( IPArray );
	SaveSubnetWiznet( SubArray); 
	W5500_Init();
	
	
	len = HTML_GetHead( outPage_z, "Bodo Janssen - settings" );
	
	if( paramsFound )
		len += sprintf( &outPage_z[len], "Success! Ethernet settings taken<p>" );
	else 
		len += sprintf( &outPage_z[len], "Error! Could not take ethernet settings. Please check made changes.<p>" );
	
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0;
}

int Extract_IPnSubnet( Stringlist* paramlist, uint8_t* ip, uint8_t* subnet )
{
	uint8_t value; 
	int itemsToGo = 8; 
	while( paramlist != NULL)
	{
		if( strstr( paramlist->string, "wip1") )
		{
			if( !TryEnterValueInArray(ip, paramlist->string, 0) )
				itemsToGo -= 1; 
		}
		if( strstr( paramlist->string, "wip2") )
		{
			if( !TryEnterValueInArray(ip, paramlist->string, 1) )
				itemsToGo -= 1;
		}
		if( strstr( paramlist->string, "wip3") )
		{
			if( !TryEnterValueInArray(ip, paramlist->string, 2) )
				itemsToGo -= 1;
		}
		if( strstr( paramlist->string, "wip4") )
		{
			if( !TryEnterValueInArray(ip, paramlist->string, 3) )
				itemsToGo -= 1;	
		}
		
		
		if( strstr( paramlist->string, "wsn1") )
		{
			if( !TryEnterValueInArray(subnet, paramlist->string, 0) )
				itemsToGo -= 1;
		}
		if( strstr( paramlist->string, "wsn2") )
		{
			if( !TryEnterValueInArray(subnet, paramlist->string, 1) )
				itemsToGo -= 1;
		}
		if( strstr( paramlist->string, "wsn3") )
		{
			if( !TryEnterValueInArray(subnet, paramlist->string, 2) )
				itemsToGo -= 1;
		}
		if( strstr( paramlist->string, "wsn4") )
		{
			if( !TryEnterValueInArray(subnet, paramlist->string, 3) )
				itemsToGo -= 1;
		}
		paramlist = Stringlist_NextItem(paramlist);
	}
	return itemsToGo;
}


int TryEnterValueInArray( uint8_t* array, uint8_t* paraStr, int index )
{
	uint8_t* pos;
	uint8_t value; 
	if( (pos = Html_GetParamValueStartPtr( paraStr )) != NULL )
	{
		value = atoi( pos );
		array[index] = value;
		return 0; 
	}
	return -1; 
}

















HtmlPageDescriptor HTML_CreatePage_SettingModbus(  )
{
	HtmlPageDescriptor descr;
	strcpy( descr.PageName, "settingmodbus.html" );
	descr.HtmlPageDescription = HTML_SettingModbusPage;
	
	return descr;
}

void HTML_SettingModbusPage( uint8_t* completeUrl_z, uint8_t* outPage_z )
{
	uint32_t len;
	uint8_t modAddress;
	int paramsFound = false;
	
	Stringlist* paramlist = Html_ExtractParametersFromUrl( completeUrl_z );
	printf( "setting page params ");
	Stringlist_Print(paramlist );
	printf( "\n");
	
	if( !Extract_Modbusaddress( paramlist, &modAddress) )
		paramsFound = true;
	
	Stringlist_Delete( paramlist );
	
	SaveModbusID(&modAddress);
	
	
	len = HTML_GetHead( outPage_z, "Bodo Janssen - settings" );
	
	if( paramsFound )
		len += sprintf( &outPage_z[len], "Success! modbusTCP settings taken<p>" );
	else
		len += sprintf( &outPage_z[len], "Error! Could not take modbusTCP settings. Please check made changes.<p>" );
	
	
	
	len += HTML_GetFoot( &outPage_z[len] );
	outPage_z[len] = 0;
}


int Extract_Modbusaddress( Stringlist* paramlist, uint8_t* modAddr )
{
	uint8_t* pos;
	while( paramlist != NULL )
	{
		if( strstr( paramlist->string, "modAdr"))
		{
			pos = Html_GetParamValueStartPtr( paramlist->string );
			if( pos )
			{
				*modAddr = atoi(pos); 
				return 0; 
			} 
		}
		paramlist = Stringlist_NextItem(paramlist);	
	}
	return -1; 
}