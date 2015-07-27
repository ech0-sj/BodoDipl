/*
 * conf_defaults.h
 *
 * Created: 27.07.2015 14:38:39
 *  Author: ech0
 */ 


#ifndef CONF_DEFAULTS_H_
#define CONF_DEFAULTS_H_


// #define DEBUG_ENABLE

#define ESP_DO_NOT_USE_PASSWORD	

#define DATA_BUF_SIZE 2048
#define DATA_BUF_SIZE_TX (4096)
#define HTML_MAX_DATA_PER_REQ 100




#ifdef DEBUG_ENABLE
	#define DebugOut		printf
	#define DebugConsole	USARTCons_Write
#else
	#define DebugOut		//
	#define DebugConsole	//
#endif 

#endif /* CONF_DEFAULTS_H_ */