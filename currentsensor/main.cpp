/*
 * main.cpp
 *
 *  Created on: Oct 25, 2014
 */

#include "BaseDef.h"
#include "Scheduler.h"
#include "Timer.h"
#include "Dio.h"
#include "debug.h"
#include "Adc.h"
#include "KS0070.h"
#include "Dem.h"

#define MAIN_FILTER_SIZE    (4)

Scheduler scheduler;
KS0070 ks0070;

uint16_t Main_Adc_Resultbuffer[ADC_CFG_NROFCHANNELS];

uint16_t Main_Adc_Ringbuffer_Ubat[MAIN_FILTER_SIZE];
uint16_t Main_Adc_Ringbuffer_Icharge[MAIN_FILTER_SIZE];
uint16_t Main_Adc_Ringbuffer_Idischarge[MAIN_FILTER_SIZE];

uint16_t Main_Filtered_Ubat;
uint16_t Main_Filtered_Icharge;
uint16_t Main_Filtered_Idischarge;

uint8_t  Main_Ringbuffer_Pos;

uint16_t filterValue(uint16_t *input)
{
    uint8_t  i;
    uint16_t filteredValue = 0x00U;

    for(i=0;i<MAIN_FILTER_SIZE;i++)
    {
        filteredValue += input[i];
    }
    return (filteredValue / MAIN_FILTER_SIZE);
}

void led_on(void)
{
	digitalWrite(PIN_LED, HIGH);
}

void led_off(void)
{
	digitalWrite(PIN_LED, LOW);
}

void startConversion(void)
{
    if(Adc_ConversionFinished())
    {
        /* filter previous result */
        Main_Filtered_Ubat       = filterValue(Main_Adc_Ringbuffer_Ubat);
        Main_Filtered_Icharge    = filterValue(Main_Adc_Ringbuffer_Icharge);
        Main_Filtered_Idischarge = filterValue(Main_Adc_Ringbuffer_Idischarge);

        /* read new values */
        (void) Adc_StartConversion(Main_Adc_Resultbuffer);
    }
    else
    {
        Serial.println("Error: Conversion not finished");
    }
}

void debug(void)
{
    uint8_t  counter;
    uint8_t  i;

    Dem_EventStatusType status;
#if 0
    ks0070.clearDisplay();
    ks0070.returnCursor();

    ks0070.writeNumber(Main_Adc_Resultbuffer[0]);
    ks0070.writeByte('-');
    ks0070.writeNumber(Main_Adc_Resultbuffer[1]);
    ks0070.writeByte('-');
    ks0070.writeNumber(Main_Adc_Resultbuffer[2]);
#endif
    /* Ubat */
    Serial.print("Ubat;");
    Serial.print(Main_Adc_Resultbuffer[0]);
    Serial.print(";");
    Serial.println(Main_Filtered_Ubat);

    /* Icharge */
    Serial.print("Icharge;");
    Serial.print(Main_Adc_Resultbuffer[1]);
    Serial.print(";");
    Serial.println(Main_Filtered_Icharge);

    /* Idischarge */
    Serial.print("Idischarge;");
    Serial.print(Main_Adc_Resultbuffer[2]);
    Serial.print(";");
    Serial.println(Main_Filtered_Idischarge);

    /* Event memory */
    for(i=0;i<DEM_CFG_NROFEVENTS;i++)
    {
        Dem_GetEventStatus(i, &status);
        Dem_GetOccurrenceCounter(i, &counter);

        if(counter > 0)
        {
            Serial.print("Event: ");
            Serial.print(i);
            Serial.print(" - Status: ");
            Serial.print((uint8_t) status);
            Serial.print(" - Occurrence Counter: ");
            Serial.println(counter);
        }
    }
    Serial.println(" ");
}

void monitor(void)
{

}

/**
 *
 * @return
 */
int main(void)
{
    uint8_t i;

	sei();

#if 0
	PRR1 |= 1<<PRUSB;
#else
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	USBDevice.attach();
	Serial.begin(115200);
#endif

	digitalWrite(PIN_LCD, LOW);
	digitalWrite(PIN_OW,  LOW);
	digitalWrite(PIN_SD,  LOW);

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_SD,  OUTPUT);
	pinMode(PIN_LCD, INPUT);
	pinMode(PIN_OW,  INPUT);

	Adc_Init();
	ks0070.init();
	Dem_Init();

	for(i=0;i<MAIN_FILTER_SIZE;i++)
	{
	    Main_Adc_Ringbuffer_Ubat[i]       = 0x00U;
	    Main_Adc_Ringbuffer_Icharge[i]    = 0x00U;
	    Main_Adc_Ringbuffer_Idischarge[i] = 0x00U;
	}
	Main_Ringbuffer_Pos = 0U;
	Main_Filtered_Ubat       = 0x00U;
	Main_Filtered_Icharge    = 0x00U;
	Main_Filtered_Idischarge = 0x00U;

    scheduler.init();
    scheduler.enableTask(0,    1,     5,  &startConversion);
    scheduler.enableTask(1,    0,  1000,  &led_on);
    scheduler.enableTask(2,  500,  1000,  &led_off);
    scheduler.enableTask(3,    0,  1000,  &debug);
    scheduler.enableTask(4, 1000,  1000,  &monitor);

    Timer_Timer1_Init();

	while(1)
	{
		scheduler.run();
	}
}

/* Callbacks */
void Adc_ConversionFinished_Indication(void)
{
    Main_Adc_Ringbuffer_Ubat[Main_Ringbuffer_Pos]       = Main_Adc_Resultbuffer[0];
    Main_Adc_Ringbuffer_Icharge[Main_Ringbuffer_Pos]    = Main_Adc_Resultbuffer[1];
    Main_Adc_Ringbuffer_Idischarge[Main_Ringbuffer_Pos] = Main_Adc_Resultbuffer[2];

    Main_Ringbuffer_Pos++;
    if(Main_Ringbuffer_Pos == MAIN_FILTER_SIZE)
    {
        Main_Ringbuffer_Pos = 0U;
    }
}

void Timer_Timer1_Interrupt(void)
{
	scheduler.incrementTicks();
}
