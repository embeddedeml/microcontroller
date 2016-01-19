/*
 * ctrl.cpp
 *
 *  Created on: Oct 10, 2015
 */


#include <stdio.h>
#include <termios.h>
#include <string.h>

#include "joystick.h"
#include "ctrl_raw.h"


#define KEY_ESC       (27)
#define KEY_FORWARD   ('w')
#define KEY_BACKWARD  ('s')
#define KEY_RIGHT     ('d')
#define KEY_LEFT      ('a')
#define KEY_SPEEDUP   ('.')
#define KEY_SPEEDDOWN (',')
#define KEY_LED       ('l')
#define KEY_INPUT     (' ')

struct termios Input_TermiosBackup;
FILE *Output_FD;


unsigned char Ctrl_Input;

void Input_ResetTerminalMode()
{
    tcsetattr(0, TCSANOW, &Input_TermiosBackup);
}

void Input_SetConioTerminalMode()
{
    struct termios termiosNew;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &Input_TermiosBackup);
    memcpy(&termiosNew, &Input_TermiosBackup, sizeof(termiosNew));

    /* register cleanup handler, and set the new terminal mode */
    atexit(Input_ResetTerminalMode);
    cfmakeraw(&termiosNew);
    tcsetattr(0, TCSANOW, &termiosNew);
}

int Input_GetChar()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        return r;
    }
    else
    {
        return c;
    }
}

int Input_ReadKey()
{
    int key = Input_GetChar();

    switch(key)
    {
    case KEY_SPEEDUP:
        Ctrl_Raw_IncSpeed();
        break;

    case KEY_SPEEDDOWN:
        Ctrl_Raw_DecSpeed();
        break;

    case KEY_RIGHT:
        Ctrl_Raw_IncLeftRight();
        break;

    case KEY_LEFT:
        Ctrl_Raw_DecLeftRight();
        break;

    case KEY_FORWARD:
        Ctrl_Raw_IncForwardBackward();
        break;

    case KEY_BACKWARD:
        Ctrl_Raw_DecForwardBackward();
        break;

    case KEY_LED:
        Ctrl_Raw_ToggleLed();
        break;

    case KEY_INPUT:
        if(Ctrl_Input == 0)
        {
            if(open_joystick("/dev/input/js0") < 0)
            {
                printf("Could not access joystick");
                Ctrl_Input = 0;
            }
            else
            {
                Ctrl_Input = 1;
            }
        }
        else
        {
            close_joystick();
            Ctrl_Input = 0;
        }
        break;

    case KEY_ESC:
        return 1;
    }

    return 0;
}

int Input_ReadJoystick()
{
    struct js_event jse;
    while(read_joystick_event(&jse) != 1)
    {
        /* wait for event */
    }

    int temp;
    jse.type &= ~JS_EVENT_INIT; /* ignore synthetic events */
    if (jse.type == JS_EVENT_AXIS) {
        switch (jse.number) {
        case 0:
            temp = (jse.value / 5461) + 8;
            Ctrl_Raw_SetLeftRight((unsigned char) temp);
            break;

        case 1:
            temp = -1 * jse.value;
            temp = (temp / 5461) + 8;
            Ctrl_Raw_SetForwardBackward((unsigned char) temp);
            break;

        case 2:
            temp = jse.value + 32767;
            temp = (temp / 819);
            Ctrl_Raw_SetSpeed((unsigned char) (80 - temp));
            break;

        default:
            break;
        }
    }
    else if((jse.type == JS_EVENT_BUTTON) && (jse.value == 1))
    {
        switch(jse.number)
        {
        case 0:
            Ctrl_Raw_ToggleLed();
            break;

        case 4:
            Ctrl_Raw_IncThrottle();
            Ctrl_Raw_UpdateLeftRight();
            break;

        case 6:
            Ctrl_Raw_DecThrottle();
            Ctrl_Raw_UpdateLeftRight();
            break;

        case 1:
            /* exit */
            return 1;
        }
    }

    return 0;
}


int main(void)
{
    int exit = 0;

    Ctrl_Raw_Init();
    Ctrl_Input = 0;

    Output_FD = NULL;

    Output_FD = fopen("/dev/ttyACM0", "wb");
    if(Output_FD == NULL)
    {
        printf("Could not open terminal\n");
        return -1;
    }

    Input_SetConioTerminalMode();

    do
    {
        Ctrl_Raw_SendFrame(Output_FD);

        if(Ctrl_Input == 0)
        {
            exit = Input_ReadKey();
        }
        else
        {
            exit = Input_ReadJoystick();
        }

    } while(exit == 0);

    Ctrl_Raw_Init();
    Ctrl_Raw_SendFrame(Output_FD);

    fclose(Output_FD);
    Input_ResetTerminalMode();

    return 0;
}
