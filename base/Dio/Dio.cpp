/*
 * Dio.c
 *
 *  Created on: Nov 2, 2014
 */

#include "Dio.h"

/**
 * @brief Set digital output pin value
 *
 * @param pin       Pin identifier (high nibble: port | low nibble: pin)
 * @param value     HIGH / LOW
 */
void digitalWrite(uint8_t pin, uint8_t value)
{
    uint8_t mask, portNr;

    value = (value & 0x01U); /* limit value range to [0..1] */
    portNr = ((pin & 0x70U) >> 4); /* port decoded in high nibble [0..5] */
    pin = (pin & 0x07U); /* pin index decoded in low nibble [0..7] */
    mask = ~(1 << pin);

    switch(portNr)
    {
    case 0: /* PORTA not available*/
        break;
    case 1: /* PORTB */
        PORTB = (PORTB & mask) | (value << pin);
        break;
    case 2: /* PORTC */
        PORTC = (PORTC & mask) | (value << pin);
        break;
    case 3: /* PORTD */
        PORTD = (PORTD & mask) | (value << pin);
        break;
    case 4: /* PORTE */
        PORTE = (PORTE & mask) | (value << pin);
        break;
    case 5: /* PORTF */
        PORTF = (PORTF & mask) | (value << pin);
        break;
    }
}

/**
 * @brief Get digital input pin value
 *
 * @param pin       Pin identifier (high nibble: port | low nibble: pin)
 * @return value     HIGH / LOW
 */
uint8_t digitalRead(uint8_t pin)
{
    uint8_t mask, portNr, value;

    portNr = ((pin & 0x70U) >> 4); /* port decoded in high nibble [0..5] */
    pin = (pin & 0x07U); /* pin index decoded in low nibble [0..7] */
    mask = (1 << pin);

    value = 0x00U;
    switch(portNr)
    {
    case 0: /* PORTA not available*/
        break;
    case 1: /* PORTB */
        value = (uint8_t)((PINB & mask) >> pin);
        break;
    case 2: /* PORTC */
        value = (uint8_t)((PINC & mask) >> pin);
        break;
    case 3: /* PORTD */
        value = (uint8_t)((PIND & mask) >> pin);
        break;
    case 4: /* PORTE */
        value = (uint8_t)((PINE & mask) >> pin);
        break;
    case 5: /* PORTF */
        value = (uint8_t)((PINF & mask) >> pin);
        break;
    }

    return (value & 0x01U);
}

/**
 * @brief Set digital pin direction
 *
 * @param pin       Pin identifier (high nibble: port | low nibble: pin)
 * @param mode      INPUT / OUTPUT
 */
void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t mask, portNr;

    mode = (mode & 0x01U); /* limit value range to [0..1] */
    portNr = ((pin & 0x70U) >> 4); /* port decoded in high nibble [0..7] */
    pin = (pin & 0x07U); /* pin index decoded in low nibble [0..7] */
    mask = ~(1 << pin);

    switch(portNr)
    {
    case 0: /* PORTA */
        /* not available */
        break;
    case 1: /* PORTB */
        DDRB = (DDRB & mask) | (mode << pin);
        break;
    case 2: /* PORTC */
        DDRC = (DDRC & mask) | (mode << pin);
        break;
    case 3: /* PORTD */
        DDRD = (DDRD & mask) | (mode << pin);
        break;
    case 4: /* PORTE */
        DDRE = (DDRE & mask) | (mode << pin);
        break;
    case 5: /* PORTF */
        DDRF = (DDRF & mask) | (mode << pin);
        break;
    }
}
