/*
 * Adc.h
 *
 *  Created on: Feb 5, 2015
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#include "BaseDef.h"
#include "Adc_Cfg.h"

void Adc_Init(void);
StdReturnType Adc_StartConversion(uint16_t *resultBuffer, uint8_t count);
Boolean Adc_ConversionFinished(void);

/* Callback */
void Adc_ConversionFinished_Indication(void);

#endif /* ADC_ADC_H_ */
