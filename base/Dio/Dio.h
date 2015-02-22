/*
 * Dio.h
 *
 *  Created on: Nov 2, 2014
 *      Author: mepping
 */

#ifndef DIO_DIO_H_
#define DIO_DIO_H_

#include "StdDef.h"
#include "Dio_Cfg.h"

void digitalWrite(uint8_t, uint8_t);
uint8_t digitalRead(uint8_t pin);
void pinMode(uint8_t pin, uint8_t mode);


#endif /* DIO_DIO_H_ */
