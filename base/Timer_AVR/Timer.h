/*
 * Timer.h
 *
 *  Created on: Nov 2, 2014
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "BaseDef.h"

extern void Timer_Timer1_Interrupt(void);
extern void Timer_Timer2_Interrupt(void);
extern void Timer_Timer3_Interrupt(void);

extern void Timer_Timer1_Start(uint16_t intervall_us);
extern void Timer_Timer1_Stop(void);

extern void Timer_Timer2_Start(uint16_t intervall_us);
extern void Timer_Timer2_Stop(void);

extern void Timer_Timer3_Start(uint16_t intervall_us);
extern void Timer_Timer3_Stop(void);

#endif /* TIMER_H_ */
