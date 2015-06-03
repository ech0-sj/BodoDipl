/*
 * SysTimer.h
 *
 * Created: 05.05.15 11:34:16
 *  Author: selim
 */ 


#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#include "asf.h"

uint64_t Get_systime_ms( void );

void Delay_ms(uint32_t delayInMs);

void Systimer_OnSecondChanged();

#endif /* SYSTIMER_H_ */