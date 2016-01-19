/*
 * Pwm.cpp
 *
 *  Created on: Oct 5, 2015
 */


#include "Pwm.h"

/**
 * @brief Initializes and starts Pwm channel 1
 */
void Pwm_Channel1_Start(uint16_t period_us)
{
    TCCR1A = (1 << COM1A0); /* toggle pin */
    TCCR1B = (1 << WGM12) | (1 << CS10); /* prescaler: 1 ; CTC */

    TCNT1  = 0;
    OCR1A  = ((F_CPU / 1000 / 1000) * period_us) - 1;
}

void Pwm_Channel1_Stop(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
}
