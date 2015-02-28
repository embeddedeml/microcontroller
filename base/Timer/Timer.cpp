/*
 * Timer.cpp
 *
 *  Created on: Nov 2, 2014
 */

#include "Timer.h"
#include "Timer_Cfg.h"

#if F_CPU < TIMER1_INTERRUPTS_PER_SEC
#error "Invalid configuration for timer1"
#endif

/**
 * @brief Initializes hardware timer 1
 */
void Timer_Timer1_Init(void)
{
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10); /* prescaler: 1 ; CTC Mode */

	OCR1A  = (F_CPU / TIMER1_INTERRUPTS_PER_SEC) - 1;
    TIMSK1 = (1 << OCIE1A);
}

/**
 * @brief Timer 1 overflow interrupt
 */
ISR (TIMER1_COMPA_vect)
{
	Timer_Timer1_Interrupt();
}
