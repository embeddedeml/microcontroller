/*
 * base.h
 *
 *  Created on: Nov 2, 2014
 */

#ifndef BASE_H_
#define BASE_H_

#include "stdint.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define HIGH 	(0x01)
#define LOW  	(0x00)

#define INPUT 	(0x00)
#define OUTPUT 	(0x01)

#define STD_OFF	(0x00)
#define STD_ON	(0x01)

#define TRUE 	(0x01)
#define FALSE 	(0x00)

#define E_OK        (0x00)
#define E_NOT_OK    (0x01)

typedef uint8_t StdReturnType;
typedef uint8_t Boolean;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#endif /* BASE_H_ */
