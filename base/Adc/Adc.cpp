/*
 * Adc.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: mepping
 */

#include "Adc.h"

uint8_t Adc_currentChannel;
uint16_t *Adc_resultBuffer;

uint8_t Adc_muxValues[ADC_CFG_NROFCHANNELS + 1] =
{
#if (ADC_CFG_CHANNEL_0 == STD_ON)
	0x00U,
#endif
#if (ADC_CFG_CHANNEL_1 == STD_ON)
	0x01U,
#endif
#if (ADC_CFG_CHANNEL_4 == STD_ON)
	0x04U,
#endif
#if (ADC_CFG_CHANNEL_5 == STD_ON)
	0x05U,
#endif
#if (ADC_CFG_CHANNEL_6 == STD_ON)
	0x06U,
#endif
#if (ADC_CFG_CHANNEL_7 == STD_ON)
	0x07U,
#endif
#if (ADC_CFG_CHANNEL_8 == STD_ON)
	0x80U,
#endif
#if (ADC_CFG_CHANNEL_9 == STD_ON)
	0x81U,
#endif
#if (ADC_CFG_CHANNEL_10 == STD_ON)
	0x82U,
#endif
#if (ADC_CFG_CHANNEL_11 == STD_ON)
	0x83U,
#endif
#if (ADC_CFG_CHANNEL_12 == STD_ON)
	0x84U,
#endif
#if (ADC_CFG_CHANNEL_13 == STD_ON)
	0x85U,
#endif
	0x00U /* dummy */
};

void Adc_init(void)
{
	ADMUX = (ADC_CFG_REF << REFS0); /* Reference Voltage, Channel0 */

	ADCSRA = ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); /* Prescaler */
	ADCSRA |= (1<<ADEN); /* Enable ADC */

	ADCSRA |= (1<<ADSC); /* dummy conversion */
	while((ADCSRA & (1<<ADSC)) > 0x00U);

	Adc_currentChannel = ADC_CFG_NROFCHANNELS;
}

StdReturnType Adc_startConversion(uint16_t *resultBuffer)
{
	uint8_t retVal = E_NOT_OK;

	if(Adc_currentChannel == ADC_CFG_NROFCHANNELS)
	{
		Adc_currentChannel = 0x00U;
		Adc_resultBuffer   = resultBuffer;
		ADMUX   = ((ADC_CFG_REF << REFS0) | (Adc_muxValues[Adc_currentChannel]));
		ADCSRA |= ((1<<ADSC) | (1<<ADIE)); /* Start conversion, enable interrupt */
		retVal  = E_OK;
	}

	return retVal;
}

Boolean Adc_conversionFinished(void)
{
	Boolean retVal = false;
	if(Adc_currentChannel == ADC_CFG_NROFCHANNELS)
	{
		retVal = true;
	}

	return retVal;
}

ISR(ADC_vect)
{
	uint16_t result;
	result  = ADCL;
	result += (ADCH<<8);

	*Adc_resultBuffer = result;

	Adc_currentChannel++;
	if(Adc_currentChannel < ADC_CFG_NROFCHANNELS)
	{
		Adc_resultBuffer++;
		ADMUX   = ((ADC_CFG_REF << REFS0) | (Adc_muxValues[Adc_currentChannel]));
		ADCSRA |= (1<<ADSC);
	}
}
