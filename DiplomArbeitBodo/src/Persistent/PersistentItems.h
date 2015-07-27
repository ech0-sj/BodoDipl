/*
 * PersistentItems.h
 *
 * Created: 03.06.2015 17:53:32
 *  Author: ech0
 */ 


#ifndef PERSISTENTITEMS_H_
#define PERSISTENTITEMS_H_

#include "../asf.h"


typedef enum
{
	PersRes_Successful, 
	PersRes_NotFound,
	PersRes_Error, 
	PersRes_NotSupported,
	
}PersisentResult;




PersisentResult LoadIPWiznet( uint8_t* ip );
PersisentResult SaveIPWiznet( uint8_t* ip );

PersisentResult LoadMACWiznet( uint8_t* mac );
PersisentResult SaveMACWiznet( uint8_t* mac );

PersisentResult LoadSubnetWiznet( uint8_t* subnet );
PersisentResult SaveSubnetWiznet( uint8_t* subnet );

PersisentResult LoadModbusID( uint8_t* modbusid );
PersisentResult SaveModbusID( uint8_t* modbusid );

PersisentResult LoadSSIDName( uint8_t* ssid );
PersisentResult SaveSSIDName( uint8_t* ssid );

PersisentResult LoadSSIDPass( uint8_t* ssidpass );
PersisentResult SaveSSIDPass( uint8_t* ssidpass );

#endif /* PERSISTENTITEMS_H_ */