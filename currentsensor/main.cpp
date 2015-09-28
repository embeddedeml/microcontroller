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
#include "Dem.h"
#include "SDLogger.h"

#include <avr/sleep.h>

#define MAIN_FILTER_SIZE                        (4)
#define MAIN_THRESHOLD_OVERVOLTAGE              (1022)
#define MAIN_THRESHOLD_UNDERVOLTAGE             (1)
#define MAIN_THRESHOLD_OVERCURRENT_ICHARGE      (1022)
#define MAIN_THRESHOLD_OVERCURRENT_IDISCHARGE   (1022)

Scheduler scheduler;
/* KS0070 ks0070; */
SDLogger logger("log.txt");

uint16_t Main_Adc_Resultbuffer[ADC_CFG_NROFCHANNELS*MAIN_FILTER_SIZE];

uint16_t Main_Filtered_Ubat;
uint16_t Main_Filtered_Icharge;
uint16_t Main_Filtered_Idischarge;


/* Prototypes - Helper */
static uint16_t Main_FilterChannel(uint8_t channel);

/* Helper */
static uint16_t Main_FilterChannel(uint8_t channel)
{
    uint8_t  i;
    uint16_t filteredValue = 0x00U;

    for(i=0;i<MAIN_FILTER_SIZE;i++)
    {
        filteredValue += Main_Adc_Resultbuffer[channel+i*ADC_CFG_NROFCHANNELS];
    }
    return (filteredValue / MAIN_FILTER_SIZE);
}

void startConversion(void)
{
    /* read new values */
    (void) Adc_StartConversion(Main_Adc_Resultbuffer, MAIN_FILTER_SIZE);

    while(Adc_ConversionFinished() == false)
    {
    }

    /* filter result */
    Main_Filtered_Ubat       = Main_FilterChannel(0);
    Main_Filtered_Icharge    = Main_FilterChannel(1);
    Main_Filtered_Idischarge = Main_FilterChannel(2);
}

void monitor(void)
{
    if(Main_Filtered_Ubat > MAIN_THRESHOLD_OVERVOLTAGE)
    {
        Dem_SetEventStatus(DEM_EVENT_OVERVOLTAGE, DEM_EVENTSTATUS_FAILED);
    }
    else
    {
        Dem_SetEventStatus(DEM_EVENT_OVERVOLTAGE, DEM_EVENTSTATUS_PASSED);
    }

    if(Main_Filtered_Ubat < MAIN_THRESHOLD_UNDERVOLTAGE)
    {
        Dem_SetEventStatus(DEM_EVENT_UNDERVOLTAGE, DEM_EVENTSTATUS_FAILED);
    }
    else
    {
        Dem_SetEventStatus(DEM_EVENT_UNDERVOLTAGE, DEM_EVENTSTATUS_PASSED);
    }

    if(Main_Filtered_Icharge > MAIN_THRESHOLD_OVERCURRENT_ICHARGE)
    {
        Dem_SetEventStatus(DEM_EVENT_OVERCURRENT_ICHARGE, DEM_EVENTSTATUS_FAILED);
    }
    else
    {
        Dem_SetEventStatus(DEM_EVENT_OVERCURRENT_ICHARGE, DEM_EVENTSTATUS_PASSED);
    }

    if(Main_Filtered_Idischarge > MAIN_THRESHOLD_OVERCURRENT_IDISCHARGE)
    {
        Dem_SetEventStatus(DEM_EVENT_OVERCURRENT_IDISCHARGE, DEM_EVENTSTATUS_FAILED);
    }
    else
    {
        Dem_SetEventStatus(DEM_EVENT_OVERCURRENT_IDISCHARGE, DEM_EVENTSTATUS_PASSED);
    }
}

void logData(void)
{
    StdReturnType retVal = E_OK;

    digitalWrite(PIN_SD, HIGH);

    retVal |= logger.appendNumber(Main_Filtered_Ubat);
    retVal |= logger.appendChar(';');
    retVal |= logger.appendNumber(Main_Filtered_Icharge);
    retVal |= logger.appendChar(';');
    retVal |= logger.appendNumber(Main_Filtered_Idischarge);
    retVal |= logger.appendChar('\r');
    retVal |= logger.appendChar('\n');
    retVal |= logger.force();

    digitalWrite(PIN_SD, LOW);

    if(retVal == E_OK)
    {
        Dem_SetEventStatus(DEM_EVENT_SDCARD, DEM_EVENTSTATUS_PASSED);
    }
    else
    {
        Dem_SetEventStatus(DEM_EVENT_SDCARD, DEM_EVENTSTATUS_FAILED);
        Serial.println("Access to SD card failed");
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

    if(Serial)
    {
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
}

void measure(void)
{
    uint8_t i;

    digitalWrite(PIN_LED, HIGH);

    for(i=0;i<MAIN_FILTER_SIZE;i++)
    {
        startConversion();
    }

	digitalWrite(PIN_LED, LOW);
}


/**
 *
 * @return
 */
int main(void)
{
	sei();

#if 0
	PRR1 |= 1<<PRUSB;
#else
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	USBDevice.attach();
	Serial.begin(115200);
#endif

	digitalWrite(PIN_LED, LOW);
	digitalWrite(PIN_SD,  LOW);
	digitalWrite(PIN_LCD, LOW);
	digitalWrite(PIN_OW,  LOW);

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_SD,  OUTPUT);
	pinMode(PIN_LCD, INPUT);
	pinMode(PIN_OW,  INPUT);

	Adc_Init();
	/* ks0070.init(); */
	Dem_Init();

	Main_Filtered_Ubat       = 0xFFFFU;
	Main_Filtered_Icharge    = 0xFFFFU;
	Main_Filtered_Idischarge = 0xFFFFU;
	logData();

    scheduler.init();
    scheduler.enableTask(0, SCHEDULER_MS_IN_TICKS(1000), SCHEDULER_MS_IN_TICKS(1000),  &measure);
    scheduler.enableTask(1, SCHEDULER_MS_IN_TICKS(1000), SCHEDULER_MS_IN_TICKS(1000),  &monitor);
    scheduler.enableTask(2, SCHEDULER_MS_IN_TICKS(1000), SCHEDULER_MS_IN_TICKS(1000),  &debug);
    scheduler.enableTask(3, SCHEDULER_MS_IN_TICKS(1000), SCHEDULER_MS_IN_TICKS(60000), &logData);

    Timer_Timer1_Init();

	while(1)
	{
		scheduler.run();
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
	}
}

/* Callbacks */
void Adc_ConversionFinished_Indication(void)
{
    /* unused */
}

void Timer_Timer1_Interrupt(void)
{
	scheduler.incrementTicks();
}
