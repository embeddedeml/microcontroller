/*
 * ctrl_raw.c
 *
 *  Created on: Oct 14, 2015
 */

#include "ctrl_raw.h"

#define CTRL_RAW_SPEED_INIT           (0)
#define CTRL_RAW_FORWARDBACKWARD_INIT (8)
#define CTRL_RAW_LEFTRIGHT_INIT       (8)
#define CTRL_RAW_LED_INIT             (0)
#define CTRL_RAW_THROTTLE_INIT        (0)

unsigned char Ctrl_Raw_Speed;
unsigned char Ctrl_Raw_ForwardBackward;
unsigned char Ctrl_Raw_LeftRight;
  signed char Ctrl_Raw_Throttle;
unsigned char Ctrl_Raw_Led;

unsigned char Ctrl_Out[4];

void Ctrl_Raw_ToggleLed()
{
    if(Ctrl_Raw_Led == 1)
    {
        Ctrl_Raw_Led = 0;
    }
    else
    {
        Ctrl_Raw_Led = 1;
    }
}

void Ctrl_Raw_IncThrottle()
{
    if(Ctrl_Raw_Throttle < 6)
    {
        Ctrl_Raw_Throttle++;
    }
}
void Ctrl_Raw_DecThrottle()
{
    if(Ctrl_Raw_Throttle > -6)
    {
        Ctrl_Raw_Throttle--;
    }
}

void Ctrl_Raw_IncForwardBackward()
{
    if(Ctrl_Raw_ForwardBackward < 14)
    {
        Ctrl_Raw_ForwardBackward++;
    }
}
void Ctrl_Raw_DecForwardBackward()
{
    if(Ctrl_Raw_ForwardBackward > 1)
    {
        Ctrl_Raw_ForwardBackward--;
    }
}
void Ctrl_Raw_SetForwardBackward(unsigned char value)
{
    if((value >= 1) && (value <= 14))
    {
        Ctrl_Raw_ForwardBackward = value;
    }
    else
    {
        Ctrl_Raw_ForwardBackward = CTRL_RAW_FORWARDBACKWARD_INIT;
    }
}

void Ctrl_Raw_IncLeftRight()
{
    if(Ctrl_Raw_LeftRight < 14)
    {
        Ctrl_Raw_LeftRight++;
    }
}
void Ctrl_Raw_DecLeftRight()
{
    if(Ctrl_Raw_LeftRight > 1)
    {
        Ctrl_Raw_LeftRight--;
    }
}
void Ctrl_Raw_SetLeftRight(unsigned char value)
{
    signed char temp = Ctrl_Raw_Throttle + value;
    if(temp <= 1)
    {
        Ctrl_Raw_LeftRight = 1;
    }
    else if(temp >= 14)
    {
        Ctrl_Raw_LeftRight = 14;
    }
    else
    {
        Ctrl_Raw_LeftRight = temp;
    }
}
void Ctrl_Raw_UpdateLeftRight()
{
    Ctrl_Raw_SetLeftRight(CTRL_RAW_LEFTRIGHT_INIT);
}

void Ctrl_Raw_IncSpeed()
{
    if(Ctrl_Raw_Speed < 80)
    {
        Ctrl_Raw_Speed++;
    }
}
void Ctrl_Raw_DecSpeed()
{
    if(Ctrl_Raw_Speed > 1)
    {
        Ctrl_Raw_Speed--;
    }
    else
    {
        Ctrl_Raw_Speed = CTRL_RAW_SPEED_INIT;
    }
}
void Ctrl_Raw_SetSpeed(unsigned char value)
{
    if((value >= 1) && (value <= 80))
    {
        Ctrl_Raw_Speed = value;
    }
    else
    {
        Ctrl_Raw_Speed = CTRL_RAW_SPEED_INIT;
    }
}

void Ctrl_Raw_Init()
{
    Ctrl_Raw_Speed            = CTRL_RAW_SPEED_INIT;
    Ctrl_Raw_ForwardBackward  = CTRL_RAW_FORWARDBACKWARD_INIT;
    Ctrl_Raw_LeftRight        = CTRL_RAW_LEFTRIGHT_INIT;
    Ctrl_Raw_Led              = CTRL_RAW_LED_INIT;
    Ctrl_Raw_Throttle         = CTRL_RAW_THROTTLE_INIT;
}

unsigned char Ctrl_CalcCRC(void)
{
    unsigned char crc;

    crc  = Ctrl_Out[0];
    crc += Ctrl_Out[1];
    crc += Ctrl_Out[2];
    crc += Ctrl_Out[3];
    crc /= 16;
    crc &= 0x0F;

    return crc;
}

void Ctrl_Raw_SendFrame(FILE *fd)
{
    if((Ctrl_Raw_Speed <= 0) || (Ctrl_Raw_Speed > 0x50))
    {
        Ctrl_Out[0] = 0x80;
    }
    else
    {
        Ctrl_Out[0] = Ctrl_Raw_Speed;
    }

    Ctrl_Out[1] = ((Ctrl_Raw_LeftRight) << 4) | (Ctrl_Raw_ForwardBackward);
    Ctrl_Out[2] = 0x40;
    Ctrl_Out[3] = (Ctrl_Raw_Led << 7);
    Ctrl_Out[3] = Ctrl_Out[3] | Ctrl_CalcCRC();

    printf("%x %x %x %x\n", Ctrl_Out[0], Ctrl_Out[1], Ctrl_Out[2], Ctrl_Out[3]);

    fprintf(fd, "%c", 0x55);
    fflush(fd);
    fprintf(fd, "%c", 0xAA);
    fflush(fd);
    fprintf(fd, "%c", Ctrl_Out[0]);
    fflush(fd);
    fprintf(fd, "%c", Ctrl_Out[1]);
    fflush(fd);
    fprintf(fd, "%c", Ctrl_Out[2]);
    fflush(fd);
    fprintf(fd, "%c", Ctrl_Out[3]);
    fflush(fd);
}
