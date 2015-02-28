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

Scheduler scheduler;
KS0070 ks0070;

uint16_t Main_Adc_Resultbuffer[ADC_CFG_NROFCHANNELS];

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
	if(Adc_StartConversion(Main_Adc_Resultbuffer) != E_OK)
	{
		Serial.println("Start conversion failed");
	}
}

void getValue(void)
{
	if(Adc_ConversionFinished() == true)
	{
		ks0070.clearDisplay();
		ks0070.returnCursor();

		ks0070.writeNumber(Main_Adc_Resultbuffer[0]);
		ks0070.writeByte('-');
		ks0070.writeNumber(Main_Adc_Resultbuffer[1]);
		ks0070.writeByte('-');
		ks0070.writeNumber(Main_Adc_Resultbuffer[2]);

		Serial.print("Ubat: ");
		Serial.print(Main_Adc_Resultbuffer[0]);
		Serial.print(" - Icharge: ");
		Serial.print(Main_Adc_Resultbuffer[1]);
		Serial.print(" - Idischarge: ");
		Serial.println(Main_Adc_Resultbuffer[2]);
	}
	else
	{
		Serial.println("conversion not finished");
	}
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

	digitalWrite(PIN_LCD, LOW);
	digitalWrite(PIN_OW,  LOW);
	digitalWrite(PIN_SD,  LOW);

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_SD,  OUTPUT);
	pinMode(PIN_LCD, INPUT);
	pinMode(PIN_OW,  INPUT);

	scheduler.init();
	scheduler.enableTask(0,   0,  1000,  &led_on);
	scheduler.enableTask(1, 500,  1000,  &led_off);
	scheduler.enableTask(2,  50,  1000,  &startConversion);
	scheduler.enableTask(3, 550,  1000,  &getValue);

	Timer_Timer1_Init();
	Adc_Init();
	ks0070.init();

	while(1)
	{
		scheduler.run();
	}
}

void Timer_Timer1_Interrupt(void)
{
	scheduler.incrementTicks();
}
