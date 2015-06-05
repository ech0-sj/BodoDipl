/*
 * ModbusTCP_slave.h
 *
 * Created: 05.06.2015 18:01:53
 *  Author: ech0
 */ 


#ifndef MODBUSTCP_SLAVE_H_
#define MODBUSTCP_SLAVE_H_

#include <asf.h>
#include "../Ethernet/Ethernet.h"

#define MODBUS_BUF_SIZE 512


typedef enum 
{
	Mod_Read_Coil = 1,
	Mod_Read_DiscreteInput,
	Mod_Read_Register,
	Mod_Read_Input,
	Mod_Write_Coil,
	Mod_Write_Register,
	Mod_Unknown,
		
}ModbusFunc;


void Modbus_DoSlaveTCP( SOCKET sock );



#endif /* MODBUSTCP_SLAVE_H_ */