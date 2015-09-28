/*
 * blink.c
 *
 *  Created on: Oct 25, 2014
 *      Author: mepping
 */

#include "BaseDef.h"
#include "Scheduler.h"
#include "Dio.h"
#include "debug.h"
#include "irmp.h"

Scheduler scheduler;
uint8_t scheduleCounter;

void led_on(void)
{
	digitalWrite(PIN_LED, HIGH);
}

void led_off(void)
{
	digitalWrite(PIN_LED, LOW);
}

static void
timer1_init (void)
{
    OCR1A   =  (F_CPU / F_INTERRUPTS) - 1;                                  // compare value: 1/15000 of CPU frequency
    TCCR1B  = (1 << WGM12) | (1 << CS10);                                   // switch CTC Mode on, set prescaler to 1
    TCCR1A  = 0;

    TIMSK1  = 1 << OCIE1A;                                                  // OCIE1A: Interrupt by timer compare
}

ISR(TIMER1_COMPA_vect)                                                             // Timer1 output compare A interrupt service routine, called every 1/15000 sec
{
  (void) irmp_ISR();

  scheduleCounter++;
  if(scheduleCounter == 15)
  {
	  scheduler.incrementTicks();
	  scheduleCounter = 0;
  }
}

int main(void)
{
	IRMP_DATA irmp_data;

	sei();

#if 0
	PRR1 |= 1<<PRUSB;
#else
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	USBDevice.attach();
	Serial.begin(115200);
#endif

	pinMode(PIN_LED, OUTPUT);

	scheduler.init();
	scheduler.enableTask(0,   0,  1000,  &led_on);
	scheduler.enableTask(1, 500,  1000,  &led_off);

	scheduleCounter = 0;
    irmp_init();
    timer1_init();

	while(1)
	{
		scheduler.run();

		if (irmp_get_data (&irmp_data))
		{
			Serial.print("Protocol: ");
			Serial.print(irmp_data.protocol);

			Serial.print(" - address: 0x");
			Serial.print(irmp_data.address);

			Serial.print(" - command: ");
			Serial.print(irmp_data.command);

			Serial.print(" - flags: ");
			Serial.println(irmp_data.flags);
		}
	}
}


