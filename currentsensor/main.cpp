/*
 * blink.c
 *
 *  Created on: Oct 25, 2014
 *      Author: mepping
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

uint16_t Main_Adc_resultbuffer[ADC_CFG_NROFCHANNELS];
uint8_t switchState;

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
	if(Adc_startConversion(Main_Adc_resultbuffer) != E_OK)
	{
		Serial.println("Start conversion failed");
	}
}

void switchSwitch(void)
{
	pinMode(PIN_LCD, INPUT);
	pinMode(PIN_OW,  INPUT);
	digitalWrite(PIN_SD, LOW);

	if(switchState == 0x00)
	{
		switchState++;
		pinMode(PIN_LCD, OUTPUT);
		Serial.println("LCD: on");
	}
	else if(switchState == 0x01)
	{
		switchState++;
		pinMode(PIN_OW, OUTPUT);
		Serial.println("OW: on");
	}
	else
	{
		switchState=0;
		digitalWrite(PIN_SD, HIGH);
		Serial.println("SD: on");
	}
}

void getValue(void)
{
	if(Adc_conversionFinished() == true)
	{
		ks0070.clearDisplay();
		ks0070.returnCursor();

		ks0070.writeNumber(Main_Adc_resultbuffer[0]);
		ks0070.writeByte('-');
		ks0070.writeNumber(Main_Adc_resultbuffer[1]);
		ks0070.writeByte('-');
		ks0070.writeNumber(Main_Adc_resultbuffer[2]);

		Serial.print("Ubat: ");
		Serial.print(Main_Adc_resultbuffer[0]);
		Serial.print(" - Icharge: ");
		Serial.print(Main_Adc_resultbuffer[1]);
		Serial.print(" - Idischarge: ");
		Serial.println(Main_Adc_resultbuffer[2]);
	}
	else
	{
		Serial.println("conversion not finished");
	}
}

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
	scheduler.enableTask(4,   0,  3000,  &switchSwitch);

	timer1_init();
	Adc_init();
	ks0070.init();

	switchState=0;

	while(1)
	{
		scheduler.run();
	}
}

void Timer_Timer1_CHA_Interrupt(void)
{
	scheduler.incrementTicks();
}
