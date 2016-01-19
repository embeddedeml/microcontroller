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
#include "CopterCtrl.h"

#define MASTER1

#ifdef MASTER
#define Input_Serial    Serial
#define Output_Serial   Serial1
#else
#define Input_Serial    Serial1
#endif

Scheduler scheduler;
uint8_t ledState;

uint8_t Input_Cmd[4];
uint8_t Input_CmdValid;

void blink(void)
{
    if(ledState == 1)
    {
        //digitalWrite(PIN_LED, LOW);
        ledState = 0;
    }
    else
    {
        //digitalWrite(PIN_LED, HIGH);
        ledState = 1;
    }
}

#ifdef MASTER
void send_frame_master()
{
    if(Input_CmdValid == TRUE)
    {
        digitalWrite(PIN_LED, HIGH);
        CopterCtrl_SendFrame(Input_Cmd[0], Input_Cmd[1], Input_Cmd[2], Input_Cmd[3]);
    }
    /* do not reset receive flag to send frame cyclically */
}

void routing()
{
    if(Input_CmdValid == TRUE)
    {
        Output_Serial.write(0x55);
        Output_Serial.write(0xAA);
        Output_Serial.write(Input_Cmd[0]);
        Output_Serial.write(Input_Cmd[1]);
        Output_Serial.write(Input_Cmd[2]);
        Output_Serial.write(Input_Cmd[3]);
        Output_Serial.flush();
    }
}
#else

void send_frame_slave()
{
    if(Input_CmdValid == TRUE)
    {
        digitalWrite(PIN_LED, HIGH);
        CopterCtrl_SendFrame(Input_Cmd[0], Input_Cmd[1], Input_Cmd[2], Input_Cmd[3]);

        Input_CmdValid = FALSE;
    }
}

#endif

/**
 *
 * @return
 */
int main(void)
{
	sei();

#ifdef MASTER
    sbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);

    USBDevice.attach();
    Output_Serial.begin(115200);
#else
	PRR1 |= 1<<PRUSB;
#endif
	Input_Serial.begin(115200);

	/* LED */
	digitalWrite(PIN_LED, LOW);
	pinMode(PIN_LED, OUTPUT);
	ledState = 0;

    scheduler.init();
    scheduler.enableTask(0, SCHEDULER_MS_IN_TICKS(0),   SCHEDULER_MS_IN_TICKS(2000), &blink);
#ifdef MASTER
    scheduler.enableTask(1, SCHEDULER_MS_IN_TICKS(0),   SCHEDULER_MS_IN_TICKS(200),  &send_frame_master);
    scheduler.enableTask(2, SCHEDULER_MS_IN_TICKS(100), SCHEDULER_MS_IN_TICKS(200),  &routing);
#else
    scheduler.enableTask(1, SCHEDULER_MS_IN_TICKS(0),   SCHEDULER_MS_IN_TICKS(10),  &send_frame_slave);
#endif

    Timer_Timer3_Start(1000); /* 1ms - Scheduler Tick */

    Input_CmdValid = FALSE;
    uint8_t Input_State = 0;
	while(1)
	{
		scheduler.run();
		if(Input_Serial && (Input_Serial.available() > 0))
		{
		    uint8_t receivedByte = Input_Serial.read();
		    switch(Input_State)
		    {
		    case 0:
		        if(receivedByte == 0x55)
		        {
		            Input_State = 1;
		        }
		        break;

		    case 1:
                if(receivedByte == 0xAA)
                {
                    Input_CmdValid = FALSE;
                    Input_State = 2;
                }
                else
                {
                    Input_State = 0;
                }
                break;

		    case 2: /* byte 0 */
		    case 3: /* byte 1 */
		    case 4: /* byte 2 */
		    case 5: /* byte 3 */
		        Input_Cmd[Input_State-2] = receivedByte;

		        Input_State++;
		        if(Input_State > 5)
                {
                    Input_CmdValid = TRUE;
                    Input_State = 0;
                }
		        break;
		    }
		}
	}
}

/* Callbacks */
void Timer_Timer2_Interrupt(void)
{
    CopterCtrl_TimerCallback();
}

void Timer_Timer3_Interrupt(void)
{
    scheduler.incrementTicks();
}
