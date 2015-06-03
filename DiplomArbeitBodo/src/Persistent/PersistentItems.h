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
	
}PersisentResult;




PersisentResult LoadIPWiznet( uint8_t* ip );
PersisentResult SaveIPWiznet( uint8_t* ip );

PersisentResult LoadMACWiznet( uint8_t* mac );
PersisentResult SaveMACWiznet( uint8_t* mac );

PersisentResult LoadSubnetWiznet( uint8_t* subnet );
PersisentResult SaveSubnetWiznet( uint8_t* subnet );


#endif /* PERSISTENTITEMS_H_ */