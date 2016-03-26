/*
 * AdcIf.cpp
 *
 *  Created on: Mar 26, 2016
 */

#include "AdcIf.h"
#include "Adc.h"

#define ADCIF_FILTER_SIZE (4)

uint16_t AdcIf_Resultbuffer[ADC_CFG_NROFCHANNELS * ADCIF_FILTER_SIZE];
uint16_t AdcIf_Filtered[ADC_CFG_NROFCHANNELS];

/* Prototypes - Helper */
static uint16_t AdcIf_FilterChannel(uint8_t channel);
static void AdcIf_StartConversion(void);

void AdcIf_Init(void)
{
  uint8_t i;
  for(i = 0; i < ADC_CFG_NROFCHANNELS; i++)
  {
    AdcIf_Filtered[i] = 0x00U;
  }

  Adc_Init();
}

void AdcIf_MainFunction(void)
{
  AdcIf_StartConversion();
}

uint16_t AdcIf_GetValue(uint8_t channel)
{
  return AdcIf_Filtered[channel];
}

static void AdcIf_StartConversion(void)
{
  /* re-enable ADC after sleep mode */
  Adc_Enable();

  /* read new values */
  (void) Adc_StartConversion(AdcIf_Resultbuffer, ADCIF_FILTER_SIZE);

  while(Adc_ConversionFinished() == false)
  {
  }

  /* disable ADC to avoid conversion by sleep mode */
  Adc_Disable();

  /* filter result */
  AdcIf_Filtered[0] = AdcIf_FilterChannel(0);
  AdcIf_Filtered[1] = AdcIf_FilterChannel(1);
  AdcIf_Filtered[2] = AdcIf_FilterChannel(2);
}

static uint16_t AdcIf_FilterChannel(uint8_t channel)
{
  uint8_t i;
  uint16_t filteredValue = 0x00U;

  for(i = 0; i < ADCIF_FILTER_SIZE; i++)
  {
    filteredValue += AdcIf_Resultbuffer[channel + i * ADC_CFG_NROFCHANNELS];
  }
  return (filteredValue / ADCIF_FILTER_SIZE);
}

