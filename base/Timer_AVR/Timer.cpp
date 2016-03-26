/*
 * Timer.cpp
 *
 *  Created on: Nov 2, 2014
 */

#include "Timer.h"
#include "Timer_Cfg.h"


#if TIMER_TIMER1_USED == STD_ON
/**
 * @brief Initializes and starts hardware timer 1 (Counter 1 / 16-bit)
 */
void Timer_Timer1_Start(uint16_t intervall_us)
{
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS10); /* prescaler: 1 ; CTC Mode */

	OCR1A  = (((F_CPU / 1000 / 1000) * intervall_us) - 1);
	TCNT1  = 0;
    TIMSK1 = (1 << OCIE1A);
}

/**
 * @brief Stops hardware timer 1
 */
void Timer_Timer1_Stop(void)
{
    TIMSK1 = 0;
}

/**
 * @brief Timer 1 overflow interrupt
 */
ISR (TIMER1_COMPA_vect)
{
    Timer_Timer1_Interrupt();
}
#endif

#if TIMER_TIMER2_USED == STD_ON
/**
 * @brief Initializes and starts hardware timer 2 (Counter 3 / 16-bit)
 */
void Timer_Timer2_Start(uint16_t intervall_us)
{
    TCCR3A = 0;
    TCCR3B = (1 << WGM02) | (1 << CS00); /* prescaler: 1 ; CTC Mode */

    OCR3A  = (((F_CPU / 1000 / 1000) * intervall_us) - 1);
    TCNT3  = 0;
    TIMSK3 = (1 << OCIE3A);
}

/**
 * @brief Stops hardware timer 2
 */
void Timer_Timer2_Stop(void)
{
    TIMSK3 = 0;
}

/**
 * @brief Timer 2 overflow interrupt
 */
ISR (TIMER3_COMPA_vect)
{
    Timer_Timer2_Interrupt();
}
#endif

#if TIMER_TIMER3_USED == STD_ON
/**
 * @brief Initializes and starts hardware timer 3 (Counter 0 / 8-bit)
 */
void Timer_Timer3_Start(uint16_t intervall_us)
{
    TCCR0A = (1 << WGM01); /* CTC Mode */
    TCCR0B = (1 << CS01) | (1 << CS00); /* prescaler: 64  */

    OCR0A  = (intervall_us / (64 / (F_CPU / 1000 / 1000))) - 1;
    TCNT0  = 0;
    TIMSK0 = (1 << OCIE0A);
}

/**
 * @brief Stops hardware timer 3
 */
void Timer_Timer3_Stop(void)
{
    TIMSK0 = 0;
}

/**
 * @brief Timer 3 overflow interrupt
 */
ISR (TIMER0_COMPA_vect)
{
    Timer_Timer3_Interrupt();
}
#endif
