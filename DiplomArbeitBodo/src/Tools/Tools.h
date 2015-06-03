/*
 * Tools.h
 *
 * Created: 24.05.2015 18:37:18
 *  Author: ech0
 */ 


#ifndef TOOLS_H_
#define TOOLS_H_

#include "asf.h"

uint32_t ParseIpToArray( char* ipAstring, uint8_t** ipArray );

char *strwrd(char *s, char *buf, size_t len, char *delim);

#endif /* TOOLS_H_ */