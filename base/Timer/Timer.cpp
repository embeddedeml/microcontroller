/*
 * Timer.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: mepping
 */

#include "Timer.h"
#include "Timer_Cfg.h"

#if F_CPU < TIMER1_INTERRUPTS_PER_SEC
#error "Invalid configuration for timer1"
#endif

extern void timer1_init(void)
{
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10); /* prescaler: 1 ; CTC Mode */

	OCR1A  = (F_CPU / TIMER1_INTERRUPTS_PER_SEC) - 1;
    TIMSK1 = (1 << OCIE1A);
}

ISR (TIMER1_COMPA_vect)
{
	Timer_Timer1_CHA_Interrupt();
}
