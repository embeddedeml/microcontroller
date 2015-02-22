/*
 * Adc.h
 *
 *  Created on: Feb 5, 2015
 *      Author: mepping
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "StdDef.h"
#include "Adc_Cfg.h"

void Adc_init(void);
StdReturnType Adc_startConversion(uint16_t *resultBuffer);
Boolean Adc_conversionFinished(void);

#endif /* ADC_ADC_H_ */
