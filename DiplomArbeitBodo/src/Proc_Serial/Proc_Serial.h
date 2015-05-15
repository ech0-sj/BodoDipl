/*
 * Proc_Serial.h
 *
 * Created: 15.05.2015 00:43:15
 *  Author: ech0
 */ 


#ifndef PROC_SERIAL_H_
#define PROC_SERIAL_H_

#include <asf.h>
#include "Usart/myUsart.h"
#include "Schedule/Scheduler.h"

#define PROCSER_CHAR_TIMEOUT	50		/* wenn 50ms kein Byte kam, angefangene msg verwerfen */ 
#define PROCSER_CONS_ENDBYTE	0x0A	/* Console endezeichen LF*/

#define CMD_BUF_MAX_LEN		2048
typedef struct  
{
	uint64_t lastRecv; 
	uint8_t buffer[CMD_BUF_MAX_LEN];
	uint32_t nextWrite;
	
}SerialCmdBuffer;

typedef struct
{
	SerialCmdBuffer cmdbuf[Serport_Count];
	
}serialProcStruct;



void ProcSerial( ProcessStruct* procStruct ); 
void ProcSerial_Init( ProcessStruct* procStruct );
void ProcSerial_InitCmdBuf( SerialCmdBuffer* buffer );


enum status_code ProcSerial_DoConsole( SerialCmdBuffer* cmdbuf );
enum status_code ProcSerial_DoWifi( SerialCmdBuffer* cmdbuf );




#endif /* PROC_SERIAL_H_ */