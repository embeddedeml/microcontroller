/*
 * Adc.cpp
 *
 *  Created on: Feb 5, 2015
 */

#include "Adc.h"

#define ADC_STATUS_IDLE (0U)
#define ADC_STATUS_BUSY (1U)

/* @var Current status of adc */
static uint8_t Adc_Status;
/* @var Channel number, which is currently converted */
static uint8_t Adc_CurrentChannel;
/* @var Number of iteration, if multiple conversion is running */
static uint8_t Adc_CurrentSample;
/* @var Pointer to result buffer provided by application */
static uint16_t *Adc_ResultBuffer;

/* @var Array with values for mux register (only active channels) */
static uint8_t Adc_MuxValues[ADC_CFG_NROFCHANNELS + 1] =
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

/**
 * @brief Initializes Adc hardware unit and starts dummy conversion
 */
void Adc_Init(void)
{
    Adc_Status = ADC_STATUS_BUSY;

    ADMUX   = (ADC_CFG_REF << REFS0); /* Reference Voltage, Channel0 */
    ADCSRA  = ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); /* Prescaler: 128 */
    ADCSRA |= (1 << ADEN); /* Enable ADC */
    ADCSRA |= (1 << ADSC); /* dummy conversion */

    while((ADCSRA & (1 << ADSC)) > 0x00U)
        ;

    Adc_CurrentChannel = 0x00U;
    Adc_CurrentSample  = 0x00U;
    Adc_Status = ADC_STATUS_IDLE;
}

/**
 * @brief Enables ADC. API must be called after sleep mode.
 */
void Adc_Enable(void)
{
    ADCSRA |= (1 << ADEN); /* Enable ADC */
}

/**
 * @brief Disables ADC. API must be called before sleep mode.
 */
void Adc_Disable(void)
{
    ADCSRA &= ~(1 << ADEN); /* Disable ADC */
}

/**
 * @brief Starts conversion of all configured channels
 *
 * @param   resultBuffer    Pointer to result buffer. Must be large enough for results of all channels and all samples
 * @param   count           Number of samples (of each channel)
 *
 * @return  E_OK            Conversion started
 * @return  E_NOT_OK        Adc not initialized or previous conversion not finished
 */
StdReturnType Adc_StartConversion(uint16_t *resultBuffer, uint8_t count)
{
    StdReturnType retVal = E_NOT_OK;

    if(Adc_Status == ADC_STATUS_IDLE)
    {
        Adc_Status = ADC_STATUS_BUSY;

        Adc_CurrentChannel = 0x00U;
        Adc_CurrentSample  = count;
        Adc_ResultBuffer   = resultBuffer;

        ADMUX   = ((ADC_CFG_REF << REFS0) | (Adc_MuxValues[Adc_CurrentChannel]));
        ADCSRA |= ((1 << ADSC) | (1 << ADIE)); /* Start conversion, enable interrupt */

        retVal = E_OK;
    }

    return retVal;
}

/**
 * @brief Returns status of conversion
 *
 * @return  TRUE    conversion finished
 * @return  FALSE   conversion not finished
 */
boolean Adc_ConversionFinished(void)
{
    boolean retVal = false;
    if(Adc_Status == ADC_STATUS_IDLE)
    {
        retVal = true;
    }

    return retVal;
}

/**
 * @brief Adc conversion finished interrupt
 *
 * New conversion is started automatically until all channels
 * are processed.
 */
ISR(ADC_vect)
{
    uint16_t result;
    result = ADCL;
    result += (ADCH<<8);

    *Adc_ResultBuffer = result;

    Adc_CurrentChannel++;
    if(Adc_CurrentChannel < ADC_CFG_NROFCHANNELS)
    {
        Adc_ResultBuffer++;
        ADMUX = ((ADC_CFG_REF << REFS0) | (Adc_MuxValues[Adc_CurrentChannel]));
        ADCSRA |= (1<<ADSC);
    }
    else if(Adc_CurrentSample > 0x01U)
    {
        /* start new iteration */
        Adc_ResultBuffer++;
        Adc_CurrentSample--;
        Adc_CurrentChannel = 0x00U;

        ADMUX = ((ADC_CFG_REF << REFS0) | (Adc_MuxValues[Adc_CurrentChannel]));
        ADCSRA |= (1<<ADSC);
    }
    else
    {
        /* last iteration finished */
        Adc_Status = ADC_STATUS_IDLE;
        Adc_ConversionFinished_Indication();
    }
}
