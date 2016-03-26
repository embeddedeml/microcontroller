/*
 * AdcIf.h
 *
 *  Created on: Mar 26, 2016
 */

#ifndef ADCIF_H_
#define ADCIF_H_

#include "BaseDef.h"

void AdcIf_Init(void);
void AdcIf_MainFunction(void);
uint16_t AdcIf_GetValue(uint8_t channel);

#endif /* ADCIF_H_ */
