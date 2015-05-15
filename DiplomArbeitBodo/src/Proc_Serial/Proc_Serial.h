/*
 * Proc_Serial.h
 *
 * Created: 15.05.2015 00:43:15
 *  Author: ech0
 */ 


#ifndef PROC_SERIAL_H_
#define PROC_SERIAL_H_

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



void ProcSerial( void ); 
void ProcSerial_Init( void );


#endif /* PROC_SERIAL_H_ */